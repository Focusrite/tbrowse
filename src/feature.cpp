#include <string>
#include <cmath>
#include <unordered_map>
#include <iostream>

#include "armadillo"

#include "feature.h"
#include "settings.h"
#include "sigprocess.h"

void AudioFeature::init(int seed) {
  if(seed != 0)
    arma::arma_rng::set_seed(seed);

  float maxSeconds = Settings::getFloat("maxSeconds");
  float secPerFrame = Settings::getFloat("frameSize");
  float secPerStep = Settings::getFloat("frameStep");
  int mfccPerFrame = Settings::getInteger("mfccPerFrame");


  AudioFeature::numFrames = (int)std::floor((maxSeconds - secPerFrame) / secPerStep);
  AudioFeature::unprojectedFeatureLength = AudioFeature::numFrames * mfccPerFrame;
  int numCol = AudioFeature::unprojectedFeatureLength;
  int numRow = Settings::getInteger("postProjectionSize");

  arma::fmat pm = arma::fmat(numRow, numCol, arma::fill::randn);
  // Orthonormailze the rows of the projection matrix
  arma::fmat sqrm = arma::sqrtmat_sympd(pm * pm.t());
  AudioFeature::projectionMatrix = sqrm * pm;
}

arma::fvec AudioFeature::getFeature() {
  return featureVector;
}

void AudioFeature::extractFromAudio(std::string path) {
  arma::fvec mfccVector = arma::fvec(AudioFeature::unprojectedFeatureLength, arma::fill::zeros);
  Audio a = Audio(path.c_str());
  // Error handling here

  int fs = a.getSamplerate();
  int samplesPerFrame = (int)(Settings::getFloat("frameSize") * fs);
  int samplesPerStep = (int)(Settings::getFloat("frameStep") * fs);
  arma::fvec data;
  arma::fvec* win = AudioFeature::getWindow(fs);
  arma::fmat* fbank = AudioFeature::getFilterbank(fs);
  int mpf = Settings::getInteger("mfccPerFrame");

  for(int i = 0; i < AudioFeature::numFrames; i++) {
    int startIndex = i * samplesPerStep;
    data = a.getData(startIndex, startIndex + samplesPerFrame);
    mfccVector.rows(i * mpf, (i + 1) * mpf - 1) = sigp::mfcc(&data, mpf, fbank,
      Settings::getInteger("NFFT"), win);
  }
  this->featureVector = AudioFeature::projectionMatrix * mfccVector;
}

void AudioFeature::finalize() {
  // Clean up window and filterbank memories.
  for(auto iter = AudioFeature::windowMap.begin(); iter != AudioFeature::windowMap.end(); ++iter)
    delete iter->second;
  AudioFeature::windowMap.clear();

  for(auto iter = AudioFeature::filterbankMap.begin(); iter != AudioFeature::filterbankMap.end(); ++iter)
    delete iter->second;
  AudioFeature::filterbankMap.clear();
}

arma::fvec* AudioFeature::getWindow(int fs) {
  if(AudioFeature::windowMap.count(fs))
    return AudioFeature::windowMap.at(fs);
  else {
    // It doesn't exist, so we create it and return the pointer.
    int samplesPerFrame = (int)(Settings::getFloat("frameSize") * fs);
    arma::fvec twin = sigp::hamming(samplesPerFrame); // Slightly suboptimal...
    arma::fvec* win = new arma::fvec(twin);
    AudioFeature::windowMap.insert({fs, win});
    return win;
  }
}

arma::fmat* AudioFeature::getFilterbank(int fs) {
  if(AudioFeature::filterbankMap.count(fs))
    return AudioFeature::filterbankMap.at(fs);
  else {
    // It doesn't exist, so we create it and return the pointer.
    int samplesPerFrame = (int)(Settings::getFloat("frameSize") * fs);
    arma::fmat tfbank = sigp::melfilterbank(samplesPerFrame, Settings::getInteger("mfccPerFrame"),
      Settings::getFloat("filterbankMinHz"), Settings::getFloat("filterbankMaxHz"), fs,
      Settings::getInteger("NFFT"));

    arma::fmat* fbank = new arma::fmat(tfbank);
    AudioFeature::filterbankMap.insert({fs, fbank});
    return fbank;
  }
}

AudioFeature::AudioFeature() {

}

AudioFeature::~AudioFeature() {

}

// Definitions
arma::fmat AudioFeature::projectionMatrix;
int AudioFeature::unprojectedFeatureLength;
int AudioFeature::numFrames;
std::unordered_map<int, arma::fvec*> AudioFeature::windowMap;
std::unordered_map<int, arma::fmat*> AudioFeature::filterbankMap;

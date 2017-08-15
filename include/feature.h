#pragma once
#include <string>
#include <unordered_map>

#include "armadillo"
#include "audioread.h"

class AudioFeature {
private:
  static arma::fmat projectionMatrix;
  static int unprojectedFeatureLength;
  static int numFrames;

  // Sample rate to appropriate window and filterbank mapping.
  static std::unordered_map<int, arma::fvec*> windowMap;
  static std::unordered_map<int, arma::fmat*> filterbankMap;

  arma::fvec featureVector;
public:
  static void init(int seed = 0);
  static void finalize();
  static arma::fvec* getWindow(int fs);
  static arma::fmat* getFilterbank(int fs);

  void extractFromAudio(std::string path);
  arma::fvec getFeature();
  AudioFeature();
  ~AudioFeature();
};

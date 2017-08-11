#include <vector>
#include <algorithm>
#include <stdlib.h>

#include "audioread.h"
#include "portsf/portsf.h"
#include "armadillo"

int Audio::getSamplerate() {
  return this->samplerate;
}
int Audio::getChannels() {
  return this->channels;
}
psf_stype Audio::getBitdepth() {
  return this->bitDepth;
}

arma::fvec Audio::getData(int start, int end, bool monoize) {
  //Error check, not loaded so return empty vector.
  if(this->bufferLength <= 0 || this->buf == 0)
    return std::vector<float>();

  int last = std::max(std::min(end, this->numSamples), 0);
  int first = std::max(std::min(start, last), 0);
  int length = last - first;

  if(monoize) { // We want to monoize
    arma::fvec res = arma::zeros<arma::fvec>(length);
    int index;
    for(int i = 0; i < length * this->channels; i++) {
      index = i / this->channels;
      if(i % this->channels <= 1) // Sum channels, omit channels over stereo
        res[index] += (this->channels > 1) ? buf[first + i] / 2.0 : buf[first + i];
    }
    return res;
  }
  else {
    return arma::fvec(buf[first], length * this->channels);
  }
}

int Audio::init() {
  return psf_init();
}
int Audio::finalize() {
  return psf_finish();
}

Audio::Audio(const char *path, float maxSeconds) {
  // TODO Add a lot of error handling
  psf_props properties;
  this->fd = psf_sndOpen(path, &properties, 0);
  if(this->fd < 0) {
    // Do error, we couldn't open it for some reason.
  }
  this->samplerate = properties.srate;
  this->channels = properties.chans;
  this->bitDepth = properties.samptype;

  this->seconds = maxSeconds;
  int numSamples = psf_sndSize(this->fd);
  if(this->seconds > 0)
    numSamples = std::min((int) (this->seconds * this->samplerate), numSamples);
  this->numSamples = numSamples;
  this->bufferLength = channels * numSamples;
  this->buf = (float*)malloc(sizeof(float) * bufferLength);
  int status = psf_sndReadFloatFrames(this->fd, this->buf, numSamples);
  if(status) {
    //Do something if error
  }

  psf_sndClose(this->fd);
}
Audio::~Audio() {
  if(this->buf != 0)
    free(this->buf);
}

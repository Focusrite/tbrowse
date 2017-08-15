#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

#include "portsf/portsf.h"
#include "armadillo"

#include "audioread.h"
#include "log.h"

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
    return arma::zeros<arma::fvec>(0);

  int last = std::max(std::min(end, this->numSamples), 0);
  int first = std::max(std::min(start, last), 0);
  int length = last - first;
  int resultLength = abs(end - start);

  if(monoize) { // We want to monoize
    arma::fvec res = arma::zeros<arma::fvec>(resultLength);
    int index;
    for(int i = 0; i < length * this->channels; i++) {
      index = i / this->channels;
      if(i % this->channels <= 1) // Sum channels, omit channels over stereo
        res[index] += (this->channels > 1) ? this->buf[first + i] / 2.0 : this->buf[first + i];
    }
    return res;
  }
  else {
    //TODO: make this resultLength too even if not that much data, to be consitent with above.
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
  if(this->fd < 0)
    logErr("Error opening file.", this->fd);

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
  if(status < 0)
    logErr("Error reading file.", status);

  status = psf_sndClose(this->fd);
  if(status < 0)
    logErr("Error closing file.", status);
}
Audio::~Audio() {
  if(this->buf != 0)
    free(this->buf);
}

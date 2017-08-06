#include "audioread.h"
#include "portsf.h"
#include <vector>
#include <math>
#include <stdlib>

int Audio::getSamplerate() {
  return this->samplerate;
}
int Audio::getChannels() {
  return this->channels;
}
psf_stype Audio::getBitdepth() {
  return this->bitDepth;
}

std::vector<float> Audio::getData(int start, int end, bool monoize = true) {
  //Error check, not loaded so return empty vector.
  if(this->bufferLength <= 0 || this->buf == 0)
    return std::vector<float>();

  unsigned int last = min(end, this->numSamples));
  unsigned int first = min(start, last);
  unsigned int length = last - first;

  //TODO: Special handling needed in case of stereo, but omitted for now
  if(monoize) { // We want to monoize
    std::vector<float> res(length, 0.0);
    int index;
    for(int i = 0; i < length * this->channels; i++) {
      index = i / this->channels;
      if(i % this->channels <= 1) // Sum channels, omit channels over stereo
        res[index] += (this->channels > 1) ? buf[first + i] / 2.0 : buf[first + i];
    }
  }
  else {
    std::vector<float> res(buf[first], buf[first] + length);
  }
}

static int Audio::init() {
  return psf_init();
}
static int Audio::finalize() {
  return psf_finish();
}

Audio::Audio(const char *path, float maxSeconds = -1.0) {
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
    numSamples = min((int) (this->seconds * this->samplerate), numSamples);
  this->numSamples = numSamples;
  this->bufferLength = channels * numSamples;
  this->buf = malloc(sizeof(float) * bufferLength);
  int status = psf_sndReadFloatFrames(this->fd, this->buf, numSamples);

  psf_sndClose(this->fd);
}
Audio::~Audio() {
  if(this->buf != 0)
    free(this->buf);
}

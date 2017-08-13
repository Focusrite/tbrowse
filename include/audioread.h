#pragma once
#include <vector>

#include "armadillo"
#include "portsf/portsf.h"

class Audio {
private:
  int fd;
  int samplerate;
  int channels;
  psf_stype bitDepth;
  float seconds;
  float* buf;
  int bufferLength;
  int numSamples;

public:
  static int init();
  static int finalize();
  int getSamplerate();
  int getChannels();
  psf_stype getBitdepth();
  arma::fvec getData(int start, int end, bool monoize = true);

  Audio(const char *path, float maxSeconds = -1.0);
  ~Audio();
};

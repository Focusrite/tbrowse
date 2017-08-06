#pragma once
#include "portsf.h"
#include <vector>

class Audio {
private:
  int fd;
  int samplerate;
  int channels;
  psf_stype bitDepth;
  float seconds = -1.0;
  float* buf = 0;
  unsigned int bufferLength = 0;
  int numSamples;

public:
  static int init();
  static int finalize();
  int getSamplerate();
  int getChannels();
  psf_stype getBitdepth();
  std::vector<float> getData(int start, int end, bool monoize = true);

  Audio(const char *path, bool monoize=true, float maxSeconds = -1.0);
  ~Audio();
};

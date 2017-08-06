#pragma once
#include "portsf.h"

class audio {
private:
  PSF_PROPS info;
  

public:
  float* getData();
  int getSamplerate();
  int getChannels();
  psf_stype getBitdepth();
  
  audio(const char *path);
}

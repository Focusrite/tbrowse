#pragma once
#include "portsf.h"

class audio {
private:
  PSF_PROPS info;
  

public:
  float* getData();
  int getFs();
  int getChannels();
  audio();
}

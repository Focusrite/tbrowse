#pragma once

#include "armadillo"
#include <string.h>

class Sound {
private:
  //arma::fmat features;
  float *sounddata;
  void calcMFCCFromWav(std::string path);

public:
  Sound();
  ~Sound();
};

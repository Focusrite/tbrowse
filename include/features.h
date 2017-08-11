#pragma once
#include <string>

#include "armadillo"

class Sound {
private:
  //arma::fmat features;
  float *sounddata;
  void calcMFCCFromWav(std::string path);

public:
  Sound();
  ~Sound();
};

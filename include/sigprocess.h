#pragma once

#include "armadillo"
#include <math.h>

namespace sigp {
  arma::fvec melscale(arma::fvec Hz);
  arma::fvec invmelscale(arma::fvec mel);
  arma::fmat melfilterbank(unsigned int length, unsigned int numFilters, float minHz, float maxHz,
    unsigned int fs, unsigned int NFFT);
  arma::fvec mfcc(float* input, unsigned int size, unsigned int numCoeff);
  arma::fvec dct(arma::fvec* x);
  arma::fvec periodogram(std::vector<float> input, unsigned int NFFT=512, arma::fvec* window = 0);
  arma::fvec hamming(unsigned int length);
}

#pragma once

#include "armadillo"
#include <math.h>

namespace sigp {
  arma::fvec melscale(arma::fvec Hz);
  arma::fvec invmelscale(arma::fvec mel);
  arma::fmat melfilterbank(int length, int numFilters, float minHz, float maxHz,
    int fs, int NFFT);
  arma::fvec mfcc(arma::fvec* input, unsigned int numCoeff, arma::fmat* filterbank,
    unsigned int NFFT, arma::fvec* window);
  arma::fvec dct(arma::fvec* x);
  arma::fvec periodogram(arma::fvec* input, unsigned int NFFT=512, arma::fvec* window = 0);
  arma::fvec hamming(unsigned int length);
}

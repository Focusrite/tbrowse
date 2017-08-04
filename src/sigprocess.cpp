#include <algorithm>
#include <iterator>
#include <vector>
#include <complex>

#include "armadillo"
#include "sigprocess.h"


namespace sigp {
  arma::fvec melscale(arma::fvec Hz) {
    return 1125 * arma::log(1 + Hz/700);
  }

  arma::fvec invmelscale(arma::fvec mel) {
    return 700 * arma::exp(mel/1125 - 1);
  }

  arma::fmat melfilterbank(unsigned int length, unsigned int numFilters, float minHz, float maxHz,
    unsigned int fs, unsigned int NFFT) {
    arma::fvec lower = melscale(arma::fvec(minHz));
    arma::fvec upper = melscale(arma::fvec(maxHz));
    arma::fvec midpts = arma::linspace<arma::fvec>(lower(0), upper(0), numFilters + 2);
    midpts = invmelscale(midpts);
    //Truncate to right FFT bin
    arma::vec fftpts = arma::conv_to<arma::vec>::from(arma::floor(((NFFT + 1) / fs) * midpts));

    // Do calculation of the actual filterbanks, one (frequency domain) filter per row.
    arma::fmat filterbanks = arma::zeros<arma::fmat>(numFilters, NFFT);
    for(unsigned int i = 0; i < numFilters; i++) {
      unsigned int startIndex = fftpts(i);
      unsigned int midIndex = fftpts(i + 1);
      unsigned int endIndex = fftpts(i + 2);
      filterbanks.submat(i, startIndex, i, midIndex) = arma::linspace<arma::frowvec>(0.0, 1.0, midIndex - startIndex);
      float nextLastValue = filterbanks(i, midIndex - 1);
      filterbanks.submat(i, midIndex + 1, i, endIndex) = arma::linspace<arma::frowvec>(nextLastValue, 0.0, endIndex - (midIndex + 1));
    }

    return filterbanks;
  }

  arma::fvec mfcc(std::vector<float> input, unsigned int numCoeff, arma::fmat* filterbank,
    unsigned int NFFT, arma::fvec* window) {
    arma::fmat Pxx = periodogram(input, NFFT, window);
    arma::fvec mfc = arma::zeros<arma::fvec>(numCoeff);
    for(unsigned int i = 0; i < numCoeff; i++) {
      mfc(i) = log(arma::accu(filterbank->col(i) % Pxx));
    }
    return dct(&mfc);
  }

  arma::fvec dct(arma::fvec* x) {
    //A faster implementation of the DCT (http://fourier.eng.hmc.edu/e161/lectures/dct/node2.html)
    unsigned int N = x->n_elem;
    arma::fvec y = arma::fvec(N);
    // Step 1:
    for(unsigned int m = 0; m < N/2; m++) {
      y(m) = (*x)(2*m);
      y(N - 1 - m) = (*x)(2 * m + 1);
    }
    // Step 2:
    arma::cx_fvec Y = arma::fft(y);
    // Step 3:
    //Not sure Armadillo can take complex numbers directly..
    arma::fvec exponent = arma::linspace<arma::fvec>(0, N - 1, N);
    exponent *= (arma::datum::pi / 2 * N);
    arma::fvec X = arma::real(exp(std::complex<float>(0.0f,-1.0f) * exponent) % Y);
    return X;
  }

  arma::fvec periodogram(std::vector<float> input, unsigned int NFFT, arma::fvec* window) {
    arma::fvec X = arma::fvec(input);
    if(window != 0) {
      X = X % *window;
    }
    arma::cx_fvec Y = arma::fft(X, NFFT); // TODO: Beware of where "0Hz" is, 0 or NFFT/2
    Y = Y.head(NFFT/2 + 1); // Since it's just the mirror, simpler to just loose some amplitude
    X = arma::abs(Y);
    X = (1.0 / input.size()) * (X % X); // Elementwise multiplication, ie. square
    return X;
  }

  arma::fvec hamming(unsigned int length) {
    // Defined as such for Hamming window
    const float alpha = 0.54;
    const float beta = 1.0 - alpha;

    arma::fvec a = arma::linspace<arma::fvec>(0.0, (float)length - 1, length);
    return alpha - beta * arma::cos((2 * arma::datum::pi * a) / (length - 1));
  }
}
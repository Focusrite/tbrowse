#include <iostream>
#include <cmath>
#include <string>

#include "armadillo"

#include "audioread.h"
#include "sigprocess.h"
#include "feature.h"
#include "log.h"
#include "settings.h"

#define IN_TEST TRUE

void passedText(bool passed) {
  if(passed)
    std::cout << "Passed\n";
  else
    std::cout << "Failed\n";
}

void testSigp() {
  float tolerance = 0.01;
  std::cout << "Test DCT 1 (zero): ";
  float ans = 0;
  arma::fvec test = arma::zeros<arma::fvec>(5);
  arma::fvec res = sigp::dct(&test);
  passedText(abs(arma::accu(res)) - ans < tolerance);

  std::cout << "Test DCT 2 (non-zero): ";
  ans = 2.7559;
  test(0) = 1; test(2) = 1; test(4) = 1;
  res = sigp::dct(&test);
  passedText(abs(arma::accu(res)) - ans < tolerance);

  std::cout << "Test Hamming: ";
  ans = 4.94;
  res = sigp::hamming(10);
  passedText(abs(arma::accu(res)) - ans < tolerance);

  std::cout << "Test periodogram: ";
  ans = 1.01;
  arma::fvec win = sigp::hamming(5);
  res = sigp::periodogram(&test, 10, &win);
  passedText(abs(arma::accu(res)) - ans < tolerance);
}

void testFeatureExtraction() {
  float tolerance = 0.01;
  std::cout << "Test empty: ";
  float ans = 0.0;
  AudioFeature empty = AudioFeature();
  empty.extractFromAudio("./test/resources/empty.wav");
  passedText(arma::accu(arma::abs(empty.getFeature())) - ans < tolerance);

  std::cout << "Test not empty: ";
  ans = 0.0;
  AudioFeature notempty = AudioFeature();
  notempty.extractFromAudio("./test/resources/notempty.wav");
  std::cout << "Got: " << arma::accu(arma::abs(notempty.getFeature())) << "\n";
  passedText(arma::accu(arma::abs(notempty.getFeature())) - ans < tolerance);
}

// Audio tests
void testAudioread() {
  std::cout << "Testing reading of file empty.wav: ";
  Audio a = Audio("./test/resources/empty.wav");
  arma::fvec data = a.getData(0, 99999);
  passedText(arma::accu(data % data) == 0);

  std::cout << "Testing reading of file notempty.wav: ";
  Audio b = Audio("./test/resources/notempty.wav");
  data = b.getData(0, 99999);
  passedText(arma::accu(data % data) != 0);

}


int main() {
  Settings::load();
  int status = Audio::init();
  AudioFeature::init(10);
  if (status != 0)
    std::cout << "Error intializing\n";

  Settings::load();
  std::cout << "\nSignal processing:\n";
  testSigp();
  std::cout << "\nAudio reading:\n";
  testAudioread();
  std::cout << "\nAudio processing:\n";
  testFeatureExtraction();
  AudioFeature::finalize();
  Audio::finalize();
  std::cout << "\n..done.\n";
}

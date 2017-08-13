#include <iostream>

#include "armadillo"
#include "audioread.h"
#include "sigprocess.h"
#include "log.h"

#define IN_TEST TRUE

void passedText(bool passed) {
  if(passed)
    std::cout << "Passed\n";
  else
    std::cout << "Failed\n";
}

void testSigp(Audio* a) {
  logInf("Calc fbank");
  arma::fmat fbank = sigp::melfilterbank(400, 20, 40, 16000, 44100, 512);
  logInf("Calc window");
  arma::fvec window = sigp::hamming(400);

  arma::fvec d = a->getData(0, 400);
  logInf("Calc mfcc");
  arma::fvec mfcc = sigp::mfcc(&d, 20, &fbank, 512, &window);
  mfcc.print();
}

// Audio tests
void testAudioread() {
  int status = Audio::init();
  if (status != 0)
    std::cout << "Error intializing\n";
  std::cout << "Testing reading of file empty.wav: ";
  Audio a = Audio("./test/resources/empty.wav");
  arma::fvec data = a.getData(0, 99999);
  passedText(arma::accu(data % data) == 0);

  std::cout << "Testing reading of file notempty.wav: ";
  Audio b = Audio("./test/resources/notempty.wav");
  data = b.getData(0, 99999);
  passedText(arma::accu(data % data) != 0);

  testSigp(&b);

  Audio::finalize();
}


int main() {
  testAudioread();
}

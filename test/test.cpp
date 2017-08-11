#include <iostream>

#include "armadillo"
#include "audioread.h"

void passedText(bool passed) {
  if(passed)
    std::cout << "Passed\n";
  else
    std::cout << "Failed\n";
}

// Audio tests
void testAudioread() {
  std::cout << "Testing reading of file empty.wav: ";
  Audio a = Audio("../test/resources/empty.wav");
  arma::fvec data = a.getData(0, 99999);
  passedText(arma::accu(data % data) == 0);

  std::cout << "Testing reading of file notempty.wav: ";
  a = Audio("../test/resources/notempty.wav");
  data = a.getData(0, 99999);
  passedText(arma::accu(data % data) != 0);
}

int main() {
  testAudioread();
}

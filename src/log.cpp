#include "log.h"
#include <iostream>

void logErr(const char* msg, int id) {
  if (id != 0)
    std::cout << "ERROR (" << id << "): " << msg << "\n";
  else
    std::cout << "ERROR: " << msg << "\n";
}

void logWrn(const char* msg, int id) {
  if (id != 0)
    std::cout << "WARN (" << id << "): " << msg << "\n";
  else
    std::cout << "WARN: " << msg << "\n";
}

void logInf(const char* msg, int id) {
  if (id != 0)
    std::cout << "INFO (" << id << "): " << msg << "\n";
  else
    std::cout << "INFO: " << msg << "\n";
}


#include <string>
#include <unordered_map>

#include "settings.h"

// TODO: Create this proper instead of this hackjob.

int Settings::getInteger(std::string key) {
  try {
    return Settings::intmap.at(key);
  }
  catch(int e) {
    return 0;
  }
}

bool Settings::getBool(std::string key) {
  try {
    return Settings::boolmap.at(key);
  }
  catch(int e) {
    return false;
  }
}

float Settings::getFloat(std::string key) {
  try {
    return Settings::floatmap.at(key);
  }
  catch(int e) {
    return 0.0;
  }
}

void Settings::load() {
  // Temporary, should load from file
  intmap.insert({"mfccPerFrame", 20});
  intmap.insert({"NFFT", 512});
  intmap.insert({"postProjectionSize", 1024});

  floatmap.insert({"maxSeconds", 2.0});
  floatmap.insert({"frameSize", 0.04});
  floatmap.insert({"frameStep", 0.01});
  floatmap.insert({"filterbankMinHz", 60});
  floatmap.insert({"filterbankMaxHz", 16000});
}

std::unordered_map<std::string, int> Settings::intmap;
std::unordered_map<std::string, bool> Settings::boolmap;
std::unordered_map<std::string, float> Settings::floatmap;


#include <string>
#include <unordered_map>

class Settings {
private:
  static std::unordered_map<std::string, int> intmap;
  static std::unordered_map<std::string, bool> boolmap;
  static std::unordered_map<std::string, float> floatmap;
public:
  static int getInteger(std::string key);
  static bool getBool(std::string key);
  static float getFloat(std::string key);

  static void load();
};

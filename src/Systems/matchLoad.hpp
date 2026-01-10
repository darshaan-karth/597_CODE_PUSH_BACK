#pragma once

#include "../Constants.hpp"
#include "pros/adi.hpp"

using namespace pros::adi;
using namespace Constants;

class matchLoad {
private:
  bool stateLoader = false;
  DigitalOut loader = DigitalOut(loader_p, stateLoader);
  

public:
  matchLoad() {}

  void toggleLoader() {
    stateLoader = !stateLoader;
    loader.set_value(stateLoader);
  }

  void toggleLoaderOn() {
    stateLoader = true;
    loader.set_value(stateLoader);
  }

  void toggleLoaderOff() {
    stateLoader = false;
    loader.set_value(stateLoader);
  }
};
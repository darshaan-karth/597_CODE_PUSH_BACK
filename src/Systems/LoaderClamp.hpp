#pragma once

#include "../Constants.hpp"
#include "pros/adi.hpp"


using namespace pros::adi;
using namespace Constants;

class LoaderClamp {
private:
  DigitalOut clampLock = DigitalOut(clamp_p, true);
  bool stateClamp = true;

public:
  LoaderClamp() {}

  void toggleClampLock() {
    stateClamp = !stateClamp;
    clampLock.set_value(stateClamp);
  }
};
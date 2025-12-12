#pragma once

#include "../Constants.hpp"
#include "pros/adi.hpp"


using namespace pros::adi;
using namespace Constants;

class DescoreMech {
private:
  DigitalOut descore = DigitalOut(descore_p, false);
  bool stateDescore = false;

public:
  DescoreMech() {}

  void toggleDescore() {
    stateDescore = !stateDescore;
    descore.set_value(stateDescore);
  }

  void toggleDescoreOn() {
    stateDescore = true;
    descore.set_value(stateDescore);
  }
  
  void toggleDescoreOff() {
    stateDescore = false;
    descore.set_value(stateDescore);
  }
};
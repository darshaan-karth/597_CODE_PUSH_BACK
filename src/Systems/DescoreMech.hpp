#pragma once

#include "main.h"
#include "pros/adi.hpp"

using namespace pros::adi;
using namespace Constants;

class DescoreMech {
 private:
  bool stateDescore = false;
  DigitalOut descore = DigitalOut(descore_p, stateDescore);

 public:
  DescoreMech() {}

  bool getStateDescore() { return stateDescore; }

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
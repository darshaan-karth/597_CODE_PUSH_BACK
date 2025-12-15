#pragma once

#include "../Constants.hpp"
#include "pros/adi.hpp"
#include "Intake.hpp"

using namespace pros::adi;
using namespace Constants;

class DescoreMech {
private:
bool stateDescore = false;
  DigitalOut descore = DigitalOut(descore_p, stateDescore);
  Intake intk = Intake();
  

public:
  DescoreMech() {}

  void toggleDescore() {
    stateDescore = !stateDescore;
    descore.set_value(stateDescore);
  }

  void toggleDescoreOn() {
    stateDescore = false;
    descore.set_value(stateDescore);
  }
  
  void toggleDescoreOff() {
    if (stateDescore == false) {
      intk.lowerGoal();
      delay(100);
      intk.stopIntakeMotors();
    }
    stateDescore = true;
    descore.set_value(stateDescore);
  }
};
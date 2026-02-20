#pragma once

#include "../Constants.hpp"
#include "DescoreMech.hpp"
#include "pros/motors.h"

using namespace Constants;
using namespace pros;

enum class IntakeOwner { AUTON, DRIVER };
enum class IntakeMode { STOP, STORE, LOWER, MIDDLE, TOP };

struct Intake {
  // Initalizing Intake Motors
  pros::v5::Motor bottomIntakeMotor_motor = pros::v5::Motor(bottomIntakeMotor);
  pros::v5::Motor middleScoreMotor_motor = pros::v5::Motor(middleScoreMotor);
  pros::v5::Motor topScoreMotor_motor = pros::v5::Motor(topScoreMotor);
  pros::Mutex mutex;

  DescoreMech descore = DescoreMech();
  IntakeOwner owner = IntakeOwner::DRIVER;
  IntakeMode mode = IntakeMode::STOP;
  bool ejecting = false;
  bool isIntaked = false;
  bool isMiddleActive = false;
  bool isTopActive = false;
  int volt[3] = {0, 0, 0}; // bottom, middle, top

  Intake() {
    bottomIntakeMotor_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
    middleScoreMotor_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
    topScoreMotor_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);

    bottomIntakeMotor_motor.tare_position();
    middleScoreMotor_motor.tare_position();
    topScoreMotor_motor.tare_position();
  }

  // Intake Management Functions
  inline void setOwner(IntakeOwner newOwner) {
    mutex.take();
    owner = newOwner;
    mode = IntakeMode::STOP;
    ejecting = false;
    mutex.give();
  }

  inline void stop() { mode = IntakeMode::STOP; }
  inline void store() { mode = IntakeMode::STORE; }
  inline void lower() { mode = IntakeMode::LOWER; }
  inline void middle() { mode = IntakeMode::MIDDLE; }
  inline void top() { mode = IntakeMode::TOP; }

  // Intake Actions Functions
  void applyStop() {
    volt[0] = 0;
    volt[1] = 0;
    volt[2] = 0;

    isMiddleActive = false;
    isTopActive = false;

    bottomIntakeMotor_motor.move(volt[0]);
    middleScoreMotor_motor.move(volt[1]);
    topScoreMotor_motor.move(volt[2]);
  }

  void applyStorage() {
    isMiddleActive = false;
    isTopActive = false;
    if (!descore.getStateDescore()) {
      descore.toggleDescoreOn();
      delay(75);
    }
    volt[0] = 227;
    volt[1] = -127;
    volt[2] = 127;

    bottomIntakeMotor_motor.move(volt[0]);
    middleScoreMotor_motor.move(volt[1]);
    topScoreMotor_motor.move(volt[2]);
    isIntaked = true;
  }

  void applyLowerGoal() {
    volt[0] = -127;
    volt[1] = 127;
    volt[2] = -127;

    isMiddleActive = false;
    isTopActive = false;

    bottomIntakeMotor_motor.move(volt[0]);
    middleScoreMotor_motor.move(volt[1]);
    topScoreMotor_motor.move(volt[2]);
  }

  void applyMiddleGoal() {
    isMiddleActive = true;
    isTopActive = false;
    if (isIntaked) {
      applyLowerGoal();
      delay(150);
      applyStop();
      isIntaked = false;
    }

    volt[0] = 127;
    volt[1] = -127;
    volt[2] = -127;

    topScoreMotor_motor.move(volt[2]);
    bottomIntakeMotor_motor.move(volt[0]);
    middleScoreMotor_motor.move(volt[1]);
  }

  void applyTopGoal() {
    isMiddleActive = false;
    isTopActive = true;
    if (descore.getStateDescore()) {
      applyLowerGoal();
      delay(150);
      applyStop();
      descore.toggleDescoreOff();
      delay(50);
    }

    volt[0] = 127;
    volt[1] = -127;
    volt[2] = 127;

    bottomIntakeMotor_motor.move(volt[0]);
    middleScoreMotor_motor.move(volt[1]);
    topScoreMotor_motor.move(volt[2]);
    isIntaked = true;
  }

  void topEject() {
    bool initDescoreState = descore.getStateDescore();
    if (initDescoreState) {
      descore.toggleDescoreOff();
      delay(75);
    }
    topScoreMotor_motor.move(127);
    delay(75);
    topScoreMotor_motor.move(volt[2]);
    if (initDescoreState) {
      descore.toggleDescoreOn();
    }
  }

  void middleEject() {
    topScoreMotor_motor.move(-127);
    delay(75);
    topScoreMotor_motor.move(volt[2]);
  }

  // Intake Contral Status Getters
  inline bool getMiddleActive() { return isMiddleActive; }

  inline bool getTopActive() { return isTopActive; }

  inline int32_t getMotorCurrent(int tier = 1) {
    // Returns the current draw from the middle intake motor in mA
    if (tier == 1)
      return middleScoreMotor_motor.get_current_draw();
    else if (tier == 2)
      return topScoreMotor_motor.get_current_draw();
    else
      return bottomIntakeMotor_motor.get_current_draw();
  }

  // Intake Main Control Task
  void task() {
    while (true) {
      mutex.take();

      if (ejecting && owner != IntakeOwner::DRIVER &&
          owner != IntakeOwner::AUTON) {
        if (getMiddleActive()) {
          topEject();
        } else if (getTopActive()) {
          middleEject();
        }
        ejecting = false;

      } else {
        switch (mode) {
        case IntakeMode::STOP:
          applyStop();
          break;
        case IntakeMode::STORE:
          applyStorage();
          break;
        case IntakeMode::LOWER:
          applyLowerGoal();
          break;
        case IntakeMode::MIDDLE:
          applyMiddleGoal();
          break;
        case IntakeMode::TOP:
          applyTopGoal();
          break;
        }
      }

      mutex.give();
      pros::delay(20);
    }
  }
};
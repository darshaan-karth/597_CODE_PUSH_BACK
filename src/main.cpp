#include "main.h"
#include "Autonomous/ChassisAuton.hpp"
#include "Constants.hpp"
#include "Systems/DriveTrain.hpp"
#include "Systems/Intake.hpp"
#include "Systems/DescoreMech.hpp"
#include "Systems/matchLoad.hpp"
#include "lemlib-tarball/api.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <cstdio>

using namespace Constants;
using namespace pros;

DriveTrain dt = DriveTrain();
Intake intk = Intake();
DescoreMech descore = DescoreMech();
matchLoad loader = matchLoad();
ChassisAuton auton = ChassisAuton();
Controller master(E_CONTROLLER_MASTER);
pros::Task* statusTask = nullptr;

ASSET(blueRight_txt);
lemlib_tarball::Decoder BLUE_RIGHT(blueRight_txt);

// ASSET(blueRightNotStrict_txt);
// lemlib_tarball::Decoder BLUE_RIGHT(blueRightNotStrict_txt);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
  pros::lcd::initialize();
  auton.initialize();
  dt.teleMove = [=] {
    dt.arcadeDrive(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y),
                   master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X));
  };

  statusTask = new pros::Task([] {
    while (true) {
      pros::lcd::print(0, "X: %f", auton.getPoseX());
      pros::lcd::print(1, "Y: %f", auton.getPoseY());
      pros::lcd::print(2, "H: %f", auton.getPoseHeading());
      pros::lcd::print(3, "Current: %ld mA", intk.getMotorCurrent());
      pros::lcd::print(4, "Middle Stalled: %s", intk.isMiddleMotorStalled() ? "TRUE" : "No");
      pros::lcd::print(5, "Bottom Stalled: %s", intk.isBottomMotorStalled() ? "TRUE" : "No");
      pros::delay(100);
    }
  });
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

// LEFT SIDE AUTONOMOUS
void leftSideAuton() {
  auton.resetCoordinateSystem();
  auton.moveTo(0, 13, 64);
  auton.turnTo(-45);
  descore.toggleDescoreOn();
  intk.storageIntake();
  auton.resetCoordinateSystem();
  delay(100);
  auton.moveTo(0, 24, 32);
  delay(1500);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -14.8, 127, false);
  intk.stopIntakeMotors();
  delay(100);
  auton.turnTo(-90);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 17);
  intk.middleGoal();
  delay(4000);
  intk.stopIntakeMotors();
  auton.resetCoordinateSystem();
}

void rightSideAuton() {
  auton.resetCoordinateSystem();
  auton.moveTo(0, 13, 64);
  auton.turnTo(45);
  descore.toggleDescoreOn();
  intk.storageIntake();
  auton.resetCoordinateSystem();
  delay(100);
  auton.moveTo(0, 24, 32);
  delay(1500);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -14.8, 127, false);
  intk.stopIntakeMotors();
  delay(100);
  auton.turnTo(-90);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 17);
  intk.lowerGoal();
  delay(4000);
  intk.stopIntakeMotors();
  auton.resetCoordinateSystem();
}

void runSkillsAuton() {
  //Lower Goal Scoring
  auton.resetCoordinateSystem();
  auton.moveTo(0, 13, 64);
  auton.turnTo(45);
  descore.toggleDescoreOn();
  intk.storageIntake();
  auton.resetCoordinateSystem();
  delay(100);
  auton.moveTo(0, 24, 32);
  delay(1500);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -14.8, 127, false);
  intk.stopIntakeMotors();
  delay(100);
  auton.turnTo(-90);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 17);
  intk.lowerGoal();
  delay(4000);
  intk.stopIntakeMotors();
  auton.resetCoordinateSystem();

  //Parking 
  auton.moveTo(0, -16.6, 127, false);
  auton.resetCoordinateSystem();
  auton.turnTo(45);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -40, 127, false);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 5);
  delay(100);
  auton.turnTo(-110);
  auton.resetCoordinateSystem();
  intk.lowerGoal();
  auton.moveTo(0, 70, 227);
  delay(4000);
  intk.stopIntakeMotors();
}

void autonomous() {
  if (isMatchAuton == true) {
    if (isRightSide) {
      rightSideAuton();
    } else {
      leftSideAuton();
    }
  } else if (isMatchAuton == false) {
    runSkillsAuton();
  }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  int intakeTime = 0, descoreTime = 0, loaderTime = 0;
  bool isIntaked = false;

  while (true) {
    // Calling DriveTrain System
    dt.teleMove();

    // Intake System to spin, spinFast, spinRev, or stop
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      descore.toggleDescoreOff();
      intk.topGoal();
      isIntaked = true;
    } 
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      descore.toggleDescoreOn();
      intk.storageIntake();
      isIntaked = true;
    } 
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      if (isIntaked){
        intk.lowerGoal();
        delay(100);
        intk.stopIntakeMotors();
        isIntaked = false;
      }
      intk.middleGoal();
    } 
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      descore.toggleDescoreOn();
      intk.lowerGoal();
    } 
    else {
      intk.stopIntakeMotors();
    }

    // Descore System 
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B) &&
        (millis() - descoreTime > 500)) {
      descore.toggleDescore();
      descoreTime = millis();
    };

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) &&
        (millis() - loaderTime > 500)) {
      loader.toggleLoader();
      loaderTime = millis();
    };

    delay(20); // Run for 20 ms then update
  }
}
#include "main.h"
#include "Autonomous/ChassisAuton.hpp"
#include "Constants.hpp"
#include "Systems/DriveTrain.hpp"
#include "Systems/Intake.hpp"
#include "Systems/LoaderClamp.hpp"
#include "lemlib-tarball/api.hpp"
#include "pros/misc.h"

using namespace Constants;
using namespace pros;

DriveTrain dt = DriveTrain();
Intake intk = Intake();
LoaderClamp loader = LoaderClamp();
ChassisAuton auton = ChassisAuton();
Controller master(E_CONTROLLER_MASTER);

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

  pros::Task statusTask([] {
    while (true) {
      pros::lcd::print(0, "X: %.2f", auton.getPoseX());
      pros::lcd::print(1, "Y: %.2f", auton.getPoseY());
      pros::lcd::print(2, "H: %.2f", auton.getPoseHeading());
      pros::delay(50);
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

// SIMPLE TEST AUTON
void runMatchAuton(int chgAngle) {
  auton.resetCoordinateSystem();
  intk.storageIntake();
  auton.follow(BLUE_RIGHT["INTAKE_PATH_1"], 2, 1000);
  delay(900);
  intk.stopIntakeMotors();

  auton.resetCoordinateSystem();
  auton.turnTo(-10);
  auton.moveTo(0, -4.7, 1000, false);
  delay(100);
  auton.swingTo(-170, DriveSide::LEFT, 800);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 3, 500);
  intk.lowerGoal();
  delay(900);
  intk.stopIntakeMotors();

  /*auton.resetCoordinateSystem();
  auton.moveTo(0, -20, 1000, false);
  auton.turnTo(-135);
  loader.toggleClampLock();*/
}

void runSkillsAuton() {}

void autonomous() {
  if (isMatchAuton == true) {
    int chgAngle = (isRightSide == true) ? 1 : -1;
    runMatchAuton(chgAngle);
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
  int intakeTime = 0, loaderTime = 0;

  while (true) {
    // Calling DriveTrain System
    dt.teleMove();

    // Intake System to spin, spinFast, spinRev, or stop
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      intk.topGoal();
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      intk.storageIntake();
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intk.middleGoal();
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intk.lowerGoal();
    } else {
      intk.stopIntakeMotors();
    }

    // LoaderClmap System for accessing Loaders
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) &&
        (millis() - loaderTime > 500)) {
      loader.toggleClampLock();
      loaderTime = millis();
    };

    delay(20); // Run for 20 ms then update
  }
}
#include "main.h"
#include "Autonomous/ChassisAuton.hpp"
#include "Constants.hpp"
#include "Systems/ColorSort.hpp"
#include "Systems/DescoreMech.hpp"
#include "Systems/DriveTrain.hpp"
#include "Systems/Intake.hpp"
#include "Systems/matchLoad.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <cstdio>

using namespace Constants;
using namespace pros;

DriveTrain dt = DriveTrain();
Intake intk = Intake();
ColorSort colorSort = ColorSort();
DescoreMech descore = DescoreMech();
matchLoad loader = matchLoad();
ChassisAuton auton = ChassisAuton();
Controller master(E_CONTROLLER_MASTER);
pros::Task *statusTask = nullptr;
pros::Task *visionTaskHandle = nullptr;
pros::Task *intakeTaskHandle = nullptr;

// ASSET(blueRight_txt);
// lemlib_tarball::Decoder BLUE_RIGHT(blueRight_txt);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void visionTask(void *) {
  while (true) {
    if (colorSort.isWrongColor() && !intk.ejecting) {
      intk.mutex.take();
      intk.ejecting = true;
      intk.mutex.give();
    }
    delay(10);
  }
}

void initialize() {
  visionTaskHandle = new pros::Task(visionTask);
  intakeTaskHandle = new pros::Task([] { intk.task(); });

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
      pros::lcd::print(3, "Bottom Current: %ld mA", intk.getMotorCurrent(0));
      pros::lcd::print(4, "Middle Current: %ld mA", intk.getMotorCurrent(1));
      pros::lcd::print(5, "Top Current: %ld mA", intk.getMotorCurrent(2));
      pros::lcd::print(7, "Current Top Block: %s",
                       colorSort.detect_color() == RED_SIG_id    ? "Red"
                       : colorSort.detect_color() == BLUE_SIG_id ? "Blue"
                                                                 : "None");
      pros::delay(20);
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
  int count = 0;
  auton.resetCoordinateSystem();
  auton.moveTo(0, 13, 64);
  auton.turnTo(-45);
  descore.toggleDescoreOn();
  intk.store();
  auton.resetCoordinateSystem();
  auton.moveTo(0, 27.5, 64);
  delay(2000);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -18.2, 127, false);
  intk.stop();
  delay(100);
  auton.turnTo(-90);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -18.3, 127, false);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 0.5);
  while (count < 10) {
    intk.lower();
    delay(175);
    intk.stop();
    intk.middle();
    delay(2000);
    intk.stop();
    count++;
  }
  intk.stop();
  auton.resetCoordinateSystem();
}

void rightSideAuton() {
  int count = 0;
  auton.resetCoordinateSystem();
  auton.moveTo(0, 13 * 1.22, 64 * 2);
  intk.store();
  auton.turnTo(45);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 27, 64);
  delay(1500);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -18.9, 127, false);
  intk.stop();
  auton.turnTo(-90);
  auton.resetCoordinateSystem();
  intk.lower();
  delay(50);
  auton.moveTo(0, 17);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -1.2, 127, false);
  while (count < 100) {
    intk.lower();
    delay(700);
    intk.stop();
    delay(25);
    intk.store();
    delay(25);
    intk.stop();
    count++;
  }
  intk.stop();
  auton.resetCoordinateSystem();

  // Top Goal Scoring
  auton.moveTo(0, -16.5, 127, false);
  auton.turnTo(106);
  auton.resetCoordinateSystem();
  intk.store();
  delay(100);
  auton.moveTo(0, 22, 64);
  delay(700);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -22, 64, false);
  auton.turnTo(74);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 35);
  auton.turnTo(45);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -24, 127, false);
  intk.top();
  delay(2000);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 15);
  auton.turnTo(-25);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -16.6, 127, false);
  auton.turnTo(25);
  descore.toggleDescoreOff();
  auton.resetCoordinateSystem();
  auton.moveTo(0, -12, 64, false);
}

void runSkillsAuton() {
  // Lower Goal Scoring
  int count = 0;
  auton.resetCoordinateSystem();
  auton.moveTo(0, 13 * 1.22, 64 * 2);
  intk.store();
  auton.turnTo(45);
  auton.resetCoordinateSystem();
  auton.moveTo(0, 27, 64);
  delay(1500);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -18.9, 127, false);
  intk.stop();
  auton.turnTo(-90);
  auton.resetCoordinateSystem();
  intk.lower();
  delay(50);
  auton.moveTo(0, 17);
  auton.resetCoordinateSystem();
  auton.moveTo(0, -1.2, 127, false);
  while (count < 100) {
    intk.lower();
    delay(700);
    intk.stop();
    delay(25);
    intk.store();
    delay(25);
    intk.stop();
    count++;
  }
  intk.stop();
  auton.resetCoordinateSystem();

  // Parking
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
  intk.lower();
  auton.moveTo(0, 70, 227);
  delay(4000);
  intk.stop();
}

void autonomous() {
  intk.setOwner(IntakeOwner::AUTON);

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
  intk.setOwner(IntakeOwner::DRIVER);
  int descoreTime = 0, loaderTime = 0;

  while (true) {
    // Calling DriveTrain System
    dt.teleMove();

    // Intake System to spin, spinFast, spinRev, or stop
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      intk.top();
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      intk.store();
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intk.middle();
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intk.lower();
    } else {
      intk.stop();
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
#pragma once

#include <cstdint>
#include <cmath>
#include "lemlib/api.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.h"

using namespace pros;

namespace Constants {

    // ======================
    // ==  Motor Ports     ==
    // ======================

    // DriveTrain Motor Ports
    constexpr int8_t fl_p = -13; // Front Left
    constexpr int8_t ml_p = 12;  // Middle Left
    constexpr int8_t bl_p = -11; // Back Left

    constexpr int8_t fr_p = 18;  // Front Right
    constexpr int8_t mr_p = -19; // Middle Right
    constexpr int8_t br_p = 20;  // Back Right

    // IMU Port
    constexpr int8_t imu_port = 1;

    //Loader Clamp Port
    constexpr int8_t clamp_p = 10;

    // Intake Motor Ports
    constexpr int8_t frontBottom_intake = 8;
    constexpr int8_t frontTop_intake = 9;
    constexpr int8_t back_intake = 10;

    // ======================
    // == PID Parameters   ==
    // ======================

    // Lateral PID
    constexpr float lateral_kP = 15.0;
    constexpr float lateral_kI = 0.0;
    constexpr float lateral_kD = 30.0;

    constexpr float lateral_windupRange = 4;
    constexpr float lateral_smallError = 3;
    constexpr float lateral_smallTime = 100;
    constexpr float lateral_largeError = 5;
    constexpr float lateral_largeTime = 300;
    constexpr float lateral_maxVoltage = 12000;

    // Angular PID
    constexpr float angular_kP = 3.0;
    constexpr float angular_kI = 0.0;
    constexpr float angular_kD = 20.0;

    constexpr float angular_windupRange = 2;
    constexpr float angular_smallError = 1;
    constexpr float angular_smallTime = 100;
    constexpr float angular_largeError = 3;
    constexpr float angular_largeTime = 300;
    constexpr float angular_maxVoltage = 12000;

    // ======================
    // == LemLib Objects   ==
    // ======================

    //Motor Groups for Autonomous
    inline pros::MotorGroup* leftMotors = new pros::MotorGroup({fl_p, ml_p, bl_p});
    inline pros::MotorGroup* rightMotors = new pros::MotorGroup({fr_p, mr_p, br_p});

    // PID Controller Settings
    inline lemlib::ControllerSettings lateralPID(
        lateral_kP, lateral_kI, lateral_kD,
        lateral_windupRange,
        lateral_smallError, lateral_smallTime,
        lateral_largeError, lateral_largeTime,
        lateral_maxVoltage
    );

    inline lemlib::ControllerSettings angularPID(
        angular_kP, angular_kI, angular_kD,
        angular_windupRange,
        angular_smallError, angular_smallTime,
        angular_largeError, angular_largeTime,
        angular_maxVoltage
    );

    // Drivetrain Settings
    inline lemlib::Drivetrain drivetrain(
        leftMotors,
        rightMotors,
        14, // Track width (inches)
        lemlib::Omniwheel::NEW_325,
        600, // Wheel travel (ticks per rotation)
        2    // Chase power (max curve aggressiveness)
    );

    // ======================
    // == Motion Profile   ==
    // ======================

    // Motion profiling constants
    constexpr double autonVelocity = 1.0;       // ft/s
    constexpr double autonAcceleration = 2.0;   // ft/s^2
    constexpr double autonJerk = 10.0;          // ft/s^3

    // ======================
    // == Drive Control    ==
    // ======================

    constexpr uint8_t threshold = 9; // Joystick deadzone (7–10%)

    // ======================
    // == Auton Config     ==
    // ======================

    constexpr bool isMatchAuton = true;
    constexpr bool isBlueAlliance = false;
    constexpr bool isRightSide = true;
}

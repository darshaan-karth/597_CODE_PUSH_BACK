#pragma once

#include "lemlib/api.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.h"
#include <cmath>
#include <cstdint>

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
constexpr int8_t imu_port = 7;
constexpr int8_t encoder_port = 5;

// Loader Clamp Port
constexpr unsigned char clamp_p = 'E'; // Port for the Pneumatic clamps

// Intake Motor Ports
constexpr int8_t frontBottom_intake = 8;
constexpr int8_t frontTop_intake = 9;
constexpr int8_t back_intake = 10;

// ======================
// == PID Parameters   ==
// ======================

/**
 * ===== PID TUNING GUIDE =====
 *
 * - Tune lateral (drive) and angular (turn) PID separately.
 * - Start with low kP, increase until you get good response.
 * - Add kD to reduce overshoot and oscillation.
 * - Use small kI only if steady-state error occurs.
 *
 * LATERAL PID (Forward/Backward Driving):
 * - lateral_kP: Increases response speed
 *     - Too high -> overshoot or oscillation
 * - lateral_kD: Dampens overshoot and oscillation
 *     - Too low -> robot bounces at target
 * - lateral_kI: Fixes consistent undershooting (try 0.01–0.05)
 *
 * ANGULAR PID (Turning In Place):
 * - angular_kP: Controls how fast it turns to target
 *     - Too high -> spins past target
 * - angular_kD: Prevents wobbles at final heading
 *     - Too low -> jittery at end
 * - angular_kI: Rarely needed; try only if it never reaches angle
 *
 * Testing:
 * 1. Test moveDistancesInches() for straight driving.
 * 2. Tune lateral PID for smooth stops without jitter.
 * 3. Test turnAngleDegrees() for turning accuracy.
 * 4. Tune angular PID for precise heading control.
 *
 * Tips:
 * - Increase smallError or smallTime to reduce jitter at target.
 * - Use delays after moves to observe robot behavior.
 * - Print sensor data for debugging.
 *
 * DEBUGGING Tips:
 * - Print pose or sensor values with pros::lcd::print()
 * - Increase smallError or smallTime if it jitters at target
 * - Add delay() after move/turn commands to watch behavior
 */

// Lateral PID
constexpr float lateral_kP = 10.8;
constexpr float lateral_kI = 0.0;
constexpr float lateral_kD = 3.5;

constexpr float lateral_windupRange = 0;
constexpr float lateral_smallError = 0;
constexpr float lateral_smallTime = 0;
constexpr float lateral_largeError = 0;
constexpr float lateral_largeTime = 0;
constexpr float lateral_maxVoltage = 12000;

// Angular PID
constexpr float angular_kP = 2;
constexpr float angular_kI = 0;
constexpr float angular_kD = 10.5; //10

constexpr float angular_windupRange = 0;
constexpr float angular_smallError = 0.2;
constexpr float angular_smallTime = 100;
constexpr float angular_largeError = 0;
constexpr float angular_largeTime = 0;
constexpr float angular_maxVoltage = 12000;

// ======================
// == LemLib Objects   ==
// ======================

// PID Controller Settings
inline lemlib::ControllerSettings
    lateralPID(lateral_kP, lateral_kI, lateral_kD, lateral_windupRange,
               lateral_smallError, lateral_smallTime, lateral_largeError,
               lateral_largeTime, lateral_maxVoltage);

inline lemlib::ControllerSettings
    angularPID(angular_kP, angular_kI, angular_kD, angular_windupRange,
               angular_smallError, angular_smallTime, angular_largeError,
               angular_largeTime, angular_maxVoltage);

pros::Rotation horizontal_encoder(encoder_port);
inline lemlib::TrackingWheel lateral_tracking(&horizontal_encoder,
                                              lemlib::Omniwheel::NEW_325, 0);

// ======================
// == Drive Control    ==
// ======================

constexpr uint8_t threshold = 9; // Joystick deadzone (7–10%)

// ======================
// == Auton Config     ==
// ======================

constexpr bool isMatchAuton = true;
constexpr bool isRightSide = true;
} // namespace Constants

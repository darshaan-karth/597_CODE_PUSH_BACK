#pragma once

#include "lemlib/chassis/chassis.hpp"
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
constexpr int8_t br_p = 17;  // Back Right

// Sensor Ports
constexpr int8_t imu_port = 5;
constexpr int8_t vertical_encoder_port = 16;
constexpr int8_t vision_p = 7;

// Descore Mechanism Port
constexpr unsigned char descore_p = 'E'; // Port for the Pneumatic descorer
constexpr unsigned char loader_p = 'C';  // Port for the Pneumatic loader

// Intake Motor Ports
constexpr int8_t bottomIntakeMotor = 2;
constexpr int8_t middleScoreMotor = 3;
constexpr int8_t topScoreMotor = 1;

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
constexpr float lateral_kP = 10.8; // 10.8;
constexpr float lateral_kI = 0.0;
constexpr float lateral_kD = 5;

constexpr float lateral_windupRange = 0;
constexpr float lateral_smallError = 0.2;
constexpr float lateral_smallTime = 100;
constexpr float lateral_largeError = 1;
constexpr float lateral_largeTime = 1000;
constexpr float lateral_maxVoltage = 12000;

// Angular PID
constexpr float angular_kP = 3.4;
constexpr float angular_kI = 0;
constexpr float angular_kD = 20; // 10.5

constexpr float angular_windupRange = 0;
constexpr float angular_smallError = 0.1;
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

inline pros::Rotation vertical_encoder(vertical_encoder_port);
inline lemlib::TrackingWheel vertical_tracking(&vertical_encoder,
                                               lemlib::Omniwheel::NEW_2, -1);

// ======================
// == Drive Control    ==
// ======================

constexpr uint8_t threshold = 9; // Joystick deadzone (7–10%)

// ======================
// == Color Sort Const ==
// ======================

// Setting up the signature IDs for blue and red colors
constexpr int RED_SIG_id = 1;
constexpr int BLUE_SIG_id = 2;
constexpr int object_threshold = 0;
constexpr int NUM_VISION_OBJECTS = 1;

// ======================
// == Auton Config     ==
// ======================

constexpr bool isMatchAuton = true;
constexpr bool isRightSide = true;
constexpr bool isBlue = true;
} // namespace Constants

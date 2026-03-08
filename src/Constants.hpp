#pragma once

#include <cmath>
#include <cstdint>

namespace Constants {

// ======================
// ==  Motor Ports     ==
// ======================

// DriveTrain Motor Ports
constexpr int8_t fl_p = -13;  // Front Left
constexpr int8_t ml_p = 12;   // Middle Left
constexpr int8_t bl_p = -11;  // Back Left

constexpr int8_t fr_p = 18;   // Front Right
constexpr int8_t mr_p = -19;  // Middle Right
constexpr int8_t br_p = 17;   // Back Right

// Sensor Ports
constexpr int8_t imu_port = 5;
constexpr int8_t vertical_encoder_port = 16;
constexpr int8_t vision_p = 7;

// Descore Mechanism Port
constexpr unsigned char descore_p = 'E';  // Port for the Pneumatic descorer
constexpr unsigned char loader_p = 'C';   // Port for the Pneumatic loader

// Intake Motor Ports
constexpr int8_t bottomIntakeMotor = 2;
constexpr int8_t middleScoreMotor = 3;
constexpr int8_t topScoreMotor = 1;

// ======================
// == Drive Control    ==
// ======================

constexpr uint8_t threshold = 9;  // Joystick deadzone (7–10%)

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

constexpr bool isBlue = true;
}  // namespace Constants

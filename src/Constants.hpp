#pragma once
#include <stdint.h>
#include <cmath>

namespace Constants {

    // DriveTrain Constants
    static const int8_t fl_p = -13; // Port of Front Left DriveTrain Motor
    static const int8_t ml_p = 12; // Port of Middle Left DriveTrain Motor
    static const int8_t bl_p = -11; // Port of Back Left DriveTrain Motor

    static const int8_t fr_p = 18; // Port of Front Right DriveTrain Motor
    static const int8_t mr_p = -19; // Port of Middle Right DriveTrain Motor
    static const int8_t  br_p = 20; // Port of Back Right DriveTrain Motor

    static const int8_t imu_port = 1; // Port of IMU for heading corrections

    // Intake Constants
    static const int8_t frontBottom_intake = 8; // Port of Front Bottom Intake Motor
    static const int8_t frontTop_intake = 9; // Port of Front Top Intake Motor
    static const int8_t back_intake = 10; // Port of Back Intake Motor

    // PID gains for motion control:
    //
    // Distance PID: Controls how accurately the robot moves to a target position.
    //   - Proportional (P): Reacts to how far the robot is from the target distance.
    //   - Integral (I): Helps eliminate steady-state error (usually left at 0).
    //   - Derivative (D): Dampens overshoot by reacting to rate of error change.
    //
    // Turn PID: Controls how accurately the robot turns to a target heading.
    //   - Proportional (P): Reacts to the angular error (difference in heading).
    //   - Integral (I): Helps with small consistent angular errors (often unused).
    //   - Derivative (D): Smooths the turn and reduces overshooting.
    //
    // Angle PID: Controls heading correction while the robot is driving (not turning in place).
    //   - Proportional (P): Corrects heading error while following a path.
    //   - Integral (I): Helps correct long-term drift during path following.
    //   - Derivative (D): Reduces oscillations in heading during motion.
    //
    // Tip:
    // - Increase P for faster response, but too high can overshoot.
    // - D helps stabilize motion.
    // - I is rarely needed unless you have drift or long-term error.

    //Distance PID
    static const double distance_kP = 0.001;
    static const double distance_kI = 0.0;
    static const double distance_kD = 0.0;
    
    //Turn PID
    static const double turn_kP = 0.003;
    static const double turn_kI = 0.0;
    static const double turn_kD = 0.0003;

    //Angle PID
    static const double angle_kP = 0.003;
    static const double angle_kI = 0.0;
    static const double angle_kD = 0.0002;

    //Chassis Autonomous - Motion Profile Constants
    // Set motion profile constraints:
    //   - Max velocity (ft/s): limits the top speed of the robot during path following.
    //   - Max acceleration (ft/s²): limits how quickly the robot can speed up or slow down.
    //   - Max jerk (ft/s³): limits the rate of change of acceleration to ensure smooth motion and reduce mechanical stress.
    //
    // Adjust these values to balance speed, precision, and smoothness of the robot's movements.
    // Lower values = smoother and more controlled motion but slower.
    // Higher values = faster but potentially less smooth and accurate.

    static const double autonVelocity = 1.0;
    static const double autonAcceleration = 2.0;
    static const double autonJerk = 10.0;

    //Restricting Variables
    static constexpr unsigned char threshold = 9; // Threshhold for controller to start moving the robot for DriveTrain (7%)

    // Autonomous Mode Types
    static const bool isMatchAuton = true;
    static const bool isBlueAlliance = false;
    static const bool isRightSide = true: 
};
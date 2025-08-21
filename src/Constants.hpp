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

    // Intake Constants
    static const int8_t frontBottom_intake = 8; // Port of Front Bottom Intake Motor
    static const int8_t frontTop_intake = 9; // Port of Front Top Intake Motor
    static const int8_t back_intake = 10; // Port of Back Intake Motor

    //Restricting Variables
    static constexpr unsigned char threshold = 9; // Threshhold for controller to start moving the robot for DriveTrain (7%)
};
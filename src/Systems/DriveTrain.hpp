#pragma once

#include "../Constants.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.h"

using namespace Constants;
using namespace pros;

struct DriveTrain {
    MotorGroup left_g = MotorGroup({fl_p, ml_p, bl_p}); //Abstracting the left side motors as a motor group
    MotorGroup right_g = MotorGroup({fr_p, mr_p, br_p}); //Abstracting the right side motors as a motor group

    std::function<void(void)> teleMove;

    DriveTrain() {
        left_g.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
        right_g.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
   
        left_g.tare_position();
        right_g.tare_position();
    }

    inline void tankDrive(signed char leftY, signed char rightY){
        left_g.move(abs(leftY)<threshold ? 0 :leftY);
        right_g.move(abs(rightY)<threshold ? 0 :rightY);
    }

    inline void arcadeDrive(signed char leftY, signed char rightX) {
        leftY = abs(leftY)<threshold ? 0 : leftY;
        rightX = abs(rightX)<threshold ? 0 : rightX;
        left_g.move(leftY + rightX);
        right_g.move(leftY - rightX);
    }
};
#pragma once

#include "../Constants.hpp"
#include "pros/motors.h"

using namespace Constants;
using namespace pros;

struct Intake {
    // Initalizing Intake Motors
    Motor frontBottom_motor = Motor(frontBottom_intake);
    Motor frontTop_motor = Motor(frontTop_intake);
    Motor back_motor = Motor(back_intake);

    Intake() {
        frontBottom_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
        frontTop_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
        back_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
   
        frontBottom_motor.tare_position();
        frontTop_motor.tare_position();
        back_motor.tare_position();
    }

    inline void stopIntakeMotors(){
        frontBottom_motor.move(0);
        back_motor.move(0);
        frontTop_motor.move(0);
    }

    inline void storageIntake(){
        frontBottom_motor.move(127);
        back_motor.move(127);
    }

    inline void lowerGoal(){
        frontBottom_motor.move(-127);
        back_motor.move(-127);
    }

    inline void middleGoal(){
        frontBottom_motor.move(127);
        back_motor.move(-127);
        frontTop_motor.move(-127);
    }

    inline void topGoal(){
        frontBottom_motor.move(127);
        back_motor.move(-127);
        frontTop_motor.move(127);
    }
};
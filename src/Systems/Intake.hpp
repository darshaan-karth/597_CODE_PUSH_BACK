#pragma once

#include "../Constants.hpp"
#include "pros/motors.h"

using namespace Constants;
using namespace pros;

struct Intake {
    // Initalizing Intake Motors
    Motor bottomIntakeMotor_motor = Motor(bottomIntakeMotor);
    Motor middleScoreMotor_motor = Motor(middleScoreMotor);
    Motor topScoreMotor_motor = Motor(topScoreMotor);

    Intake() {
        bottomIntakeMotor_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
        middleScoreMotor_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
        topScoreMotor_motor.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
   
        bottomIntakeMotor_motor.tare_position();
        middleScoreMotor_motor.tare_position();
        topScoreMotor_motor.tare_position();
    }

    inline void stopIntakeMotors(){
        bottomIntakeMotor_motor.move(0);
        topScoreMotor_motor.move(0);
        middleScoreMotor_motor.move(0);
    }

    inline void storageIntake(){
        bottomIntakeMotor_motor.move(227);
        middleScoreMotor_motor.move(-127);
        topScoreMotor_motor.move(64);
    }

    inline void lowerGoal(){
        bottomIntakeMotor_motor.move(-127);
        middleScoreMotor_motor.move(127);
        topScoreMotor_motor.move(-127);
    }

    inline void middleGoal(){
        bottomIntakeMotor_motor.move(127);
        middleScoreMotor_motor.move(-127);
        topScoreMotor_motor.move(-64);
    }

    inline void topGoal(){
        bottomIntakeMotor_motor.move(127);
        middleScoreMotor_motor.move(-127);
        topScoreMotor_motor.move(127);
    }
};
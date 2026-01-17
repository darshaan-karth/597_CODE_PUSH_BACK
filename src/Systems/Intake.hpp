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

    inline int32_t getMotorCurrent(){
        // Returns the current draw from the middle intake motor in mA
        return bottomIntakeMotor_motor.get_current_draw();
    }

    /*inline bool isMiddleMotorStalled(int32_t currentThreshold = 2400){
        // Detects if middle intake motor is drawing high current (stalled/blocked)
        // Returns true if motor is drawing significant current
        int32_t middleCurrent = middleScoreMotor_motor.get_current_draw();
        
        return (abs(middleCurrent) > currentThreshold);
    }

    inline bool isBottomMotorStalled(int32_t currentThreshold = 2900){
        // Detects if bottom intake motor is drawing high current (stalled/blocked)
        // Returns true if motor is drawing significant current
        int32_t bottomCurrent = bottomIntakeMotor_motor.get_current_draw();

        return (abs(bottomCurrent) > currentThreshold);
    }

    inline void lowerGoal(){
        if (isMiddleMotorStalled() || isBottomMotorStalled()){
            storageIntake();
            delay(100);
            stopIntakeMotors();
        }
        bottomIntakeMotor_motor.move(-127);
        middleScoreMotor_motor.move(127);
        topScoreMotor_motor.move(-127);
    }*/

    inline void middleGoal(){
        topScoreMotor_motor.move(-64);
        delay(100);
        bottomIntakeMotor_motor.move(127);
        middleScoreMotor_motor.move(-127);
        
    }

    inline void topGoal(){
        bottomIntakeMotor_motor.move(127);
        middleScoreMotor_motor.move(-127);
        topScoreMotor_motor.move(127);
    }
};
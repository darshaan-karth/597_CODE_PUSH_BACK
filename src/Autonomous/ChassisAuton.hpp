#pragma once

#include "../Systems/DriveTrain.hpp"
#include "../Constants.hpp"
#include "okapi/api.hpp"

using namespace okapi;
using namespace Constants;
using namespace pros;

class ChassisAuton {
    private:
        DriveTrain &drive;

        std::shared_ptr<ChassisController> chassis;
        std::shared_ptr<AsyncMotionProfileController> profile;
        IMU imu;
    
    public:
        ChassisAuton(DriveTrain &d): drive(d), imu(imu_port) {
            // Wait for IMU calibration
            while (imu.is_calibrating()) {
                pros::delay(10); // Wait in small intervals to avoid CPU overload
            }

            // Create motion control system
            chassis = ChassisControllerBuilder()
                .withMotors({fl_p, ml_p, bl_p},                         // Left Motor Group
                            {fr_p, mr_p, br_p})                         // Right Motor Group
                .withGains({distance_kP, distance_kI, distance_kD},     // Distance PID - [kP, kI, kD]
                           {turn_kP, turn_kI, turn_kD},                 // Turn PID - [kP, kI, kD]
                           {angle_kP, angle_kI, angle_kD})              // Angle PID - [kP, kI, kD]
                .withDimensions(AbstractMotor::gearset::blue, {{3.25_in, 12_in}, imev5BlueTPR})
                .withIMU(imu)
                .build();
            
            profile = AsyncMotionProfileControllerBuilder()
                .withLimits({autonVelocity, autonAcceleration, autonJerk}) // velocity, acceleration, and jerk
                .withOutput(chassis)
                .build();
        }
    
        void reset() {
            imu.reset();
            chassis -> resetSensors();
            chassis -> setState({0_in, 0_in, 0_deg});
        }
        
        // Move forward or backward dynamically (positive = forward, negative = backward)
        void moveDistancesInches(double inches) {
            //Reset the Chassis 
            reset();

            profile -> generatePath({
                {0_in, 0_in, 0_deg},
                {inches * inch, 0_in, 0_deg}
            }, "dynamic_move");

            profile -> setTarget("dynamic_move");

            while (!profile -> isSettled()) {
                pros::delay(10);
            }
        }
        
        // Turn in place dynamically (positive = clockwise degrees, negative = counterclockwise)
        void turnAngleDegrees(double degrees) {
            //Reset the Chassis 
            reset();

            profile -> generatePath({
                {0_in, 0_in, 0_deg},
                {0_in, 0_in, degrees * degree}
            }, "dynamic_turn");

            profile -> setTarget("dynamic_turn");

            while (!profile -> isSettled()) {
                pros::delay(10);
            }
        }

        // Example of a curved path with control points - you can expand this
        void moveCurvedPath(std::vector<std::tuple<QLength, QLength, QAngle>> waypoints) {
            //Reset the Chassis 
            reset();

            profile->generatePath(waypoints, "dynamic_curve");
            profile->setTarget("dynamic_curve");

            while (!profile->isSettled()) {
                pros::delay(10);
            }
        }
};

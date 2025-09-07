#pragma once

#include "../Constants.hpp"
#include "lemlib/api.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/imu.hpp"

using namespace Constants;
using namespace pros;

class ChassisAuton {
private:
    pros::Imu imu;

    pros::MotorGroup leftMotors;
    pros::MotorGroup rightMotors;

    lemlib::Drivetrain drivetrain;
    lemlib::OdomSensors odomSensors;
    lemlib::Chassis chassis;

public:
    ChassisAuton()
        : imu(imu_port),
          leftMotors({fl_p, ml_p, bl_p}),
          rightMotors({fr_p, mr_p, br_p}),
          drivetrain(
              &leftMotors,
              &rightMotors,
              11.75, // track width
              lemlib::Omniwheel::NEW_325,
              600, // wheel travel
              2    // chase power
          ),
          odomSensors(nullptr, nullptr, nullptr, nullptr, &imu),
          chassis(
              drivetrain,
              lateralPID,
              angularPID,
              odomSensors,
              nullptr,
              nullptr
          )
    {}

    void initialize() {
        imu.reset();
        while (imu.is_calibrating()) pros::delay(10);
        chassis.calibrate();
        chassis.setPose(0, 0, 0); // Start pose (0,0), facing forward (0°)
    }

    void resetCoordinateSystem() {
        chassis.setPose(0, 0, 0);
    }

    // Moves robot to an absolute field position (X, Y in inches)
    void moveTo(double x, double y) {
        chassis.moveToPoint(x, y, 3000); // 3 second timeout
        chassis.waitUntilDone();
        delay(100);
    }

    // Turns robot to face a specific heading (0–360 degrees)
    void turnToHeading(double heading) {
        auto pose = chassis.getPose();
        chassis.moveToPose(pose.x, pose.y, heading, 3000);
        //chassis.turnToPoint(pose.x, pose.y, heading);
        chassis.waitUntilDone();
        delay(100);
    }

    // Optional: Curved path between waypoints (absolute)
    void moveCurvedPath(const std::vector<lemlib::Pose>& waypoints) {
        for (const auto& point : waypoints) {
            chassis.moveToPoint(point.x, point.y, 3000);
            chassis.waitUntilDone();
        }
    }
};

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
    lemlib::OdomSensors odomSensors;
    lemlib::Chassis chassis;

public:
    ChassisAuton()
        : imu(imu_port),
          odomSensors(nullptr, nullptr, nullptr, nullptr, &imu),
          chassis(
              drivetrain,      // not a pointer anymore
              lateralPID,
              angularPID,
              odomSensors,
              nullptr,
              nullptr
          )
    {
        imu.reset();
        while (imu.is_calibrating()) pros::delay(10);
        chassis.calibrate();
    }

    void reset() {
        imu.reset();
        while (imu.is_calibrating()) pros::delay(10);
        chassis.setPose(0, 0, 0);
    }

    void moveDistancesInches(double inches) {
        reset();
        chassis.moveToPoint(inches, 0, 3000);  // x, y, timeout
        chassis.waitUntilDone();              // replaces .settled()
    }

    void turnAngleDegrees(double degrees) {
        reset();
        auto pose = chassis.getPose();
        chassis.turnToPoint(pose.x, pose.y, degrees);  // x, y, target heading
        chassis.waitUntilDone();
    }

    void moveCurvedPath(const std::vector<lemlib::Pose>& waypoints) {
        reset();
        for (const auto& point : waypoints) {
            chassis.moveToPoint(point.x, point.y, 3000);  // ignores theta
            chassis.waitUntilDone();
        }
    }
};

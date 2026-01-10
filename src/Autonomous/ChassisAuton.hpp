#pragma once

#include "../Constants.hpp"
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
      : imu(imu_port), leftMotors({fl_p, ml_p, bl_p}),
        rightMotors({fr_p, mr_p, br_p}), drivetrain(&leftMotors, &rightMotors,
                                                    11.75, // track width
                                                    lemlib::Omniwheel::NEW_325,
                                                    600, // wheel travel
                                                    1   // higher  values make the robot move faster but causes more overshoot on turns
                                                    ),
        odomSensors(nullptr, nullptr, &horizontal_tracking, nullptr, &imu),
        chassis(drivetrain, lateralPID, angularPID, odomSensors, nullptr,
                nullptr) {}

  void initialize() {
    imu.reset();
    while (imu.is_calibrating())
      pros::delay(10);
    chassis.calibrate();
    chassis.setPose(0, 0, 0); // Start pose (0,0), facing forward (0°)
  }

  double getPoseX() { return chassis.getPose().x; }
  double getPoseY() { return chassis.getPose().y; }
  double getPoseHeading() { return chassis.getPose().theta; }

  void resetCoordinateSystem() { chassis.setPose(0, 0, 0); }

  void follow(const asset &path, double lookahead = 10, int timeout = 3000) {
    chassis.follow(path, lookahead, timeout, true, false);
  }

  // Moves robot to an absolute field position (X, Y in inches)
  void moveTo(double x, double y, float speed = 127, bool isForward = true) {
    chassis.moveToPoint(x, y, std::abs(y) * (420.0 / 24.0) * (127 / speed),
                        {.forwards = isForward, .maxSpeed = speed}); // timeout is adjusted based on distance and speed to ensure it completes in time
    chassis.waitUntilDone();
  }

  // Turns robot to face a specific heading (0–360 degrees)
  void turnTo(double heading) {
    chassis.turnToHeading(heading, 1000);
    chassis.waitUntilDone();
  }

  void swingTo(double heading, DriveSide side, int timeout = 1000) {
    chassis.swingToHeading(heading, side, timeout, {.maxSpeed = 60});
    chassis.waitUntilDone();
  }

  // Optional: Curved path between waypoints (absolute)
  void moveCurvedPath(const std::vector<lemlib::Pose> &waypoints,
                      float speed = 127) {
    for (const auto &point : waypoints) {
      chassis.moveToPoint(point.x, point.y, point.y * (400 / 24),
                          {.maxSpeed = speed});
      chassis.waitUntilDone();
    }
  }
};

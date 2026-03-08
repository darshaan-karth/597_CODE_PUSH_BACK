#pragma once

#include "main.h"
#include "pros/vision.hpp"

using namespace Constants;
using namespace pros;

struct ColorSort {
  pros::Vision vision_sensor = pros::Vision(vision_p);
  pros::vision_object_s_t object_arr[NUM_VISION_OBJECTS];

  vision_signature_s_t RED_SIG = Vision::signature_from_utility(
      RED_SIG_id, 10185, 13107, 11651, -2493, -1065, -1779, 2.5, 0);
  vision_signature_s_t BLUE_SIG = Vision::signature_from_utility(
      BLUE_SIG_id, -4965, -3755, -4360, 6769, 9057, 7913, 2.5, 0);

  vision_color_code_t COLOR_CODE =
      vision_sensor.create_color_code(RED_SIG_id, BLUE_SIG_id);

  ColorSort() {
    vision_sensor.set_signature(RED_SIG_id, &RED_SIG);
    vision_sensor.set_signature(BLUE_SIG_id, &BLUE_SIG);
    vision_sensor.clear_led();
  }

  int detect_color() {
    int detect_sig = 0;
    if (vision_sensor.get_object_count() > object_threshold) {
      vision_sensor.read_by_code(0, COLOR_CODE, NUM_VISION_OBJECTS, object_arr);
      // if (object_arr[0].left_coord >= 75){
      detect_sig = object_arr[0].signature;
      //}
    }
    return (detect_sig);
  }

  bool isWrongColor() {
    int detected_color = detect_color();
    if (isBlue) {
      detected_color = (detected_color == 0) ? BLUE_SIG_id : detected_color;
      return (detected_color == RED_SIG_id);
    } else {
      detected_color = (detected_color == 0) ? RED_SIG_id : detected_color;
      return (detected_color == BLUE_SIG_id);
    }
  }
};

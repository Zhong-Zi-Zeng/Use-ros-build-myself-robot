//
// Created by ximen on 2023/1/2.
//

#ifndef ARDUINOMEGA_PID_H
#define ARDUINOMEGA_PID_H

#include "Parameter.h"
#include "math.h"


class PID {
  public:
    // 期望數值
    float desired = 0;

    // 積分所累計的誤差
    float Ki_error = 0;

    // 上一次的誤差
    float last_error = 0;

    // 上一次的輸出
    float last_output = 0;

    // 紀錄時間
    unsigned long now_time = millis();
    unsigned long last_time = now_time;

    bool restrain_flag = false;

    int run_vel(float desired_vel, float now_vel) {
      desired = desired_vel;
      now_time = millis();
      float dt = now_time - last_time;

      if (dt >= _SAMPLE_RATE) {
        float error = desired - now_vel;
        float delta_error = error - last_error;

        float Kp_error = _Kp * error;
        float Kd_error = delta_error / dt;
        Ki_error += error * dt;

        float vel = Kp_error + _Ki * Ki_error + _Kd * Kd_error;

        if (_OUTPUT_RAMP > 0) {
          float output_rate = (vel - last_output) / dt;
          if (output_rate > _OUTPUT_RAMP)
            vel = last_output + _OUTPUT_RAMP * dt;
          else if (output_rate < -_OUTPUT_RAMP)
            vel = last_output - _OUTPUT_RAMP * dt;
        }

        last_time = now_time;
        last_error = error;
        last_output = vel;

        return (int)((vel - 5.41) / 0.04645805);
      }
    }
};


#endif //ARDUINOMEGA_PID_H
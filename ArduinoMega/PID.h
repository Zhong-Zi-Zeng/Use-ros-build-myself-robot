//
// Created by ximen on 2023/1/2.
//

#ifndef ARDUINOMEGA_PID_H
#define ARDUINOMEGA_PID_H

#include "Parameter.h"
#include "math.h"


class PID{
public:
    // 期望數值
    float desired = 0;

    // 積分所累計的誤差
    float Ki_error = 0;

    // 上一次的誤差
    float last_error = 0;

    // 紀錄時間
    unsigned long now_time = millis();
    unsigned long last_time = now_time;

    int run_vel(float desired_vel, float now_vel){

        desired = desired_vel;
        now_time = millis();

        float dt = now_time - last_time;

        if (dt >= _SAMPLE_RATE) {
            float error = desired - now_vel;
            float delta_error = error - last_error;

            float Kp_error = _Kp * error;
            Ki_error += error * dt;
            float Kd_error = delta_error / dt;

            last_time = now_time;
            last_error = error;

            float vel = Kp_error + _Ki * Ki_error + _Kd * Kd_error;

            return (int)((vel-6.72766) / 0.03372969);
        }

    }


};


#endif //ARDUINOMEGA_PID_H

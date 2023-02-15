//
// Created by ximen on 2022/12/25.
//

#include "Controller.h"
#include "Arduino.h"
#include "Parameter.h"
#include "math.h"
#include "PID.h"


Controller::Controller(int lf_fg_pin, int lb_fg_pin, int rf_fg_pin, int rb_fg_pin) {
    this->lf_fg_pin = lf_fg_pin;
    this->lb_fg_pin = lb_fg_pin;
    this->rf_fg_pin = rf_fg_pin;
    this->rb_fg_pin = rb_fg_pin;

    for (int i = 0; i < 4; i++)
        pid[i] = PID();
}


void Controller::lf_ISR() {
    round_count[0]++;

    if (direction[0] == '+')
        degree_count[0]++;
    else
        degree_count[0]--;

    if (degree_count[0] % _PPR == 0)
        degree_count[0] = 0;
}

void Controller::lb_ISR() {
    round_count[1]++;

    if (direction[1] == '+')
        degree_count[1]++;
    else
        degree_count[1]--;

    if (degree_count[1] % _PPR == 0)
        degree_count[1] = 0;
}

void Controller::rf_ISR() {
    round_count[2]++;

    if (direction[2] == '+')
        degree_count[2]++;
    else
        degree_count[2]--;

    if (degree_count[2] % _PPR == 0)
        degree_count[2] = 0;
}

void Controller::rb_ISR() {
    round_count[3]++;

    if (direction[3] == '+')
        degree_count[3]++;
    else
        degree_count[3]--;

    if (degree_count[3] % _PPR == 0)
        degree_count[3] = 0;
}

void Controller::setup() {
    pinMode(lf_fg_pin, INPUT);
    pinMode(lb_fg_pin, INPUT);
    pinMode(rf_fg_pin, INPUT);
    pinMode(rb_fg_pin, INPUT);

    attachInterrupt(digitalPinToInterrupt(lf_fg_pin), lf_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(lb_fg_pin), lb_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(rf_fg_pin), rf_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(rb_fg_pin), rb_ISR, RISING);
}


void Controller::vel_command(int motor_id, float desired_vel, char *output) {
    int pwm;

    switch (motor_id) {
        case 0:
            pwm = pid[0].run_vel(desired_vel, now_vel[0]);
            break;
        case 1:
            pwm = pid[1].run_vel(desired_vel, now_vel[1]);
            break;
        case 2:
            pwm = pid[2].run_vel(desired_vel, now_vel[2]);
            break;
        case 3:
            pwm = pid[3].run_vel(desired_vel, now_vel[3]);
            break;
    }

    // 排除太小的pwm數值
    if (pwm < _MIN_PWM) {
        pwm = 0;
    }

    // 排除太大的pwm數值
    if (pwm > _MAX_PWM) {
        pwm = _MAX_PWM;
    }

    // 取出百位、十位、個位數
    output[2] = (char) (pwm % 10 + 48);
    output[1] = (char) ((pwm / 10) % 10 + 48);
    output[0] = (char) ((pwm / 100) % 10 + 48);
}


void Controller::read(char *direction_) {
    now_time = millis();

//    strcpy(direction, direction_);
    direction = direction_;

    // 更新速度
    if ((now_time - last_time) >= _INTERVAL) {
        last_time = now_time;

        for (int i = 0; i < 4; i++) {
            now_vel[i] = (round_count[i] / _PPR) * (1000 / _INTERVAL) * 2 * M_PI;
            round_count[i] = 0;
        }
    }

    // 更新角度
    for (int i = 0; i < 4; i++)
        now_degree[i] = (float) degree_count[i] * (360.0 / _PPR);


}

void Controller::get_vel(float *arr) {
    for (int i = 0; i < 4; i++)
        arr[i] = now_vel[i];
}

void Controller::get_degree(float *arr) {
    for (int i = 0; i < 4; i++)
        arr[i] = now_degree[i];
}

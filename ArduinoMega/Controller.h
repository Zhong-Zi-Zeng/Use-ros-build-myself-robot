//
// Created by ximen on 2022/12/25.
//

#ifndef ARDUINOMEGA_CONTROLLER_H
#define ARDUINOMEGA_CONTROLLER_H

#include "Arduino.h"
#include "PID.h"

static float round_count[4];  // 紀錄脈波數量 (for 速度)
static int degree_count[4];  // 紀錄脈波數量 (for 角度)

// 正反轉
static char *direction;

class Controller {
public:
    //馬達回授腳位
    int lf_fg_pin;
    int lb_fg_pin;
    int rf_fg_pin;
    int rb_fg_pin;

    // 紀錄速度 (m/s)
    float now_vel[4];

    // 紀錄角度 (度)
    float now_degree[4];

    // 紀錄時間
    unsigned long last_time, now_time;

    // PID控制器
    PID pid[4];

    // 中斷服務函式
    static void lf_ISR();

    static void lb_ISR();

    static void rf_ISR();

    static void rb_ISR();

    Controller(int lf_fg_pin, int lb_fg_pin, int rf_fg_pin, int rb_fg_pin);

    void setup(); // 設定pin腳與中斷函式

    void vel_command(int motor_id, float desired_vel, char *output); // 速度控制

    void read(char *direction_); // 檢測回授訊號

    void get_vel(float *arr); // 取得目前馬達速度

    void get_degree(float *arr); // 取得目前馬達角度
};

#endif

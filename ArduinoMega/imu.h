//
// Created by ximen on 2022/12/26.
//

#ifndef ARDUINOMEGA_IMU_H
#define ARDUINOMEGA_IMU_H

#include "Arduino.h"

volatile static unsigned char Re_buf[11], counter = 0;
volatile static unsigned char sign = 0;
volatile static float a[3], w[3], angle[3], T;

class Imu {
  public:
    Imu();

    void setup(); // 初始化serial

    static void detect();  // 檢測imu

    void get_imu_degree(float *imu_data); // 取得目前imu角度
    void get_imu_acc(float *imu_data); // 取得目前imu加速度
    void get_imu_wacc(float *imu_data); // 取得目前imu角加速度
};


#endif //ARDUINOMEGA_IMU_H

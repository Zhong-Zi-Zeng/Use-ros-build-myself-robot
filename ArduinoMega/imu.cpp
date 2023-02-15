
//
// Created by ximen on 2022/12/26.
//

#include "imu.h"
#include "Arduino.h"
#include <TimerFour.h>

Imu::Imu() {

}

void Imu::setup() {
  Timer4.initialize(100);
  Timer4.attachInterrupt(Imu::detect);
  Serial3.begin(115200); // 與JY61溝通
}

void Imu::detect() {

  if (sign) {
    sign = 0;
    if (Re_buf[0] == 0x55)      //检查帧头
    {
      switch (Re_buf[1]) {
        case 0x51:
          a[0] = (short(Re_buf[3] << 8 | Re_buf[2])) / 32768.0 * 16;
          a[1] = (short(Re_buf[5] << 8 | Re_buf[4])) / 32768.0 * 16;
          a[2] = (short(Re_buf[7] << 8 | Re_buf[6])) / 32768.0 * 16;
          T = (short(Re_buf[9] << 8 | Re_buf[8])) / 340.0 + 36.25;
          break;
        case 0x52:
          w[0] = (short(Re_buf[3] << 8 | Re_buf[2])) / 32768.0 * 2000;
          w[1] = (short(Re_buf[5] << 8 | Re_buf[4])) / 32768.0 * 2000;
          w[2] = (short(Re_buf[7] << 8 | Re_buf[6])) / 32768.0 * 2000;
          T = (short(Re_buf[9] << 8 | Re_buf[8])) / 340.0 + 36.25;
          break;
        case 0x53:
          angle[0] = (short(Re_buf[3] << 8 | Re_buf[2])) / 32768.0 * 180;
          angle[1] = (short(Re_buf[5] << 8 | Re_buf[4])) / 32768.0 * 180;
          angle[2] = (short(Re_buf[7] << 8 | Re_buf[6])) / 32768.0 * 180;
          T = (short(Re_buf[9] << 8 | Re_buf[8])) / 340.0 + 36.25;


          break;
      }
    }
  }
  while (Serial3.available()) {
    Re_buf[counter] = (unsigned char) Serial3.read();
    if (counter == 0 && Re_buf[0] != 0x55) return;      //第0号数据不是帧头
    counter++;
    if (counter == 11)             //接收到11个数据
    {
      counter = 0;               //重新赋值，准备下一帧数据的接收
      sign = 1;
    }
  }
}



void Imu::get_imu_degree(float *imu_data) {
  imu_data[0] = angle[0];
  imu_data[1] = angle[1];
  imu_data[2] = angle[2];

}

void Imu::get_imu_acc(float *imu_data) {
  imu_data[0] = a[0];
  imu_data[1] = a[1];
  imu_data[2] = a[2];
}

void Imu::get_imu_wacc(float *imu_data) {
  imu_data[0] = w[0];
  imu_data[1] = w[1];
  imu_data[2] = w[2];
}

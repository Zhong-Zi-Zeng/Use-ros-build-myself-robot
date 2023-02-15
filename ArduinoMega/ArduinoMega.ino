#include "Controller.h"
#include "imu.h"
#include <TimerThree.h>

Controller controller(18, 19, 20, 21); // 回授信號腳位(lf, lb, rf, rb)
Imu imu;

void serialFlush();

float motor_vel[4] = {0, 0, 0, 0}; // 紀錄馬達速度
float motor_degree[4] = {0, 0, 0, 0}; // 紀錄馬達角度
float imu_degree[3] = {0, 0, 0}; // 紀錄imu角度
float desired_vel[4] = {0., 0., 0., 0.}; // 馬達期望速度
char direction_[5] = "++++"; // 馬達正逆轉

char lf_pwm[4] = "000";
char lb_pwm[4] = "000";
char rf_pwm[4] = "000";
char rb_pwm[4] = "000";

//d5.0,5.0,5.0,5.0

void controlMotor() {
    char order[20];

    // 控制馬達
    controller.vel_command(0, desired_vel[0], lf_pwm);
    controller.vel_command(1, desired_vel[1], lb_pwm);
    controller.vel_command(2, desired_vel[2], rf_pwm);
    controller.vel_command(3, desired_vel[3], rb_pwm);

    sprintf(order, "<%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", direction_[0], lf_pwm[0], lf_pwm[1], lf_pwm[2],
            direction_[1], lb_pwm[0], lb_pwm[1], lb_pwm[2],
            direction_[2], rf_pwm[0], rf_pwm[1], rf_pwm[2],
            direction_[3], rb_pwm[0], rb_pwm[1], rb_pwm[2]);

    Serial2.println(order);
}

void setup() {
    imu.setup(); // 初始化imu的Serial
    controller.setup(); // 設定回授信號接腳

    Timer3.initialize(100000); // 設定每0.1秒控制馬達
    Timer3.attachInterrupt(controlMotor); // 設定中斷服務函式

    Serial.begin(115200); // 與Nano通訊用
    Serial2.begin(115200); // 與下面的Arduino溝通用
}


void loop() {
    // 讀取imu資訊
    imu.get_imu_degree(imu_degree);

    // 讀取馬達資訊
    controller.read(direction_);
    controller.get_vel(motor_vel);
    controller.get_degree(motor_degree);

    //    印出經pid輸出的pwm數值
    //  Serial.println(lf_pwm);
    //  Serial.print(' ');
    //  Serial.print(lb_pwm);
    //  Serial.print(' ');
    //  Serial.print(rf_pwm);
    //  Serial.print(' ');
    //  Serial.println(rb_pwm);

    //    印出目前馬達的線速度
    //  Serial.print(motor_vel[0], 3);
    //  Serial.print(' ');
    //  Serial.print(motor_vel[1], 3);
    //  Serial.print(' ');
    //  Serial.print(motor_vel[2], 3);
    //  Serial.print(' ');
    //  Serial.println(motor_vel[3], 3);

    //    印出目前馬達的角度
    //  Serial.print(motor_degree[0], 3);
    //  Serial.print(' ');
    //  Serial.print(motor_degree[1], 3);
    //  Serial.print(' ');
    //  Serial.print(motor_degree[2], 3);
    //  Serial.print(' ');
    //  Serial.println(motor_degree[3], 3);

    //    印出目前imu訊息(pitch, row, yaw)
    //  Serial.print(imu_degree[0]);
    //  Serial.print(" ");
    //  Serial.print(imu_degree[1]);
    //  Serial.print(" ");
    //  Serial.println(imu_degree[2]);


}

void serialEvent() {

    char str = Serial.read(); // 讀取起始字元
    String data;
    delay(1);
    switch (str) {
        // 傳送機器人資訊給nano
        case 'a':
            data = "a" + String(direction_[0]) + String(motor_vel[0]) + " " + String(direction_[1]) +
                   String(motor_vel[1]) + " " + String(direction_[2]) + String(motor_vel[2]) + " " +
                   String(direction_[3]) + String(motor_vel[3]) + "|" + \
             String(motor_degree[0]) + " " + String(motor_degree[1]) + " " + String(motor_degree[2]) + " " +
                   String(motor_degree[3]) + "|" + \
             String(imu_degree[0]) + " " + String(imu_degree[1]) + " " + String(imu_degree[2]);

            Serial.println(data);

            break;

            // 從nano端接收各馬達期望速度與方向 ("dxx.xx,xx.xx,xx.xx,xx.xx\n")
        case 'd':
            data = Serial.readStringUntil('\n');

            int start_idx = 0;
            for (int i = 0; i < 4; i++) {
                int end_idx = data.indexOf(",");
                desired_vel[i] = data.substring(start_idx, end_idx).toFloat();

                // 防止馬達動作異常
                if (desired_vel[i] != 0.0) {
                    if (desired_vel[i] > 0)
                        direction_[i] = '+';
                    else {
                        direction_[i] = '-';
                        desired_vel[i] *= -1;
                    }
                }

                data.setCharAt(end_idx, '_');
                start_idx = end_idx + 1;

            }
            break;
    }

    Serial.flush();
    serialFlush();

}


void serialFlush() {
    while (Serial.available() > 0) {
        char t = Serial.read();
    }
}

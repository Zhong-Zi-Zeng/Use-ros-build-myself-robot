//
// Created by ximen on 2022/12/25.
//

#ifndef _PARAMETER_H
#define _PARAMETER_H

#define _PPR 143 // 馬達每圈產生幾個脈波訊號
#define _INTERVAL 100 // 每幾毫秒檢測一次回授訊號
#define _DIAMETER 7.5 // 輪胎直徑 (cm)
#define _MAX_VEL 13.0 // 馬達最快速度
#define _MIN_PWM 10 // 最低PWM限制
#define _MAX_PWM 180 // 最高PWM限制

//#define _Kp  0.004
//#define _Ki  0.0005
//#define _Kd  0.5

#define _Kp  0.01
#define _Ki  0.0005
#define _Kd  0.5
#define _SAMPLE_RATE  0.1 // PID採樣間隔(second)
#endif

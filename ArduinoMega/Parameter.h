//
// Created by ximen on 2022/12/25.
//

#ifndef _PARAMETER_H
#define _PARAMETER_H

#define _PPR 143 // 馬達每圈產生幾個脈波訊號
#define _INTERVAL 100 // 每幾毫秒檢測一次回授訊號
#define _DIAMETER 7.5 // 輪胎直徑 (cm)
#define _MIN_PWM 0 // 最低PWM限制
#define _MAX_PWM 180 // 最高PWM限制
#define _OUTPUT_RAMP 1000.0 // 抑制PID超調
#define _SAMPLE_RATE  0.1 // PID採樣間隔(second)

#define _Kp  0.04
#define _Ki  0.001
#define _Kd  0.4

#endif
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <PID_v1.h>
typedef struct {
  double TargetTicksPerFrame;    // 目标速度 mm/s
  double input;                    //1/30秒计算出的速度 mm/s
  double output;                   // pwm值
  long Encoder;                  //电机当前的转脉冲数
  long PrevEnc;                  //电机前一帧的脉冲数
  int PrevInput;
  bool dir;//true正传  false反转
} SetPointInfo;

void initPID();
void doPID();
void updatePIDSpeed();

#endif


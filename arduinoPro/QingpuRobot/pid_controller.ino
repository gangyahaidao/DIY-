#include "pid_controller.h"

SetPointInfo leftMotor, rightMotor;//存储左右轮子的状态值

double Kp = 18.0 / 50;
double Ki = 35.0 / 50;
double Kd = 0.0 / 50;

unsigned char moving = 1; // 默认启动
PID myPIDL(&(leftMotor.input), &(leftMotor.output), &(leftMotor.TargetTicksPerFrame), Kp, Ki, Kd, DIRECT);
PID myPIDR(&(rightMotor.input), &(rightMotor.output), &(rightMotor.TargetTicksPerFrame), Kp, Ki, Kd, DIRECT);

void initPID() { //初始化PID控制类
  myPIDL.SetMode(AUTOMATIC);
  myPIDR.SetMode(AUTOMATIC);
}

void resetPID() {
  leftMotor.TargetTicksPerFrame = 0.0;
  leftMotor.Encoder = readEncoder(LEFT);
  leftMotor.output = 0;
  leftMotor.Encoder = readEncoder(LEFT);
  leftMotor.PrevInput = 0;
  leftMotor.PrevEnc = leftMotor.Encoder;

  rightMotor.TargetTicksPerFrame = 0.0;
  rightMotor.Encoder = readEncoder(RIGHT);
  rightMotor.output = 0;
  rightMotor.Encoder = readEncoder(RIGHT);
  rightMotor.PrevInput = 0;
  rightMotor.PrevEnc = rightMotor.Encoder;
}

void doPID() {  
  leftMotor.input = (leftMotor.Encoder - leftMotor.PrevEnc)*19.792034;//当前速度mm/s
  rightMotor.input = (rightMotor.Encoder - rightMotor.PrevEnc)*19.792034;
  /*Serial3.print('A');//用于PID波形调试
  Serial3.println(leftMotor.input);
  Serial3.print('B');
  Serial3.println(rightMotor.input);*/
  myPIDL.Compute();//计算PID输出值
  myPIDR.Compute();  
  /*Serial3.print(leftMotor.output);
  Serial3.print("; ");
  Serial3.println(rightMotor.output);*/
  //左轮
  leftMotor.PrevEnc = leftMotor.Encoder;
  leftMotor.PrevInput = leftMotor.input;
  //右轮
  rightMotor.PrevEnc = rightMotor.Encoder;
  rightMotor.PrevInput = rightMotor.input;
}

void updatePID() {
  leftMotor.Encoder = readEncoder(LEFT);
  rightMotor.Encoder = readEncoder(RIGHT);
  //Serial.print(leftMotor.Encoder);
  //Serial.println(rightMotor.Encoder);
  if (!moving) {
    if (leftMotor.PrevInput != 0 || rightMotor.PrevInput != 0)
      resetPID();
    return;
  }
  doPID();
  setMotorSpeeds(leftMotor.output, rightMotor.output);
  //setMotorSpeeds(MAX_PWM, MAX_PWM);
}

long readPidIn(int i) {
  long pidin = 0;
  if (i == LEFT) {
    pidin = leftMotor.PrevInput;
  } else {
    pidin = rightMotor.PrevInput;
  }
  return pidin;
}

long readPidOut(int i) {
  long pidout = 0;
  if (i == LEFT) {
    pidout = leftMotor.output;
  } else {
    pidout = rightMotor.output;
  }
  return pidout;
}

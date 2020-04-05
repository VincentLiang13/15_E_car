#ifndef _SPEED_H
#define _SPEED_H

extern float DirectionPwm;


void PWMOut(void);
void CalSpeedError(void);
void SpeeedPID_Init(void);
void CalSpeedPID(void);
void SpeedPartitionPwmOut(void);

//void SpeedControl(void);

//pid????????
typedef struct
{
  float curr;  //????
  float set;   //?趨?

  float En;   //??????
  float En_1;
  float En_2;
  
  float Kp;   //???????
  float Tsam; //采样周期---控制周期，每隔Tsam控制器输出一次PID运算结果
  float Ti;//?????????
  float Td;//????????
  
  float Dout;//增量PID计算本次应该输出的增量值--本次计算的结果
  
  u16 calc_cycle; 
  short currpwm;  //当前的pwm宽度
  u16 pwm_cycle; //pwm的周期

}speedPID;

extern speedPID  s_pid;
//extern u8 STATUS;

#endif
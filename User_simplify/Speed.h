#ifndef _SPEED_H
#define _SPEED_H

extern float DirectionPwm;

void PWMOut(void);
void CalSpeedError(void);
void SpeeedPID_Init(void);
void CalSpeedPID(void);
void SpeedPartitionPwmOut(void);

//pid初始化结构体
typedef struct
{
  float curr;  //当前值
  float set;   //设定值

  float En;   //偏差
  float En_1; //前一次偏差
  float En_2; //前两次偏差
  
  float Kp;   //展开式中的系数kp，不是最终的Kp
  float Tsam; //采样周期---控制周期，每隔Tsam控制器输出一次PID运算结果
  float Ti;   //展开式中的系数TI
  float Td;   //展开式中的系数Td
  
  float Dout;//增量PID计算本次应该输出的增量值--本次计算的结果
  
  u16 calc_cycle; //计算周期，用不到
  short currpwm;  //当前的pwm
  u16 pwm_cycle;  //pwm宽度

}speedPID;

extern speedPID  s_pid;

#endif
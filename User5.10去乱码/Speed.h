#ifndef _SPEED_H
#define _SPEED_H

extern float DirectionPwm;

void PWMOut(void);
void CalSpeedError(void);
void SpeeedPID_Init(void);
void CalSpeedPID(void);
void SpeedPartitionPwmOut(void);

//pid��ʼ���ṹ��
typedef struct
{
  float curr;  //��ǰֵ
  float set;   //�趨ֵ

  float En;   //ƫ��
  float En_1; //ǰһ��ƫ��
  float En_2; //ǰ����ƫ��
  
  float Kp;   //չ��ʽ�е�ϵ��kp���������յ�Kp
  float Tsam; //��������---�������ڣ�ÿ��Tsam���������һ��PID������
  float Ti;   //չ��ʽ�е�ϵ��TI
  float Td;   //չ��ʽ�е�ϵ��Td
  
  float Dout;//����PID���㱾��Ӧ�����������ֵ--���μ���Ľ��
  
  u16 calc_cycle; //�������ڣ��ò���
  short currpwm;  //��ǰ��pwm
  u16 pwm_cycle;  //pwm���

}speedPID;

extern speedPID  s_pid;

#endif
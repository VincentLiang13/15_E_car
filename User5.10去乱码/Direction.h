#ifndef _DIRECTION_H
#define _DIRECTION_H

void Read_ADC(void);
void DirectionPID_Init(void);
void CalDirectionPID(void);
void DirectionPartitionPwmOut(void);
void CalDirectionError(void);

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

}directionPID;

extern directionPID  d_pid;   

#endif

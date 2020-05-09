#include "include.h"
#include "LQ_MOTOR.h"
#include "Speed.h"
#include "Param.h"
/**********ȫ�ֱ���********/
int32 LeftDead = 0;  //���������һ��Ϊ 10-15%
int32 RighDead = 0;  //
int32 g_nLeftpulse = 0, g_nRighpulse = 0;
//�������������ٶȣ���pwm�����ٶ�
//float Ratio_Encoder_Left = 207/(512*0.02);// �����ٶ�=counter*�����ܳ�(mm)/(����תһȦ��Ӧ��������*��������) 
//float Ratio_Encoder_Righ = 207/(512*0.02);// �����ٶ�=counter*�����ܳ�(mm)/(����תһȦ��Ӧ��������*��������)
float g_fRealSpeed = 0;		//��ʵ�ٶȣ�δ�˲�
float g_fLeftRealSpeed;
float g_fRighRealSpeed;
float g_fSpeedFilter = 0;		//��ʵ�ٶȣ������ٶ�ƽ���˲����޷��˲�

float g_fExpectSpeed = 500;		//����pwmֵ��������max==1000
float g_fSpeedError;			//�ٶ�ƫ��

int32 LeftPWM=0, RightPWM=0;  //
int32 LeftPWM_F= 0, RighPWM_F= 0;  //����pwm

float bangbangValue = 500;            //�����㷨ֵ
float oriSpeedPwm;     //ԭʼpwm���ɵ��Ӹ����㷨
float SpeedPwm = 0;    //���������pwm
speedPID  s_pid;

extern Param param;

float g_PWMOut;

/*------------------------------------------------------------------------------------------------------
 * Name         : PWMOut
 * Function     : ���pwm
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void PWMOut(void)
{      
    //��ʼ��pwm����Ϊ300
	DirectionPwm = 0;//�����ٶ�pid����DirectionPwm��Ϊ0

	LeftPWM = (int32)(300-DirectionPwm+SpeedPwm);  //
	RightPWM = (int32)(300+DirectionPwm+SpeedPwm); 
        
	abs(LeftPWM);//pwmȡ��
	LeftPWM_F = LeftDead + LeftPWM;
	LeftPWM_F = (LeftPWM_F > 950? 950: LeftPWM_F);//�޷�
	FTM_PwmDuty(FTM0, FTM_CH4, 0);
	FTM_PwmDuty(FTM0, FTM_CH5, LeftPWM_F);
		
	abs(RightPWM);
	RighPWM_F = RighDead + RightPWM;
	RighPWM_F = (RighPWM_F > 950? 950: RighPWM_F);
	FTM_PwmDuty(FTM0, FTM_CH6, 0);
	FTM_PwmDuty(FTM0, FTM_CH7, RighPWM_F);
	
	param.LeftPWM_F = LeftPWM_F;//���ݲ�������ʾ����
	param.RighPWM_F = RighPWM_F;
}


/*------------------------------------------------------------------------------------------------------
 * Name         : CalSpeedError
 * Function     : �����ٶ�ƫ��
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void CalSpeedError(void)
{
	static float fSpeedOld = 0;//�����ٶ�ƫ�����
	static float fSpeedNew = 0;
        
	g_nLeftpulse = FTM_ABGet(FTM1);
	g_nRighpulse = FTM_ABGet(FTM2);
	//�������ٶȻ���
	g_fLeftRealSpeed = g_nLeftpulse;
	g_fLeftRealSpeed = (g_fLeftRealSpeed>2000?2000:g_fLeftRealSpeed);		//�޷�
	g_fRighRealSpeed = g_nRighpulse;
	g_fRighRealSpeed = (g_fRighRealSpeed>2000?2000:g_fRighRealSpeed);		//�޷�
	    
	g_fRealSpeed = (g_fLeftRealSpeed + g_fRighRealSpeed)*0.5;	

	fSpeedOld = g_fSpeedFilter;
	fSpeedNew = g_fRealSpeed;
	//�ٶ�ƫ���޷�
	if(fSpeedNew>=fSpeedOld)
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)>300?(fSpeedOld+300):fSpeedNew);
	else
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)<-300?(fSpeedOld-300):fSpeedNew);
	
        param.g_fSpeedFilter = g_fSpeedFilter;//���ݲ�������ʾ����      
	
	g_fSpeedError =  g_fExpectSpeed - g_fSpeedFilter;
}

/*------------------------------------------------------------------------------------------------------
 * Name         : SpeeedPID_Init
 * Function     : �ٶ�pid��ʼ��
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void SpeeedPID_Init()  
{
	
	s_pid.set =g_fExpectSpeed;
	s_pid.currpwm=0;
	s_pid.pwm_cycle=1000;  //�����pwm���ֵ���޷����� 
	s_pid.calc_cycle=20;  //pid�������ڣ�20ms��pit�жϣ�����û��
	s_pid.Td=0;
	s_pid.Ti=4000;
	s_pid.Kp=50;
	s_pid.Tsam=20;//pid�������ڣ�20ms��pit�ж�
	s_pid.En=0;
	s_pid.En_1=0;
	s_pid.En_2=0;
	
	param.g_fExpectSpeed = g_fExpectSpeed;

}

/*------------------------------------------------------------------------------------------------------
 * Name         : CalSpeedPID
 * Function     : �����ٶ�pid
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : ����ʽ
--------------------------------------------------------------------------------------------------------*/
void CalSpeedPID(void)
{
  float dk1;float dk2;
  float t1,t2,t3;
	
    CalSpeedError();
          
		s_pid.En = g_fSpeedError;
		dk1=s_pid.En-s_pid.En_1;   
		dk2=s_pid.En-2*s_pid.En_1+s_pid.En_2;
	
    t1=s_pid.Kp*dk1;//Kp����һ��
	
		t2=(s_pid.Kp*s_pid.Tsam)/s_pid.Ti;
		
	t2=t2*s_pid.En;//Ki���ڶ���
	t2= 0;//����ʱ��Ϊ0
	
		t3=(s_pid.Kp*s_pid.Td)/s_pid.Tsam;
	
	t3=t3*dk2;//Kd��������
	t3= 0;//����ʱ��Ϊ0
	
    s_pid.Dout=t1+t2+t3;  //�˴������pwm
	s_pid.currpwm+=s_pid.Dout;  //��ǰ��Ҫ�����pwm������ʽ

	if(s_pid.currpwm>s_pid.pwm_cycle) {s_pid.currpwm=s_pid.pwm_cycle;}//�޷�
	if(s_pid.currpwm<0) {s_pid.currpwm=0;}
	
	s_pid.En_2=s_pid.En_1;
	s_pid.En_1=s_pid.En;
	oriSpeedPwm=s_pid.currpwm;
}


/*------------------------------------------------------------------------------------------------------
 * Name         : SpeedPartitionPwmOut
 * Function     : �ٶȷ���pwm���
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : ����ʵ�ָ����㷨��������㷨
--------------------------------------------------------------------------------------------------------*/
void SpeedPartitionPwmOut()
{	
	CalSpeedPID();
	//�����㷨������ʱ����
	/*
	if (oriSpeedPwm < bangbangValue)
	{	
		SpeedPwm = 1000 ;//����ռ�ձ�
	}
	*/
	SpeedPwm = oriSpeedPwm;
        
    param.SpeedPwm = SpeedPwm;//���ݲ�������ʾ����
}


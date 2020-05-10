#include "include.h"
#include "LQ_MOTOR.h"
#include "Speed.h"
#include "Param.h"
/**********全局变量********/
int32 LeftDead = 0;  //电机死区，一般为 10-15%
int32 RighDead = 0;  //
int32 g_nLeftpulse = 0, g_nRighpulse = 0;
//以下舍弃计算速度，用pwm估计速度
//float Ratio_Encoder_Left = 207/(512*0.02);// 左轮速度=counter*左轮周长(mm)/(左轮转一圈对应的脉冲数*程序周期) 
//float Ratio_Encoder_Righ = 207/(512*0.02);// 右轮速度=counter*右轮周长(mm)/(右轮转一圈对应的脉冲数*程序周期)
float g_fRealSpeed = 0;		//真实速度，未滤波
float g_fLeftRealSpeed;
float g_fRighRealSpeed;
float g_fSpeedFilter = 0;		//真实速度，左右速度平均滤波，限幅滤波

float g_fExpectSpeed = 500;		//期望pwm值（待定）max==1000
float g_fSpeedError;			//速度偏差

int32 LeftPWM=0, RightPWM=0;  //
int32 LeftPWM_F= 0, RighPWM_F= 0;  //左右pwm

float bangbangValue = 500;            //棒棒算法值
float oriSpeedPwm;     //原始pwm，可叠加各种算法
float SpeedPwm = 0;    //最终输出的pwm
speedPID  s_pid;

extern Param param;

float g_PWMOut;

/*------------------------------------------------------------------------------------------------------
 * Name         : PWMOut
 * Function     : 输出pwm
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void PWMOut(void)
{      
    //起始的pwm设置为300
	DirectionPwm = 0;//调试速度pid，把DirectionPwm设为0

	LeftPWM = (int32)(300-DirectionPwm+SpeedPwm);  //
	RightPWM = (int32)(300+DirectionPwm+SpeedPwm); 
        
	abs(LeftPWM);//pwm取正
	LeftPWM_F = LeftDead + LeftPWM;
	LeftPWM_F = (LeftPWM_F > 950? 950: LeftPWM_F);//限幅
	FTM_PwmDuty(FTM0, FTM_CH4, 0);
	FTM_PwmDuty(FTM0, FTM_CH5, LeftPWM_F);
		
	abs(RightPWM);
	RighPWM_F = RighDead + RightPWM;
	RighPWM_F = (RighPWM_F > 950? 950: RighPWM_F);
	FTM_PwmDuty(FTM0, FTM_CH6, 0);
	FTM_PwmDuty(FTM0, FTM_CH7, RighPWM_F);
	
	param.LeftPWM_F = LeftPWM_F;//传递参数，给示波器
	param.RighPWM_F = RighPWM_F;
}


/*------------------------------------------------------------------------------------------------------
 * Name         : CalSpeedError
 * Function     : 计算速度偏差
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void CalSpeedError(void)
{
	static float fSpeedOld = 0;//用于速度偏差计算
	static float fSpeedNew = 0;
        
	g_nLeftpulse = FTM_ABGet(FTM1);
	g_nRighpulse = FTM_ABGet(FTM2);
	//不进行速度换算
	g_fLeftRealSpeed = g_nLeftpulse;
	g_fLeftRealSpeed = (g_fLeftRealSpeed>2000?2000:g_fLeftRealSpeed);		//限幅
	g_fRighRealSpeed = g_nRighpulse;
	g_fRighRealSpeed = (g_fRighRealSpeed>2000?2000:g_fRighRealSpeed);		//限幅
	    
	g_fRealSpeed = (g_fLeftRealSpeed + g_fRighRealSpeed)*0.5;	

	fSpeedOld = g_fSpeedFilter;
	fSpeedNew = g_fRealSpeed;
	//速度偏差限幅
	if(fSpeedNew>=fSpeedOld)
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)>300?(fSpeedOld+300):fSpeedNew);
	else
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)<-300?(fSpeedOld-300):fSpeedNew);
	
        param.g_fSpeedFilter = g_fSpeedFilter;//传递参数，给示波器      
	
	g_fSpeedError =  g_fExpectSpeed - g_fSpeedFilter;
}

/*------------------------------------------------------------------------------------------------------
 * Name         : SpeeedPID_Init
 * Function     : 速度pid初始化
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void SpeeedPID_Init()  
{
	
	s_pid.set =g_fExpectSpeed;
	s_pid.currpwm=0;
	s_pid.pwm_cycle=1000;  //输出的pwm最大值，限幅作用 
	s_pid.calc_cycle=20;  //pid计算周期，20ms用pit中断，这里没用
	s_pid.Td=0;
	s_pid.Ti=4000;
	s_pid.Kp=50;
	s_pid.Tsam=20;//pid计算周期，20ms用pit中断
	s_pid.En=0;
	s_pid.En_1=0;
	s_pid.En_2=0;
	
	param.g_fExpectSpeed = g_fExpectSpeed;

}

/*------------------------------------------------------------------------------------------------------
 * Name         : CalSpeedPID
 * Function     : 计算速度pid
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 增量式
--------------------------------------------------------------------------------------------------------*/
void CalSpeedPID(void)
{
  float dk1;float dk2;
  float t1,t2,t3;
	
    CalSpeedError();
          
		s_pid.En = g_fSpeedError;
		dk1=s_pid.En-s_pid.En_1;   
		dk2=s_pid.En-2*s_pid.En_1+s_pid.En_2;
	
    t1=s_pid.Kp*dk1;//Kp，第一项
	
		t2=(s_pid.Kp*s_pid.Tsam)/s_pid.Ti;
		
	t2=t2*s_pid.En;//Ki，第二项
	t2= 0;//调试时设为0
	
		t3=(s_pid.Kp*s_pid.Td)/s_pid.Tsam;
	
	t3=t3*dk2;//Kd，第三项
	t3= 0;//调试时设为0
	
    s_pid.Dout=t1+t2+t3;  //此次输出的pwm
	s_pid.currpwm+=s_pid.Dout;  //当前需要输出的pwm，增量式

	if(s_pid.currpwm>s_pid.pwm_cycle) {s_pid.currpwm=s_pid.pwm_cycle;}//限幅
	if(s_pid.currpwm<0) {s_pid.currpwm=0;}
	
	s_pid.En_2=s_pid.En_1;
	s_pid.En_1=s_pid.En;
	oriSpeedPwm=s_pid.currpwm;
}


/*------------------------------------------------------------------------------------------------------
 * Name         : SpeedPartitionPwmOut
 * Function     : 速度分区pwm输出
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 这里实现各种算法，如棒棒算法
--------------------------------------------------------------------------------------------------------*/
void SpeedPartitionPwmOut()
{	
	CalSpeedPID();
	//棒棒算法，调试时曲线
	/*
	if (oriSpeedPwm < bangbangValue)
	{	
		SpeedPwm = 1000 ;//给满占空比
	}
	*/
	SpeedPwm = oriSpeedPwm;
        
    param.SpeedPwm = SpeedPwm;//传递参数，给示波器
}


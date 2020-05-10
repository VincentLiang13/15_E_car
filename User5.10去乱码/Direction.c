#include "include.h"
#include "LQ_ADC.h"
#include "Direction.h"

/**********全局变量********/
directionPID  d_pid;
float DirectionErr[3];	//三对速度偏差
int16 sensor[6]={0};       //六个电感值滤波后
float ad_value_min = 200;  //电感值min
float ad_value_max = 1000;  //电感值max
float oriDirectionPwm[3] ;//原始方向pwm，可以加算法
float DirectionPwm;      //最终pwm

/*------------------------------------------------------------------------------------------------------
 * Name         : DirectionPID_Init
 * Function     : 方向pid初始化
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void DirectionPID_Init()  
{
  //d_pid.set =0;
  d_pid.currpwm=0;
  d_pid.pwm_cycle=100;    
  d_pid.calc_cycle=100;  
  d_pid.Td=2000;
  //d_pid.Ti=4000;
  d_pid.Kp=5;
  d_pid.Tsam=5;//5ms一次方向pid
  d_pid.En=0;
  d_pid.En_1=0;
  d_pid.En_2=0;

}

/*------------------------------------------------------------------------------------------------------
 * Name         : CalDirectionPID
 * Function     : 计算方向pid
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void CalDirectionPID(void)
{

  float dk1;float dk2;
  float t1,t2,t3;
  short i; 

  for (i = 0; i < 2; i++)
  {
    CalDirectionError();

      d_pid.En = DirectionErr[i];
      dk1=d_pid.En-d_pid.En_1;   
      dk2=d_pid.En-2*d_pid.En_1+d_pid.En_2;

    t1=d_pid.Kp*dk1;
        
        t2=(d_pid.Kp*d_pid.Tsam)/d_pid.Ti;    
    t2=t2*d_pid.En;
    t2=0;//调试时设为0
        
        t3=(d_pid.Kp*d_pid.Td)/d_pid.Tsam;
    t3=t3*dk2;
    t3=0;//调试时设为0
        
    d_pid.Dout=t1+t2+t3;  ////此次输出的pwm
    d_pid.currpwm+=d_pid.Dout;  //当前需要输出的pwm，增量式
    if(d_pid.currpwm>d_pid.pwm_cycle) {d_pid.currpwm=d_pid.pwm_cycle;}//限幅
    if(d_pid.currpwm<0) {d_pid.currpwm=0;}

    oriDirectionPwm[i]=d_pid.currpwm;
    d_pid.En_2=d_pid.En_1;
    d_pid.En_1=d_pid.En;
  } 
}

/*------------------------------------------------------------------------------------------------------
 * Name         : DirectionPartitionPwmOut
 * Function     : 方向分区pid输出
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 这里实现各种特殊位置识别算法，环岛等
--------------------------------------------------------------------------------------------------------*/
void DirectionPartitionPwmOut()
{       
  CalDirectionPID();
  //调试
  if (/*flagRound = 1*/ 1)  
  { 
    /*自定义*/
    DirectionPwm = oriDirectionPwm[0];
  }
  else if(1)//两对水平电感加权
  {
    DirectionPwm = oriDirectionPwm[1]*30+oriDirectionPwm[2]*70; 
  }
}

/*------------------------------------------------------------------------------------------------------
 * Name         : CalDirectionError
 * Function     : 计算方向pid偏差
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void CalDirectionError()
{
  Read_ADC();

  //竖直电感2	
	DirectionErr[0] = (float)(sensor[0]-sensor[5])/(sensor[0]+sensor[5]);//差比和
	DirectionErr[0] = (DirectionErr[0]>= 1? 1:DirectionErr[0]);       //限幅	
	DirectionErr[0] = (DirectionErr[0]<=-1?-1:DirectionErr[0]);
	//水平电感2		
	DirectionErr[1] = (float)(sensor[1]-sensor[4])/(sensor[1]+sensor[4]);
	DirectionErr[1] = (DirectionErr[1]>= 1? 1:DirectionErr[1]);	
	DirectionErr[1] = (DirectionErr[1]<=-1?-1:DirectionErr[1]);	
	//水平电感1	
	DirectionErr[2] = (float)(sensor[2]-sensor[3])/(sensor[2]+sensor[3]);
	DirectionErr[2] = (DirectionErr[2]>= 1? 1:DirectionErr[2]);	
	DirectionErr[2] = (DirectionErr[2]<=-1?-1:DirectionErr[2]);	

}


/*------------------------------------------------------------------------------------------------------
 * Name         : Read_ADC
 * Function     : 读取电感值
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 滑动平均滤波+归一化
--------------------------------------------------------------------------------------------------------*/    
void Read_ADC() 
{
  int16_t i,j,k,temp;
  int16_t ad_value[5][5],ad_valueF[5],ad_sum[5];
  float ad_value_one[5];
//     int16_t ad_value_old[5],ad_value_new[5];  

  //二维数组初始化
  for(i=0;i<5;i++)
  {
    for(int j = 0; j < 5; j++)
    {
      ad_value[i][j]=0;
    }
  }
  i=0;j=0;
  
  for(i=0;i<5;i++)
  {   
      ad_value[0][i]=ADC_Get(0);  // 从左到右0~5
      ad_value[1][i]=ADC_Get(1);  //           
      ad_value[2][i]=ADC_Get(2);  // 
      ad_value[3][i]=ADC_Get(3);  //          
      ad_value[4][i]=ADC_Get(4);  // 
      ad_value[5][i]=ADC_Get(5);  // 
  }  

/*=========================冒泡降序排列==========================*/
  for(i=0;i<5;i++)
  {
    for(j=0;j<4;j++)
    {
      for(k=0;k<4-j;k++)  
      {
        if(ad_value[i][k]>ad_value[i][k+1]) 
        {
          temp=ad_value[i][k+1];
          ad_value[i][k+1]=ad_value[i][k];
          ad_value[i][k]= temp;
        }
      }
    }
  }
/*===========================滑动平均滤波=================================*/
  for(i=0;i<5;i++)    
  {
    ad_sum[i] =ad_value[i][1]+ad_value[i][2]+ad_value[i][3];  //取中间三位
    ad_valueF[i]=ad_sum[i]/3;  
    //归一化
    ad_value_one[i]=(ad_valueF[i]-ad_value_min)/(ad_value_max-ad_value_min);
    ad_value_one[i]=(ad_valueF[i] < 0 ? 0 : ad_valueF[i]);
    ad_value_one[i]=(ad_valueF[i] > 1 ? 1 : ad_valueF[i]);
  }	
  for(i=0;i<5;i++)  //乘100
  {
    sensor[i] = (int16_t)(ad_value_one[i]*100);//sensor[i] (0,100)
  }       

}
	 


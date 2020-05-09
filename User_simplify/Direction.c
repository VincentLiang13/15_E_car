#include "include.h"
#include "LQ_ADC.h"
#include "Direction.h"

/**********????********/
directionPID  d_pid;
float DirectionErr[3];	//???????
int16 sensor[6]={0};       //??????
float ad_value_min = 200;  //???min?
float ad_value_max = 1000;  //???max?
float oriDirectionPwm[3] ;//??pwm??????
float DirectionPwm;      //??pwm

/*------------------------------------------------------------------------------------------------------
 * Name         : DirectionPID_Init
 * Function     : ??pid???
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
  d_pid.Tsam=5;//5ms??pid
  d_pid.En=0;
  d_pid.En_1=0;
  d_pid.En_2=0;

}

/*------------------------------------------------------------------------------------------------------
 * Name         : CalDirectionPID
 * Function     : ????pid
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
      dk1=d_pid.En-d_pid.En_1;   //??????????????????
      dk2=d_pid.En-2*d_pid.En_1+d_pid.En_2;

    t1=d_pid.Kp*dk1;
        
        t2=(d_pid.Kp*d_pid.Tsam)/d_pid.Ti;    
    t2=t2*d_pid.En;
    t2=0;//????0
        
        t3=(d_pid.Kp*d_pid.Td)/d_pid.Tsam;
    t3=t3*dk2;
    t3=0;//????0
        
    d_pid.Dout=t1+t2+t3;  //?????pwm
    d_pid.currpwm+=d_pid.Dout;  //???????pwm????
    if(d_pid.currpwm>d_pid.pwm_cycle) {d_pid.currpwm=d_pid.pwm_cycle;}//??
    if(d_pid.currpwm<0) {d_pid.currpwm=0;}

    oriDirectionPwm[i]=d_pid.currpwm;
    d_pid.En_2=d_pid.En_1;
    d_pid.En_1=d_pid.En;
  } 
}

/*------------------------------------------------------------------------------------------------------
 * Name         : DirectionPartitionPwmOut
 * Function     : ????pid??
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : ???????????
--------------------------------------------------------------------------------------------------------*/
void DirectionPartitionPwmOut()
{       
  CalDirectionPID();
  //? ??
  if (/*flagRound = 1*/ 1)  
  { 
    /*???*/
    DirectionPwm = oriDirectionPwm[0];
  }
  else if(1)//????????
  {
    DirectionPwm = oriDirectionPwm[1]*30+oriDirectionPwm[2]*70; 
  }
}

/*------------------------------------------------------------------------------------------------------
 * Name         : CalDirectionError
 * Function     : ????pid??
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/
void CalDirectionError()
{
  Read_ADC();

  //??????		
	DirectionErr[0] = (float)(sensor[0]-sensor[5])/(sensor[0]+sensor[5]);//???
	DirectionErr[0] = (DirectionErr[0]>= 1? 1:DirectionErr[0]);       //??	
	DirectionErr[0] = (DirectionErr[0]<=-1?-1:DirectionErr[0]);
	//????2??
	DirectionErr[1] = (float)(sensor[1]-sensor[4])/(sensor[1]+sensor[4]);
	DirectionErr[1] = (DirectionErr[1]>= 1? 1:DirectionErr[1]);	
	DirectionErr[1] = (DirectionErr[1]<=-1?-1:DirectionErr[1]);	
	//????1??
	DirectionErr[2] = (float)(sensor[2]-sensor[3])/(sensor[2]+sensor[3]);
	DirectionErr[2] = (DirectionErr[2]>= 1? 1:DirectionErr[2]);	
	DirectionErr[2] = (DirectionErr[2]<=-1?-1:DirectionErr[2]);	

}


/*------------------------------------------------------------------------------------------------------
 * Name         : Read_ADC
 * Function     : ?????
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 
--------------------------------------------------------------------------------------------------------*/    
void Read_ADC() 
{
  int16_t i,j,k,temp;
  int16_t ad_value[5][5],ad_valueF[5],ad_sum[5];
  float ad_value_one[5];
//     int16_t ad_value_old[5],ad_value_new[5];  

  //???????
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
      ad_value[0][i]=ADC_Get(0);  // ????0~5
      ad_value[1][i]=ADC_Get(1);  //           
      ad_value[2][i]=ADC_Get(2);  // 
      ad_value[3][i]=ADC_Get(3);  //          
      ad_value[4][i]=ADC_Get(4);  // 
      ad_value[5][i]=ADC_Get(5);  // 
  }  

/*=========================???? ??==========================*/
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
/*===========================????=================================*/
  for(i=0;i<5;i++)    
  {
    ad_sum[i] =ad_value[i][1]+ad_value[i][2]+ad_value[i][3];  //????
    ad_valueF[i]=ad_sum[i]/3;  
    //???
    ad_value_one[i]=(ad_valueF[i]-ad_value_min)/(ad_value_max-ad_value_min);
    ad_value_one[i]=(ad_valueF[i] < 0 ? 0 : ad_valueF[i]);
    ad_value_one[i]=(ad_valueF[i] > 1 ? 1 : ad_valueF[i]);
  }	
  for(i=0;i<5;i++)  //?100
  {
    sensor[i] = (int16_t)(ad_value_one[i]*100);//sensor[i] (0,100)
  }       

}
	 


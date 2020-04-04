#include "include.h"
#include "LQ_ADC.h"
#include "Direction.h"

float DirectionErr[3];	//???????g_fDirectionError[0]??????��???????
				//??[1]?????????1????????
				//??g_fDirectionError[2]?????????2????????
int16 sensor[6]={0};       //???????
float ad_value_min = 200;// ?????????g_fDirectionError
float ad_value_max = 1000;
float oriDirectionPwm[3] ;//?????????origional pwm
float DirectionPwm;      //?????????pwm
directionPID  d_pid;
/*
float Direction_P = 610;	//???????P  ???P????????? ?????????��????????????????P
float Direction_D = 2200;//???????D
float DirErr_D_P = 0.3;        //?????1 ??????????  [0,1]        


float DirectionErr_dot[3];//???????????g_fDirectionError_dot[0]????????��???????????
				//??g_fDirectionError_dot[1]?????????1????????????
                                  //??g_fDirectionError_dot[2]?????????2????????????
float DirectionOut=0;	//??????????


//int16 sensorFilter[5]={0};	//??????????????��????
uint8 Flag_Round1 = 0;		//????????????????????ON??
uint8 Flag_Round2 = 0;
uint8 Flag_Round3 = 0;  //???��????????????????

*/

/**
 * @file		???????
 *				?????????????????��??????????
 *				???????��??????????��??????????
 *
 *			                        ???????????
 *
	g_ValueOfAD[0]					        g_ValueOfAD[1]
 *	(???????)						??????��?

 *		g_ValueOfAD[2]				 g_ValueOfAD[3]
 *		????????1)	                           ????????1??
 *		
                        g_ValueOfAD[4]		g_ValueOfAD[5]
 *		        ????????2??		????????2??
 * @date		2020
 */


/*------------------------------------------------------------------------------------------------------
????    ????DirectionPID_Init
????    ???????pid???????
????    ??????
???? ?? ?????
?????????
--------------------------------------------------------------------------------------------------------*/
void DirectionPID_Init()  
{
  
  //d_pid.set =;
  d_pid.currpwm=0;
  d_pid.pwm_cycle=100;    
  d_pid.calc_cycle=100;  
  d_pid.Td=2000;
  //d_pid.Ti=4000;
  d_pid.Kp=5;
  d_pid.Tsam=20;//20ms???????pid
  d_pid.En=0;
  d_pid.En_1=0;
  d_pid.En_2=0;

}

/*------------------------------------------------------------------------------------------------------
????    ????CalDirectionPID
????    ?????????pid ???SpeedPwm
????    ??????
???? ?? ?????
??????????????pid  ????��????   ?????��????????????????څ
--------------------------------------------------------------------------------------------------------*/
void CalDirectionPID(void)
{

  float dk1;float dk2;
  float t1,t2,t3;
  short i; 
  for (i = 0; i < 2; i++)
  {
        CalDirectionError();
        
//	if(d_pidcalcms<d_pid.Tsam) return ;  //
	//d_pid.En=d_pid.set-d_pid.curr;  //???????
        d_pid.En = DirectionErr[i];
//        dk1=d_pid.En-d_pid.En_1;   //??????????????????
        dk2=d_pid.En-2*d_pid.En_1+d_pid.En_2;
        t1=d_pid.Kp*dk1;
        
//        t2=(d_pid.Kp*d_pid.Tsam)/d_pid.Ti;
//        t2=t2*d_pid.En;
        t2=0;
        
        t3=(d_pid.Kp*d_pid.Td)/d_pid.Tsam;
        t3=t3*dk2;
        
        d_pid.Dout=t1+t2+t3;  //????????????????
        d_pid.currpwm+=d_pid.Dout;  //????????????PWM
        if(d_pid.currpwm>d_pid.pwm_cycle) {d_pid.currpwm=d_pid.pwm_cycle;}//??????
        if(d_pid.currpwm<0) {d_pid.currpwm=0;}
	
        oriDirectionPwm[i]=d_pid.currpwm;//????????????PWM  ???
        d_pid.En_2=d_pid.En_1;
        d_pid.En_1=d_pid.En;
	
           //d_pidcalcms=0;
  } 
}

/*------------------------------------------------------------------------------------------------------
????    ????DirectionPartitionPwmOut
????    ??????????pwm ???
????    ??????
???? ?? ?????
?????????unfinished  ???Partition?��????flagRound
--------------------------------------------------------------------------------------------------------*/
void DirectionPartitionPwmOut()
{       
        CalDirectionPID();
        
        if (/*flagRound = 1*/ 1)  //?? ?????????
        {
           DirectionPwm = oriDirectionPwm[0];
        }
        
        else//?????1?30%  2 ?70%
        {
          DirectionPwm = oriDirectionPwm[1]*30+oriDirectionPwm[2]*70; 
        }
}

/*------------------------------------------------------------------------------------------------------
????    ????CalDirectionError
????    ????????????
????    ??????
???? ?? ????? DirectionErr[i]
?????????
--------------------------------------------------------------------------------------------------------*/
void CalDirectionError()
{
        Read_ADC();
    
    //????????????		
	DirectionErr[0] = (float)(sensor[0]-sensor[1])/(sensor[0]+sensor[1]);//??-??????��??????????
	DirectionErr[0] = (DirectionErr[0]>= 1? 1:DirectionErr[0]);       //??????	
	DirectionErr[0] = (DirectionErr[0]<=-1?-1:DirectionErr[0]);
	//?????1??????
	DirectionErr[1] = (float)(sensor[2]-sensor[3])/(sensor[2]+sensor[3]);//??-??????��??????????
	DirectionErr[1] = (DirectionErr[1]>= 1? 1:DirectionErr[1]);	//??????
	DirectionErr[1] = (DirectionErr[1]<=-1?-1:DirectionErr[1]);	
	//?????2??????
	DirectionErr[2] = (float)(sensor[4]-sensor[5])/(sensor[4]+sensor[5]);//??-??????��??????????
	DirectionErr[2] = (DirectionErr[2]>= 1? 1:DirectionErr[2]);	//??????
	DirectionErr[2] = (DirectionErr[2]<=-1?-1:DirectionErr[2]);	

}


/*------------------------------------------------------------------------------------------------------
????    ????Read_ADC
????    ??????adc?
????    ????sensor[i] ??��?adc?
???? ?? ?????  ?????*100
???????????????+??????????
--------------------------------------------------------------------------------------------------------*/        
void Read_ADC() 
{
     int16_t i,j,k,temp;
     int16_t ad_value[5][5],ad_valueF[5],ad_sum[5];
     float ad_value_one[5];
//     int16_t ad_value_old[5],ad_value_new[5];  

//      ?????        
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
         ad_value[0][i]=ADC_Get(0);  // ???????
//         ad_value[1][i]=ADC_Get(5);  // ???????           
//         ad_value[2][i]=ADC_Get(1);  // ??????1
//         ad_value[3][i]=ADC_Get(4);  // ??????1         
//         ad_value[4][i]=ADC_Get(2);  // ??????2
//         ad_value[5][i]=ADC_Get(3);  // ??????2
     }  
  
     //??��????????
/*=========================???????????==========================*///????????????��?
     for(i=0;i<5;i++)
     {
       for(j=0;j<4;j++)
       {
         for(k=0;k<4-j;k++)  //5??????????????4??
         {
           if(ad_value[i][k]>ad_value[i][k+1]) //??????????????��???
           {
             temp=ad_value[i][k+1];
             ad_value[i][k+1]=ad_value[i][k];
             ad_value[i][k]= temp;
           }
         }
       }
     }
/*===========================??????=================================*/
     for(i=0;i<5;i++)    //???��???????
     {
        ad_sum[i] =ad_value[i][1]+ad_value[i][2]+ad_value[i][3];  //????????��??��?????
        ad_valueF[i]=ad_sum[i]/3;  //??????
        //????? ??0??1??
        ad_value_one[i]=(ad_valueF[i]-ad_value_min)/(ad_value_max-ad_value_min);
        ad_value_one[i]=(ad_valueF[i] < 0 ? 0 : ad_valueF[i]);
        ad_value_one[i]=(ad_valueF[i] > 1 ? 1 : ad_valueF[i]);
     }	
	 for(i=0;i<5;i++)  
         {//??????��?��??????,??????????
	     //sensor[i] = (int16_t)(ad_value_one[i]/10*10);  //
		
             sensor[i] = (int16_t)(ad_value_one[i]*100);//sensor[i] (0,100)
             
             //?????????????��?????��40(??????)
//		ad_value_old[i]=sensorFilter[i];
//		ad_value_new[i]=sensor[i];		
//		if(ValueOfADNew[i]>=ValueOfADOld[i])
//			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])>50?(ValueOfADOld[i]+50):ValueOfADNew[i]);  //??��?��??
//		else
//			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])<-60?(ValueOfADOld[i]-60):ValueOfADNew[i]);  //??��?��??
	 }
}
	 
/*
void DirectionControl(void)
{
  static float DirectionErrTemp[3][5]={0};  //??????????
	Read_ADC();		//???????	
//	if(sensor[0]<10 && sensor[1]<10)  //Stop_Flag=1;	//??????????????	
//	sensor[0] = (sensor[0] < 10? 10:sensor[0]);	//???????
//	sensor[1] = (sensor[1] < 10? 10:sensor[1]);
//	sensor[2] = (sensor[2] < 10? 10:sensor[2]);
//	sensor[3] = (sensor[3] < 10? 10:sensor[3]);
//       sensor[4] = (sensor[4] < 10? 10:sensor[4]);
        
        
        
	//????????????		
	DirectionErr[0] = (float)(sensor[0]-sensor[1])/(sensor[0]+sensor[1]);//??-??????��??????????
	DirectionErr[0] = (DirectionErr[0]>= 1? 1:DirectionErr[0]);    //??????	
	DirectionErr[0] = (DirectionErr[0]<=-1?-1:DirectionErr[0]);
	//?????1??????
	DirectionErr[1] = (float)(sensor[2]-sensor[3])/(sensor[2]+sensor[3]);//??-??????��??????????
	DirectionErr[1] = (DirectionErr[1]>= 1? 1:DirectionErr[1]);	//??????
	DirectionErr[1] = (DirectionErr[1]<=-1?-1:DirectionErr[1]);	
	//?????2??????
	DirectionErr[2] = (float)(sensor[4]-sensor[5])/(sensor[4]+sensor[5]);//??-??????��??????????
	DirectionErr[2] = (DirectionErr[2]>= 1? 1:DirectionErr[2]);	//??????
	DirectionErr[2] = (DirectionErr[2]<=-1?-1:DirectionErr[2]);	
	//???????????????(?????????????)  
	DirectionErrTemp[0][4] = DirectionErrTemp[0][3];
	DirectionErrTemp[0][3] = DirectionErrTemp[0][2];
	DirectionErrTemp[0][2] = DirectionErrTemp[0][1];
	DirectionErrTemp[0][1] = DirectionErrTemp[0][0];
	DirectionErrTemp[0][0] = DirectionErr[0];
	DirectionErr_dot[0] = 5*(DirectionErrTemp[0][0]-DirectionErrTemp[0][3]);//?????��???????
	DirectionErr_dot[0] = (DirectionErr_dot[0]> 0.7? 0.7:DirectionErr_dot[0]);//?????????
	DirectionErr_dot[0] = (DirectionErr_dot[0]<-0.7?-0.7:DirectionErr_dot[0]);
        //?????1?????????(????????????)  
	DirectionErrTemp[1][4] = DirectionErrTemp[1][3];
	DirectionErrTemp[1][3] = DirectionErrTemp[1][2];
	DirectionErrTemp[1][2] = DirectionErrTemp[1][1];
	DirectionErrTemp[1][1] = DirectionErrTemp[1][0];
	DirectionErrTemp[1][0] = DirectionErr[1];
	DirectionErr_dot[1] = 5*(DirectionErrTemp[1][0]-DirectionErrTemp[1][3]);//?????1????????
	DirectionErr_dot[1] = (DirectionErr_dot[1]> 0.7? 0.7:DirectionErr_dot[1]);//?????????
	DirectionErr_dot[1] = (DirectionErr_dot[1]<-0.7?-0.7:DirectionErr_dot[1]);	
        //?????2?????????(????????????)  
	DirectionErrTemp[2][4] = DirectionErrTemp[2][3];
	DirectionErrTemp[2][3] = DirectionErrTemp[2][2];
	DirectionErrTemp[2][2] = DirectionErrTemp[2][1];
	DirectionErrTemp[2][1] = DirectionErrTemp[2][0];
	DirectionErrTemp[2][0] = DirectionErr[2];
	DirectionErr_dot[2] = 5*(DirectionErrTemp[2][0]-DirectionErrTemp[2][3]);//?????2????????
	DirectionErr_dot[2] = (DirectionErr_dot[2]> 0.7? 0.7:DirectionErr_dot[2]);//?????????
	DirectionErr_dot[2] = (DirectionErr_dot[2]<-0.7?-0.7:DirectionErr_dot[2]);	
//?????????????
	if((sensor[0]>900)&&(sensor[1]>900)&&(sensor[2]<200)&&(sensor[3]<200)&&(Flag_Round2<3))
	{
          if(Flag_Round1==0)
             {Flag_Round1=1;}
//          if(Flag_Round2==1)
//             {DirectionErr[1]=DirectionErr[1]*10;
//             DirectionErr_dot[1]=DirectionErr_dot[1]*10;}
	}	
	if(Flag_Round1>50) 
	{
		Flag_Round1=0;
                Flag_Round2=Flag_Round2+1;                
	}
//?????
	if(Flag_Round1==0){
          if(DirErr_D_P <= 0 && DirErr_D_P >= 1) {DirErr_D_P = 0 ;} //???????? ???????
   
          if(DirectionErr[0]<0.2&&DirectionErr[0]>-0.2){
             //????????????????��?��??????????????????
            DirectionOut=(Direction_P-500)*DirectionErr[0]+(DirErr_D_P*DirectionErr_dot[1]+(1-DirErr_D_P)*DirectionErr_dot[2])*(Direction_D-2000);}  
            
          else{//???????????
            DirectionOut=DirectionErr[0]*Direction_P+(DirErr_D_P*DirectionErr_dot[1]+(1-DirErr_D_P)*DirectionErr_dot[2])*Direction_D;}
        }
	else
        {   DirectionOut=DirectionErr[0]*700+DirectionErr_dot[0]*1600;
            Flag_Round1=Flag_Round1+1;
        }  //??????????????????��?��??????????????????	  

}        

*/

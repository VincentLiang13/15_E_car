#include "include.h"
#include "Param.h"


Param param;


void Scope()//Ð´ÔÚÖÐ¶Ï yes   £¨or main£©
{

    ANO_DT_send_int16((short)param.g_fExpectSpeed,
                      (short)param.RighPWM_F,
                      (short)param.SpeedPwm,
                      (short)param.g_fSpeedFilter,
                      0,
                      0,
                      0,
                      0);

}


void ParamOut()
{
  char txt[16];
      
  while(1)
      { 
          printf("\r\n/ExSp %5d \r\n ",(int32)param.g_fExpectSpeed);
          sprintf(txt," ExSp:%5d\r\n",(int32)param.g_fExpectSpeed);
          OLED_P8x16Str(0,0,(uint8_t*)txt);
          /*
          printf("\r\n/Lpwm %5d \r\n ",param.LeftPWM_F);
          sprintf(txt," Lpwm:%5d\r\n",param.LeftPWM_F);
          OLED_P8x16Str(0,0,(uint8_t*)txt);
          */
          printf("\r\n/Rpwm %5d \r\n ",param.RighPWM_F);
          sprintf(txt," Rpwm:%5d\r\n",param.RighPWM_F);
          OLED_P8x16Str(0,2,(uint8_t*)txt);
          
         printf("\r\n/SpPwm %5d \r\n ",(int32)param.SpeedPwm);
          sprintf(txt," SpPwm:%5d\r\n",(int32)param.SpeedPwm);
          OLED_P8x16Str(0,4,(uint8_t*)txt);
          
          printf("\r\n/Speed %5d \r\n ",(int32)param.g_fSpeedFilter);
          sprintf(txt," Speed:%5d\r\n",(int32)param.g_fSpeedFilter);
          OLED_P8x16Str(0,6,(uint8_t*)txt);
      }
}
#ifndef PARAM_H
#define PARAM_H

#include "Speed.h"
#include "Direction.h"

void ParamOut(void);

typedef struct
{       
    int32 LeftPWM_F;
    int32 RighPWM_F;
    
    float g_fSpeedFilter; //real speed + filter
    float g_fExpectSpeed; //
    
    float SpeedPwm;  //ËÙ¶ÈpwmÊä³ö
  
}Param;

extern Param param;


#endif
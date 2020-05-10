#ifndef PARAM_H
#define PARAM_H

#include "Speed.h"
#include "Direction.h"
#include "ANO_DT.h"

void ParamOut(void);

//示波器结构体
typedef struct
{       
    int32 LeftPWM_F;
    int32 RighPWM_F;
    
    float g_fSpeedFilter; //real speed + filter
    float g_fExpectSpeed; //
    
    float SpeedPwm;  
  
}Param;

extern Param param;

void Scope(void);

#endif
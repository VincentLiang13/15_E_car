/*------------------------------------------------------------------------------------------------------
 * Name         : parallel obj
 * Function     : 寻迹
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : 努力努力再努力
--------------------------------------------------------------------------------------------------------*/

#include "include.h" 
//user define

//主函数
void main(void)
{   
    PLL_Init(PLL180);         //初始化PLL为180M 
	
	/* 设置中断优先级组  0: 0个抢占优先级16位个子优先级 
	* 1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级 
	* 3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级
	*/
	/* 配置优先级组 2: 4个抢占优先级 4个子优先级 */
    NVIC_SetPriorityGrouping(0x07 - 2); 

    LED_Init();
    KEY_Init();
    UART_Init(UART4, 115200);
    OLED_Init();
    
    ADC_Init(ADC0);
    ADC_Init(ADC1);
    
    FTM_PwmInit(FTM0, FTM_CH4, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH5, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH6, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH7, 12000, 0);
    
    FTM_ABInit(FTM1);
    FTM_ABInit(FTM2);
    
    SpeeedPID_Init();  

    OLED_P8x16Str(5,0,(uint8_t*)"Go for it!"); 
    delayms(3000);    
    OLED_CLS();
    
    PIT_Init_PID();//使能pit中断
    
    //Test_Motor();
//------------------------------------------------------------------------------------------------    
//测试ADC端口  检测电压打印并显示 在include.h 宏定义选择OLED或TFT1.8  
//注意电压不要大于3.3V  悬空不等于接地
//------------------------------------------------------------------------------------------------    
      
    //ParamOut();//printf 打印数据

    while(1)
    {
        ;
    }
}

/*------------------------------------------------------------------------------------------------------
 * Name         : parallel obj
 * Function     : Ѱ��
 * Author       : Vincent.Liang
 * Date         : 2020/5/9
 * Notes        : Ŭ��Ŭ����Ŭ��
--------------------------------------------------------------------------------------------------------*/

#include "include.h" 
//user define

//������
void main(void)
{   
    PLL_Init(PLL180);         //��ʼ��PLLΪ180M 
	
	/* �����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ� 
	* 1: 2����ռ���ȼ� 8�������ȼ� 2: 4����ռ���ȼ� 4�������ȼ� 
	* 3: 8����ռ���ȼ� 2�������ȼ� 4: 16����ռ���ȼ� 0�������ȼ�
	*/
	/* �������ȼ��� 2: 4����ռ���ȼ� 4�������ȼ� */
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
    
    PIT_Init_PID();//ʹ��pit�ж�
    
    //Test_Motor();
//------------------------------------------------------------------------------------------------    
//����ADC�˿�  ����ѹ��ӡ����ʾ ��include.h �궨��ѡ��OLED��TFT1.8  
//ע���ѹ��Ҫ����3.3V  ���ղ����ڽӵ�
//------------------------------------------------------------------------------------------------    
      
    //ParamOut();//printf ��ӡ����

    while(1)
    {
        ;
    }
}
/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������K60���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ�FTM0���PWM�źſ��Ƶ������
��ע�����
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_MOTOR.h"


/*------------------------------------------------------------------------------------------------------
����    ����Test_Motor
����    �ܡ�FTM0���PWM�źſ��Ƶ������
����    ����num  :  ������  ��PTC1 �� PTC2���Ƶ��1  ��PTC3 �� PTC4���Ƶ��2
���� �� ֵ��duty �� ռ�ձ�    ��Χ -FTM_PRECISON  �� +FTM_PRECISON
��ʵ    ����Test_Motor(); //���Ե��
��ע�����
--------------------------------------------------------------------------------------------------------*/
void MOTOR_Ctrl(uint8_t num, short duty)
{
    if(abs(duty) > FTM_PRECISON)     //��ֹռ�ձȸ���
    {
        duty = 0;
    }
    switch(num)
    {
      case 1:
        if(duty > 0)
        {
            FTM_PwmDuty(FTM0, FTM_CH0, duty);
            FTM_PwmDuty(FTM0, FTM_CH1, 0);
        }
        else
        {
            FTM_PwmDuty(FTM0, FTM_CH0, 0);
            FTM_PwmDuty(FTM0, FTM_CH1, -duty);
        }
        break;
        
      case 2:
        if(duty > 0)
        {
            FTM_PwmDuty(FTM0, FTM_CH2, duty);
            FTM_PwmDuty(FTM0, FTM_CH3, 0);
        }
        else
        {
            FTM_PwmDuty(FTM0, FTM_CH2, 0);
            FTM_PwmDuty(FTM0, FTM_CH3, -duty);
        }
        break;
    }

}
/*------------------------------------------------------------------------------------------------------
����    ����Test_Motor
����    �ܡ�FTM0���PWM�źſ��Ƶ������
����    ������
���� �� ֵ����
��ʵ    ����Test_Motor(); //���Ե��
��ע�����
#define FTM0_CH4    PTD4//PTA7        //PTD4��PTA7
#define FTM0_CH5    PTD5//PTD5        //PTD5��PTA0            PTA0��Ҫ�ã���JLINK��ͻ��
#define FTM0_CH6    PTD6//PTD6        //PTD6��PTA1            PTA1��Ҫ�ã���JLINK��ͻ��
#define FTM0_CH7    PTD7//PTD7        //PTD7��PTA2            PTA2��Ҫ�ã���JLINK��ͻ��


--------------------------------------------------------------------------------------------------------*/
void Test_Motor(void)
{
//    LED_Init();
//    KEY_Init();
//    UART_Init(UART4, 115200);
//    

    FTM_PwmInit(FTM0, FTM_CH4, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH5, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH6, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH7, 12000, 0);
    
//    FTM_ABInit(FTM1);
//    FTM_ABInit(FTM2);
//    
//    OLED_Init();
//    OLED_P8x16Str(5,0,(uint8_t*)"LQ ENC Test"); 
 /*  
    printf("����������� \n");
    printf("��һ��ʹ�õ�ͬѧ������֮ǰ�����ʾ�����۲�һ�²��� \n");
    printf("����ʹ��12KHz��PWM�źſ��� \n");
    printf("����K0����������� \n");
    printf("����K1���������ǰ���� \n");
    printf("����K2������������� \n");
 */   


//            LED_Reverse(3); 
      while(1)
      {
        
      FTM_PwmDuty(FTM0, FTM_CH4, 400);
      FTM_PwmDuty(FTM0, FTM_CH5, 0);
      FTM_PwmDuty(FTM0, FTM_CH6, 0);
      FTM_PwmDuty(FTM0, FTM_CH7, 250);
      
      }      
//    while(1)
//    {            
//        //delayms(100);   
//        
//        char txt[16];
//        short speed1, speed2;
//        
//        speed1 = FTM_ABGet(FTM1);
//        speed2 = FTM_ABGet(FTM2);
//        
//        printf("\r\n/ENC1 %5d \r\n ",speed1);
//        sprintf(txt,"enc1:%5d ",speed1);
//        OLED_P8x16Str(20,2,(uint8_t*)txt);
//        
//        printf("\r\n/ENC2 %5d \r\n ",speed2);
//        sprintf(txt,"enc2:%5d ",speed2);
//        OLED_P8x16Str(20,4,(uint8_t*)txt);
//        
//        delayms(100);    
//    }

}


/*------------------------------------------------------------------------------------------------------
����    ����Test_Enc
����    �ܡ������������빦��
����    ������
���� �� ֵ����
��ʵ    ����Test_Enc(); //�����������빦��
��ע�����
--------------------------------------------------------------------------------------------------------*/
#ifdef LQ_OLED
void Test_Enc(void)
{
    LED_Init();
    KEY_Init();
    UART_Init(UART4, 115200);
    
    FTM_PwmInit(FTM0, FTM_CH0, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH1, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH2, 12000, 0);
    FTM_PwmInit(FTM0, FTM_CH3, 12000, 0);
    
    FTM_ABInit(FTM1);
    FTM_ABInit(FTM2);
    
    OLED_Init();
    OLED_P8x16Str(5,0,(uint8_t*)"LQ ENC Test"); 
    
    while(1)
    {
    
      MOTOR_Ctrl(1, 300);     //���õ��1��ת��
      MOTOR_Ctrl(2, 300);     //���õ��2��ת��
      
      char txt[16];
      short speed1, speed2;
      
      speed1 = FTM_ABGet(FTM1);
      speed2 = FTM_ABGet(FTM2);
      
      printf("\r\n/ENC1 %5d \r\n ",speed1);
      sprintf(txt,"enc1:%5d ",speed1);
      OLED_P8x16Str(20,2,(uint8_t*)txt);
      
      printf("\r\n/ENC2 %5d \r\n ",speed2);
      sprintf(txt,"enc2:%5d ",speed2);
      OLED_P8x16Str(20,4,(uint8_t*)txt);
      
      delayms(100);    
    }   
    
}
    
    
    


#else

void Test_Enc(void)
{
    LED_Init();
    KEY_Init();
    UART_Init(UART4, 115200);
    FTM_ABInit(FTM1);
    FTM_ABInit(FTM2);
    printf("��������������� \n");

    TFTSPI_Init(1);                 //LCD��ʼ��  0:����  1������
    TFTSPI_CLS(u16BLUE);
    
    TFTSPI_P8X16Str(2, 0, "LQ ENC Test", u16RED, u16BLUE);
    
    char txt[16]; 
    short speed1, speed2;
    while(1)
    {
        speed1 = FTM_ABGet(FTM1);
        speed2 = FTM_ABGet(FTM2);
        
        printf("\r\n/ENC1 %5d \r\n ",speed1);
        sprintf(txt,"enc1:%5d ",speed1);
        TFTSPI_P8X16Str(2, 2, txt, u16RED, u16BLUE);
        
        printf("\r\n/ENC2 %5d \r\n ",speed2);
        sprintf(txt,"enc2:%5d ",speed2);
        TFTSPI_P8X16Str(2, 4, txt, u16RED, u16BLUE);
        
        delayms(200);
    }

}
#endif
/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������K60���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
�������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ����Ź�����
��ע�����
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_WDOG.h"


/*------------------------------------------------------------------------------------------------------
����    ����Test_Wdog
����    �ܡ����Կ��Ź�
����    ������
���� �� ֵ����
��ʵ    ����Test_Wdog(); //���Կ��Ź�
��ע�����
--------------------------------------------------------------------------------------------------------*/
void Test_Wdog(void)
{
    LED_Init();
    
    UART_Init(UART4, 115200);
    
    printf("���Ź��������� \n");
    
    WDOG_SetTime(500);    //����ι��ʱ��
    
    delayms(200); 
    WDOG_Feed();          //ι��
    printf("ι���ɹ� 200ms ����λ \n");
    
    delayms(300);
    WDOG_Feed();          //ι��
    printf("ι���ɹ� 300ms ����λ \n");
    
    delayms(510);
    WDOG_Feed();          //ι��
    printf("ι���ɹ� 510ms ����λ \n");
    
    while(1);


}
/*
 * my_uart.c
 *
 *  Created on: 2019��7��3��
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */
#include "my_system.h"
#include "my_uart.h"
#include "delay.h"
#include "gps.h"
#include "gprs.h"
#include "WDT.h"
#include "headfile.h"


int main(void)
{	
	systemInit();//ϵͳʱ�ӳ�ʼ��80M
	initDelay();//��ʱ��ʼ��  ����SysTick��ʱ��ʵ��

/*--------------------------------LaunchPad ����LED(��)�����ų�ʼ�� Start-----------------------------------*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//����GPIOFʱ��
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
			/*--û��Ҫִ�е�����--*/
    }	
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);//F3����Ϊ���ģʽ
	delay_ms(100); //��ʼ����ʱ
/*---------------------------------LaunchPad ����LED(��)�����ų�ʼ�� End----------------------------------*/
	
		
	UART0Init();//����0��ʼ�� 115200 ��ȡGPS��
	UART1Init();//��ʼ������2 115200 ��������GPRS���� 
	
	clrGPSStruct();//GPS�����ڴ��ʼ��	
	Init_GPRS();	//��ʼ��GPRS
	delay_ms(100); //��ʼ����ʱ
		
	watchDogInit();//��ʼ�����Ź�
	
	while(1)
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);//�ı�LED״̬
		
		parseGpsBuffer();//����GPS����
		disposeGpsBuffer(); //����GPS����
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);//�ı�LED״̬
		delay_ms(2000);
		feedWatchDog();//ι��
	}
}



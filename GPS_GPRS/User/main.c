/*
 * my_uart.c
 *
 *  Created on: 2019年7月3日
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
	systemInit();//系统时钟初始化80M
	initDelay();//延时初始化  采用SysTick定时器实现

/*--------------------------------LaunchPad 板载LED(绿)灯引脚初始化 Start-----------------------------------*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//配置GPIOF时钟
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
			/*--没有要执行的内容--*/
    }	
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);//F3配置为输出模式
	delay_ms(100); //初始化延时
/*---------------------------------LaunchPad 板载LED(绿)灯引脚初始化 End----------------------------------*/
	
		
	UART0Init();//串口0初始化 115200 读取GPS用
	UART1Init();//初始化串口2 115200 用来接收GPRS数据 
	
	clrGPSStruct();//GPS数据内存初始化	
	Init_GPRS();	//初始化GPRS
	delay_ms(100); //初始化延时
		
	watchDogInit();//初始化看门狗
	
	while(1)
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);//改变LED状态
		
		parseGpsBuffer();//解析GPS数据
		disposeGpsBuffer(); //处理GPS数据
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);//改变LED状态
		delay_ms(2000);
		feedWatchDog();//喂狗
	}
}



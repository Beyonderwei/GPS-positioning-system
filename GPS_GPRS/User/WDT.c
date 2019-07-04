/*
 * my_uart.c
 *
 *  Created on: 2019年7月3日
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */
#include "WDT.h"


void watchDogInit(void )
{
	/*--注：WDT0 默认使用的是系统时钟 WDT是二次超时机制，第一次超时，产生中断，第二次超时，则产生复位信号--*/
	
	//初始化看门狗  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0); 
	if(WatchdogLockState(WATCHDOG0_BASE)==true)
	{
			WatchdogUnlock(WATCHDOG0_BASE);
	}
	WatchdogStallEnable(WATCHDOG0_BASE);//设置看门狗调试时暂停
	WatchdogReloadSet(WATCHDOG0_BASE,SysCtlClockGet()*7);//设置看门狗定时器的超时值 7s  相当于2-3次没有通信成功就重启系统
	WatchdogResetEnable(WATCHDOG0_BASE);//使能看门狗复位
	WatchdogEnable(WATCHDOG0_BASE);//使能看门狗
	WatchdogLock(WATCHDOG0_BASE);//锁定看门狗
}



void feedWatchDog(void)
{
	WatchdogUnlock(WATCHDOG0_BASE);//解锁看门狗
	WatchdogReloadSet(WATCHDOG0_BASE,SysCtlClockGet()*7);//：喂狗 -> 设置看门狗定时器的超时值 7s  相当于2-3次没有通信成功就重启系统
	WatchdogLock(WATCHDOG0_BASE);//锁定看门狗
}



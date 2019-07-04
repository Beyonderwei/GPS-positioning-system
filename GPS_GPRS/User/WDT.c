/*
 * my_uart.c
 *
 *  Created on: 2019��7��3��
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */
#include "WDT.h"


void watchDogInit(void )
{
	/*--ע��WDT0 Ĭ��ʹ�õ���ϵͳʱ�� WDT�Ƕ��γ�ʱ���ƣ���һ�γ�ʱ�������жϣ��ڶ��γ�ʱ���������λ�ź�--*/
	
	//��ʼ�����Ź�  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0); 
	if(WatchdogLockState(WATCHDOG0_BASE)==true)
	{
			WatchdogUnlock(WATCHDOG0_BASE);
	}
	WatchdogStallEnable(WATCHDOG0_BASE);//���ÿ��Ź�����ʱ��ͣ
	WatchdogReloadSet(WATCHDOG0_BASE,SysCtlClockGet()*7);//���ÿ��Ź���ʱ���ĳ�ʱֵ 7s  �൱��2-3��û��ͨ�ųɹ�������ϵͳ
	WatchdogResetEnable(WATCHDOG0_BASE);//ʹ�ܿ��Ź���λ
	WatchdogEnable(WATCHDOG0_BASE);//ʹ�ܿ��Ź�
	WatchdogLock(WATCHDOG0_BASE);//�������Ź�
}



void feedWatchDog(void)
{
	WatchdogUnlock(WATCHDOG0_BASE);//�������Ź�
	WatchdogReloadSet(WATCHDOG0_BASE,SysCtlClockGet()*7);//��ι�� -> ���ÿ��Ź���ʱ���ĳ�ʱֵ 7s  �൱��2-3��û��ͨ�ųɹ�������ϵͳ
	WatchdogLock(WATCHDOG0_BASE);//�������Ź�
}



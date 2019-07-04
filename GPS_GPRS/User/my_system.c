
/*
 * my_system.c
 *
 *  Created on: 2019��5��22��
 *      Author: Beyonderwei
 */

#include "my_system.h"

void 
systemInit(void )
{
	/*--�����������㵥Ԫ--*/
	FPUEnable();
	FPULazyStackingEnable();
	
	/*-- ʱ�����ã�2.5��Ƶ��80Mhz��ϵͳʱ��ʹ��PLL��ʹ���ⲿ��ʱ��Դ���ⲿ����Ϊ16MHz --*/
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL  | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

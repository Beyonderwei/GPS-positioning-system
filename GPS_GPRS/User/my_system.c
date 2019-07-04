
/*
 * my_system.c
 *
 *  Created on: 2019年5月22日
 *      Author: Beyonderwei
 */

#include "my_system.h"

void 
systemInit(void )
{
	/*--开启浮点运算单元--*/
	FPUEnable();
	FPULazyStackingEnable();
	
	/*-- 时钟配置，2.5分频，80Mhz，系统时钟使用PLL，使用外部主时钟源，外部晶振为16MHz --*/
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL  | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

#ifndef __HEADFILE_H__
#define __HEADFILE_H__


#define line_01_id 1	//设备ID




/*--中断的优先级设置，值越小，优先级越高--*/
#define  USER_INT0  0x00   //
#define  USER_INT1  0x20   //
#define  USER_INT2  0x40   //
#define  USER_INT3  0x60   //
#define  USER_INT4  0x80   //
#define  USER_INT5  0xA0   //
#define  USER_INT6  0xD0   //
#define  USER_INT7  0xE0   //



#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "pin_map.h"
#include "pwm.h"
#include "rom.h"
#include "sysctl.h"
#include "uart.h"
#include "interrupt.h"
#include "timer.h"
#include "hw_gpio.h"
#include "eeprom.h"
#include "watchdog.h"


#include "uartstdio.h"


#endif

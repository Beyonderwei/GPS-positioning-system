/* Copyright (C) 2015 Kristian Sloth Lauszus. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Sloth Lauszus
 Web      :  http://www.lauszus.com
 e-mail   :  lauszus@gmail.com
*/

#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void initDelay(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
uint32_t millis(void);
uint32_t micros(void);
void Delay_Ms(uint32_t x);
void Delay_Us(uint32_t x);

void delay_ms(uint32_t x);
void delay_us(uint32_t x);
	
#ifdef __cplusplus
}
#endif

#endif

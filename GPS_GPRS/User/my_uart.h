/*
 * my_uart.h
 *
 *  Created on: 2019Äê5ÔÂ21ÈÕ
 *      Author: Beyonderwei
 */
 
#ifndef __MY_UART_H__
#define __MY_UART_H__

#include "headfile.h"

void UART0Init(void);
void UART0IntHandler(void);
void UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count);

void UART1Init(void);
void UART1_IRQHandler(void);
void UART1_Send(uint8_t *pui8Buffer, uint32_t ui32Count);

#endif

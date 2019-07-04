/*
 * my_uart.c
 *
 *  Created on: 2019年7月3日
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */
#include "my_uart.h"
#include "gps.h"


void 
UART0Init(void)//串口0初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//使能UART外设
  GPIOPinConfigure(GPIO_PA0_U0RX);//GPIO模式配置 PA0--RX PA1--TX 
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(0, 115200, 16000000);
  //UART协议配置 波特率115200 8位 1停止位  无校验位	
  //UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断	//禁用后接收1位就产生中断	
  UARTFIFODisable(UART0_BASE);//使能UART0中断	IntEnable(INT_UART0);	
  UARTIntEnable(UART0_BASE,UART_INT_RX);//使能UART0接收中断		
  UARTIntRegister(UART0_BASE,UART0IntHandler);//UART中断地址注册	
  IntPrioritySet(INT_UART0, USER_INT1);//中断优先级设置
}



//*****************************************************************************
//
// The UART interrupt handler.
// UART0 用来解析 GPS 数据
//
//*****************************************************************************
void 
UART0IntHandler(void)//UART0中断函数
{
	uint8_t temp;
  //获取中断标志 原始中断状态 不屏蔽中断标志		
  uint32_t flag = UARTIntStatus(UART0_BASE,1);
  //清除中断标志	
  UARTIntClear(UART0_BASE,flag);		
  //判断FIFO是否还有数据		
//  while(UARTCharsAvail(UART0_BASE))		
//  {
			temp = UARTCharGet(UART0_BASE);
			if(temp == '$')
		{
			point1 = 0;	
		}
		

			USART_RX_BUF[point1++] = temp;

		if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
		{
			if(temp == '\n')									   
			{
				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
				memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//保存数据到 GPS_Buffer
				Save_Data.isGetData = true;  //当 Res 为换行符的时候，说明，一帧数据接收完成
				point1 = 0;
				memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空串口缓存数组
			}
		}
	
		if(point1 >= USART_REC_LEN)
		{
			point1 = USART_REC_LEN;
		}
//  }
}



//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
}



/*-----------------------串口1 的相关函数--------------------------*/
/*--串口1用来与GPRS进行通信--*/
unsigned char uart1_RX_buffer[200]={};
unsigned char uart1_RX_buffer_size=200;
unsigned char GPRS_receve_cnt=0;
/***********************************************************
@函数名：UART1_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口1数据接收
*************************************************************/

void UART1_IRQHandler(void)//UART1中断函数
{
  uint32_t flag = UARTIntStatus(UART1_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志
  UARTIntClear(UART1_BASE,flag);//清除中断标志
	if(GPRS_receve_cnt>=198) GPRS_receve_cnt=198;
	uart1_RX_buffer[GPRS_receve_cnt++]=UARTCharGet(UART1_BASE);
}


/***********************************************************
@函数名：USART1_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口1发送N个字节数据
*************************************************************/
void UART1_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART1_BASE, *pui8Buffer++);
  }
}
/***********************************************************
@函数名：ConfigureUART1
@入口参数：无
@出口参数：无
功能描述：串口1配置
***********************************************************/
void UART1Init(void)//串口1初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能GPIO外设
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//使能UART外设
  GPIOPinConfigure(GPIO_PB0_U1RX);//GPIO模式配置 PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PB1_U1TX);
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
  UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(1,115200,16000000);
  UARTFIFODisable(UART1_BASE);//使能UART1中断	
  UARTIntEnable(UART1_BASE,UART_INT_RX);//使能UART1接收中断		
  UARTIntRegister(UART1_BASE,UART1_IRQHandler);//UART1中断地址注册	
  IntPrioritySet(INT_UART1, USER_INT0);
}



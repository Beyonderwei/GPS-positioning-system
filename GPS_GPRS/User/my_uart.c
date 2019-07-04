/*
 * my_uart.c
 *
 *  Created on: 2019��7��3��
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */
#include "my_uart.h"
#include "gps.h"


void 
UART0Init(void)//����0��ʼ��
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//ʹ��UART����
  GPIOPinConfigure(GPIO_PA0_U0RX);//GPIOģʽ���� PA0--RX PA1--TX 
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO��UARTģʽ����
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(0, 115200, 16000000);
  //UARTЭ������ ������115200 8λ 1ֹͣλ  ��У��λ	
  //UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�	//���ú����1λ�Ͳ����ж�	
  UARTFIFODisable(UART0_BASE);//ʹ��UART0�ж�	IntEnable(INT_UART0);	
  UARTIntEnable(UART0_BASE,UART_INT_RX);//ʹ��UART0�����ж�		
  UARTIntRegister(UART0_BASE,UART0IntHandler);//UART�жϵ�ַע��	
  IntPrioritySet(INT_UART0, USER_INT1);//�ж����ȼ�����
}



//*****************************************************************************
//
// The UART interrupt handler.
// UART0 �������� GPS ����
//
//*****************************************************************************
void 
UART0IntHandler(void)//UART0�жϺ���
{
	uint8_t temp;
  //��ȡ�жϱ�־ ԭʼ�ж�״̬ �������жϱ�־		
  uint32_t flag = UARTIntStatus(UART0_BASE,1);
  //����жϱ�־	
  UARTIntClear(UART0_BASE,flag);		
  //�ж�FIFO�Ƿ�������		
//  while(UARTCharsAvail(UART0_BASE))		
//  {
			temp = UARTCharGet(UART0_BASE);
			if(temp == '$')
		{
			point1 = 0;	
		}
		

			USART_RX_BUF[point1++] = temp;

		if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
		{
			if(temp == '\n')									   
			{
				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
				memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//�������ݵ� GPS_Buffer
				Save_Data.isGetData = true;  //�� Res Ϊ���з���ʱ��˵����һ֡���ݽ������
				point1 = 0;
				memset(USART_RX_BUF, 0, USART_REC_LEN);      //��մ��ڻ�������
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



/*-----------------------����1 ����غ���--------------------------*/
/*--����1������GPRS����ͨ��--*/
unsigned char uart1_RX_buffer[200]={};
unsigned char uart1_RX_buffer_size=200;
unsigned char GPRS_receve_cnt=0;
/***********************************************************
@��������UART1_IRQHandler
@��ڲ�������
@���ڲ�������
��������������1���ݽ���
*************************************************************/

void UART1_IRQHandler(void)//UART1�жϺ���
{
  uint32_t flag = UARTIntStatus(UART1_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־
  UARTIntClear(UART1_BASE,flag);//����жϱ�־
	if(GPRS_receve_cnt>=198) GPRS_receve_cnt=198;
	uart1_RX_buffer[GPRS_receve_cnt++]=UARTCharGet(UART1_BASE);
}


/***********************************************************
@��������USART1_Send
@��ڲ�����uint8_t *pui8Buffer, uint32_t ui32Count
@���ڲ�������
��������������1����N���ֽ�����
*************************************************************/
void UART1_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//����N���ֽڳ��ȵ�����
{
  while(ui32Count--)
  {
    UARTCharPut(UART1_BASE, *pui8Buffer++);
  }
}
/***********************************************************
@��������ConfigureUART1
@��ڲ�������
@���ڲ�������
��������������1����
***********************************************************/
void UART1Init(void)//����1��ʼ��
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ��GPIO����
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//ʹ��UART����
  GPIOPinConfigure(GPIO_PB0_U1RX);//GPIOģʽ���� PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PB1_U1TX);
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO��UARTģʽ����
  UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(1,115200,16000000);
  UARTFIFODisable(UART1_BASE);//ʹ��UART1�ж�	
  UARTIntEnable(UART1_BASE,UART_INT_RX);//ʹ��UART1�����ж�		
  UARTIntRegister(UART1_BASE,UART1_IRQHandler);//UART1�жϵ�ַע��	
  IntPrioritySet(INT_UART1, USER_INT0);
}



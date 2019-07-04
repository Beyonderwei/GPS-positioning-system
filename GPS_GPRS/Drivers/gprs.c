/*
 * gprs.c
 *
 *  Created on: 2019��7��3��
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */

#include "gprs.h"
#include "delay.h"
#include <string.h>


extern unsigned char GPSData[200];  //�����������ת����Json��ʽ��GPS�����ַ���
extern unsigned char uart1_RX_buffer[200];  //���յ���GPRS���ص�����
extern unsigned char uart1_RX_buffer_size;  //uart1_RX_buffer��С
extern unsigned char GPRS_receve_cnt;  //����GPRS���ݵļ���ֵ

/**
 *@function ��ʼ��GPRS��������� ��������ʱȥgprs.h���޸�
 *@param none
 *@return none
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
void Init_GPRS(void)
{
	UART1_Send((unsigned char *)CHECK_CARD,11);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//�ȴ���ֱ������ֵ���Ϊ������GPRS����OK�ַ���
	{
		/*--û����Ҫִ�е�����--*/
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //����0��ӡ GPRS ���ص�����
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //�������1���ջ������� ׼����һ�ν���
	GPRS_receve_cnt=0;	//����ֵ����
	
	
	UART1_Send((unsigned char *)SET_APN,26);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//�ȴ���ֱ������ֵ���Ϊ������GPRS����OK�ַ���
	{
		/*--û����Ҫִ�е�����--*/
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //����0��ӡ GPRS ���ص�����
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //�������1���ջ������� ׼����һ�ν���
	GPRS_receve_cnt=0;	//����ֵ����
	
	
	UART1_Send((unsigned char *)ACTIVATE_NETWORK,18);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//�ȴ���ֱ������ֵ���Ϊ������GPRS����OK�ַ���
	{
		if(GPRSReturnERROR((const char *)uart1_RX_buffer))	//�������ERROR ˵���Ѿ�������� ֱ������
			break;
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //����0��ӡ GPRS ���ص�����
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //�������1���ջ������� ׼����һ�ν���
	GPRS_receve_cnt=0;	//����ֵ����
	
	
	UART1_Send((unsigned char *)SET_HTTP_URL,41);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//�ȴ���ֱ������ֵ���Ϊ������GPRS����OK�ַ���
	{
		/*--û����Ҫִ�е�����--*/
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //����0��ӡ GPRS ���ص�����
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //�������1���ջ������� ׼����һ�ν���
	GPRS_receve_cnt=0;	//����ֵ����
	
	
	UART1_Send((unsigned char *)SET_port,24);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//�ȴ���ֱ������ֵ���Ϊ������GPRS����OK�ַ���
	{
		/*--û����Ҫִ�е�����--*/
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //����0��ӡ GPRS ���ص�����
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //�������1���ջ������� ׼����һ�ν���
	GPRS_receve_cnt=0;	//����ֵ����
}





/**
 *@function �� json ��ʽ�� GPS ����ͨ��GPRS  GPRS post ���ݵ�������
 *@param �洢 GPS ���ݵ��ַ�����
 *@return  none
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
void postGPSdata(void)
{
	UART1_Send((unsigned char *)START_HTTP_CONNECTION,24);	 //����HTTP����
	delay_ms(50);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//�ȴ���ֱ������ֵ���Ϊ������GPRS����OK�ַ���
	{
		if(GPRSReturnERROR((const char *)uart1_RX_buffer))	//�������ERROR ���·��ͷ������� ��������
			goto end;
			//UART1_Send((unsigned char *)START_HTTP_CONNECTION,24);	 //����HTTP����
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //����0��ӡ GPRS ���ص�����
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //�������1���ջ������� ׼����һ�ν���
	GPRS_receve_cnt=0;	//����ֵ����
	
	
	UART1_Send((unsigned char *)POST_DATA,24);	//������������
	delay_ms(50);
	while((strchr((const char *)uart1_RX_buffer,'>')==NULL))  //�ȴ�ֱ������ '>' ��ʾ���Է���������
	{
		if(GPRSReturnERROR((const char *)uart1_RX_buffer))	//�������ERROR ���·��ͷ������� ��������
			goto end;
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //����0��ӡ GPRS ���ص�����
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //�������1���ջ������� ׼����һ�ν���
	GPRS_receve_cnt=0;	//����ֵ����	
	
	
	UART1_Send(GPSData,120);	//��������  �������ݺ����ӻ��Զ��ر�
	UART1_Send((unsigned char *)"\r\n",3);
	delay_ms(1000);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//�ȴ���ֱ������ֵ���Ϊ������GPRS����OK�ַ���
	{
		if(GPRSReturnERROR((const char *)uart1_RX_buffer))	//�������ERROR ���·��ͷ������� ��������
			goto end;
	}
	
	end:
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //����0��ӡ GPRS ���ص�����
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //�������1���ջ������� ׼����һ�ν���
	GPRS_receve_cnt=0;	//����ֵ����
	
}



/**
 *@function �ж���������ַ����Ƿ��� OK �Ӵ�
 *@param Ҫ�жϵ�Ŀ������
 *@return  ���з���1  �����з���0
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
char GPRSReturnOK(const char * data_buffer)
{
	if(strstr(data_buffer,"OK")!=NULL)
		return 1;
	else 
		return 0;
}


/**
 *@function �ж���������ַ����Ƿ��� ERROR �Ӵ�
 *@param Ҫ�жϵ�Ŀ������
 *@return  ���з���1  �����з���0
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
char GPRSReturnERROR(const char * data_buffer)
{
	if(strstr(data_buffer,"ERROR")!=NULL)
		return 1;
	else 
		return 0;
}

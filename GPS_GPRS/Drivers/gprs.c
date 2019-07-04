/*
 * gprs.c
 *
 *  Created on: 2019年7月3日
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */

#include "gprs.h"
#include "delay.h"
#include <string.h>


extern unsigned char GPSData[200];  //用来存放最终转化成Json格式的GPS数据字符串
extern unsigned char uart1_RX_buffer[200];  //接收到的GPRS返回的数据
extern unsigned char uart1_RX_buffer_size;  //uart1_RX_buffer大小
extern unsigned char GPRS_receve_cnt;  //接收GPRS数据的计数值

/**
 *@function 初始化GPRS的相关配置 更改配置时去gprs.h中修改
 *@param none
 *@return none
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
void Init_GPRS(void)
{
	UART1_Send((unsigned char *)CHECK_CARD,11);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//等待，直到返回值结果为１　即GPRS返回OK字符串
	{
		/*--没有需要执行的内容--*/
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //串口0打印 GPRS 返回的数据
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //清除串口1接收缓存数组 准备下一次接收
	GPRS_receve_cnt=0;	//计数值清零
	
	
	UART1_Send((unsigned char *)SET_APN,26);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//等待，直到返回值结果为１　即GPRS返回OK字符串
	{
		/*--没有需要执行的内容--*/
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //串口0打印 GPRS 返回的数据
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //清除串口1接收缓存数组 准备下一次接收
	GPRS_receve_cnt=0;	//计数值清零
	
	
	UART1_Send((unsigned char *)ACTIVATE_NETWORK,18);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//等待，直到返回值结果为１　即GPRS返回OK字符串
	{
		if(GPRSReturnERROR((const char *)uart1_RX_buffer))	//如果返回ERROR 说明已经激活过了 直接跳过
			break;
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //串口0打印 GPRS 返回的数据
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //清除串口1接收缓存数组 准备下一次接收
	GPRS_receve_cnt=0;	//计数值清零
	
	
	UART1_Send((unsigned char *)SET_HTTP_URL,41);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//等待，直到返回值结果为１　即GPRS返回OK字符串
	{
		/*--没有需要执行的内容--*/
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //串口0打印 GPRS 返回的数据
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //清除串口1接收缓存数组 准备下一次接收
	GPRS_receve_cnt=0;	//计数值清零
	
	
	UART1_Send((unsigned char *)SET_port,24);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//等待，直到返回值结果为１　即GPRS返回OK字符串
	{
		/*--没有需要执行的内容--*/
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //串口0打印 GPRS 返回的数据
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //清除串口1接收缓存数组 准备下一次接收
	GPRS_receve_cnt=0;	//计数值清零
}





/**
 *@function 将 json 格式的 GPS 数据通过GPRS  GPRS post 数据到服务器
 *@param 存储 GPS 数据的字符数组
 *@return  none
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
void postGPSdata(void)
{
	UART1_Send((unsigned char *)START_HTTP_CONNECTION,24);	 //建立HTTP连接
	delay_ms(50);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//等待，直到返回值结果为１　即GPRS返回OK字符串
	{
		if(GPRSReturnERROR((const char *)uart1_RX_buffer))	//如果返回ERROR 重新发送发送请求 建立连接
			goto end;
			//UART1_Send((unsigned char *)START_HTTP_CONNECTION,24);	 //建立HTTP连接
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //串口0打印 GPRS 返回的数据
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //清除串口1接收缓存数组 准备下一次接收
	GPRS_receve_cnt=0;	//计数值清零
	
	
	UART1_Send((unsigned char *)POST_DATA,24);	//发送数据配置
	delay_ms(50);
	while((strchr((const char *)uart1_RX_buffer,'>')==NULL))  //等待直到出现 '>' 表示可以发送数据了
	{
		if(GPRSReturnERROR((const char *)uart1_RX_buffer))	//如果返回ERROR 重新发送发送请求 建立连接
			goto end;
	}
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //串口0打印 GPRS 返回的数据
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //清除串口1接收缓存数组 准备下一次接收
	GPRS_receve_cnt=0;	//计数值清零	
	
	
	UART1_Send(GPSData,120);	//发送数据  发送数据后连接会自动关闭
	UART1_Send((unsigned char *)"\r\n",3);
	delay_ms(1000);
	while(GPRSReturnOK((const char *)uart1_RX_buffer)!=1)	//等待，直到返回值结果为１　即GPRS返回OK字符串
	{
		if(GPRSReturnERROR((const char *)uart1_RX_buffer))	//如果返回ERROR 重新发送发送请求 建立连接
			goto end;
	}
	
	end:
	//UART0Send(uart1_RX_buffer,uart1_RX_buffer_size);  //串口0打印 GPRS 返回的数据
	memset(uart1_RX_buffer,0,uart1_RX_buffer_size);  //清除串口1接收缓存数组 准备下一次接收
	GPRS_receve_cnt=0;	//计数值清零
	
}



/**
 *@function 判断所传入的字符串是否含有 OK 子串
 *@param 要判断的目标数组
 *@return  含有返回1  不含有返回0
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
 *@function 判断所传入的字符串是否含有 ERROR 子串
 *@param 要判断的目标数组
 *@return  含有返回1  不含有返回0
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

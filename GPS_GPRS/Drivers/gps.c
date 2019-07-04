/*
 * gps.c
 *
 *  Created on: 2019年7月3日
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */
 
#include "gps.h"
#include "my_uart.h"
#include "gprs.h"

_SaveData Save_Data;  //存放GPS数据的结构体
unsigned char GPSData[200]={0};  //用来存放最终转化成Json格式的GPS数据的字符串



char rxdatabufer;
uint16_t point1 = 0;

uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记	


uint8_t Hand(char *a)                   // 串口命令识别函数
{ 
    if(strstr((char *)USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void ClrGPSBuf(void)                           // 串口缓存清理
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空
  point1 = 0;                    
}

void clrGPSStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
}


void errorLog(void)
{
	
	while (1)
	{
	  	UART0Send((unsigned char *)"ERROR\r\n",8);
	}
}

/**
 *@function 解析GPS数据
 *@param none
 *@return none
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
void parseGpsBuffer(void)
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
//		UART0Send("**************\r\n",17);
//		UART0Send(Save_Data.GPS_Buffer,GPS_Buffer_Length);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog();	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog();	//解析错误
				}
			}
		}
	}
}



/**
 *@function 将GPS数据转换成 json 格式
 *@param 存储 GPS 数据的结构体
 *@return none
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
 
void GPSDataToJson(_SaveData gps_param_struct)
{
	char *outData;
	//创建一个cJSON结构体指针并分配空间，然后赋值给root
	cJSON *root = cJSON_CreateObject();
	
	//在root结构体中创建一个健为"key",值为" value" 的键值对。
	cJSON_AddNumberToObject(root, "id", line_01_id);
	cJSON_AddStringToObject(root, "longitude_type", gps_param_struct.E_W);
	cJSON_AddStringToObject(root, "longitude", gps_param_struct.longitude);
	cJSON_AddStringToObject(root, "latitude_type", gps_param_struct.N_S);
	cJSON_AddStringToObject(root, "latitude", gps_param_struct.latitude);
	cJSON_AddStringToObject(root, "time", gps_param_struct.UTCTime);
	
	/*--得到无格式形式的json字符串，即输出无回车和空格分离的键值对--*/
	outData = cJSON_PrintUnformatted(root);
	//将json 格式数据拷贝到全局数组 GPSData 以便被发送
	strncpy((char *)GPSData, outData, strlen(outData));
	
  /*--释放相关内存--*/
  free(outData);  //释放malloc分配的空间
  cJSON_Delete(root);  //释放cJSON结构体指针
}





/**
*@function 处理GPS数据  判断是否成功  如果成功：1.将数据转化成Json格式 2.通过GPRS发送给服务器
*         注：注释的串口发送数据是调试时使用的。
 *@param none
 *@return none
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
void disposeGpsBuffer(void)
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
//		UART0Send("Save_Data.UTCTime = ",21);
//		UART0Send(Save_Data.UTCTime,UTCTime_Length);
//		UART0Send("\r\n",3);

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			
			GPSDataToJson(Save_Data);//转化GPS数据 并发送给GPRS
				
			postGPSdata();//将数据发送给GPRS GPRS采用POST方式上传数据到服务器
			
			
//			UART0Send("Save_Data.latitude = ",22);
//			UART0Send(Save_Data.latitude,latitude_Length);
//			UART0Send("\r\n",3);


//			UART0Send("Save_Data.N_S = ",17);
//			UART0Send(Save_Data.N_S,N_S_Length);
//			UART0Send("\r\n",3);

//			UART0Send("Save_Data.longitude = ",23);
//			UART0Send(Save_Data.longitude,longitude_Length);
//			UART0Send("\r\n",3);

//			UART0Send("Save_Data.E_W = ",17);
//			UART0Send(Save_Data.E_W,E_W_Length);
//			UART0Send("\r\n",3);
		}
		else
		{
			UART0Send("GPS DATA is not usefull!\r\n",27);
		}
	}
}


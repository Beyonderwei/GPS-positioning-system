/*
 * gps.c
 *
 *  Created on: 2019��7��3��
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */
 
#include "gps.h"
#include "my_uart.h"
#include "gprs.h"

_SaveData Save_Data;  //���GPS���ݵĽṹ��
unsigned char GPSData[200]={0};  //�����������ת����Json��ʽ��GPS���ݵ��ַ���



char rxdatabufer;
uint16_t point1 = 0;

uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;       //����״̬���	


uint8_t Hand(char *a)                   // ��������ʶ����
{ 
    if(strstr((char *)USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void ClrGPSBuf(void)                           // ���ڻ�������
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //���
  point1 = 0;                    
}

void clrGPSStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
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
 *@function ����GPS����
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
					errorLog();	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W

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
					errorLog();	//��������
				}
			}
		}
	}
}



/**
 *@function ��GPS����ת���� json ��ʽ
 *@param �洢 GPS ���ݵĽṹ��
 *@return none
 *@Author: Beyonderwei
 *@Email: beyonderwei@163.com
 */
 
void GPSDataToJson(_SaveData gps_param_struct)
{
	char *outData;
	//����һ��cJSON�ṹ��ָ�벢����ռ䣬Ȼ��ֵ��root
	cJSON *root = cJSON_CreateObject();
	
	//��root�ṹ���д���һ����Ϊ"key",ֵΪ" value" �ļ�ֵ�ԡ�
	cJSON_AddNumberToObject(root, "id", line_01_id);
	cJSON_AddStringToObject(root, "longitude_type", gps_param_struct.E_W);
	cJSON_AddStringToObject(root, "longitude", gps_param_struct.longitude);
	cJSON_AddStringToObject(root, "latitude_type", gps_param_struct.N_S);
	cJSON_AddStringToObject(root, "latitude", gps_param_struct.latitude);
	cJSON_AddStringToObject(root, "time", gps_param_struct.UTCTime);
	
	/*--�õ��޸�ʽ��ʽ��json�ַ�����������޻س��Ϳո����ļ�ֵ��--*/
	outData = cJSON_PrintUnformatted(root);
	//��json ��ʽ���ݿ�����ȫ������ GPSData �Ա㱻����
	strncpy((char *)GPSData, outData, strlen(outData));
	
  /*--�ͷ�����ڴ�--*/
  free(outData);  //�ͷ�malloc����Ŀռ�
  cJSON_Delete(root);  //�ͷ�cJSON�ṹ��ָ��
}





/**
*@function ����GPS����  �ж��Ƿ�ɹ�  ����ɹ���1.������ת����Json��ʽ 2.ͨ��GPRS���͸�������
*         ע��ע�͵Ĵ��ڷ��������ǵ���ʱʹ�õġ�
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
			
			GPSDataToJson(Save_Data);//ת��GPS���� �����͸�GPRS
				
			postGPSdata();//�����ݷ��͸�GPRS GPRS����POST��ʽ�ϴ����ݵ�������
			
			
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


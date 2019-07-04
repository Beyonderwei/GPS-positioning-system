/*
 * gps.h
 *
 *  Created on: 2019��7��3��
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */

#ifndef __GPS_H
#define __GPS_H

#include "headfile.h"


#define USART_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA;         		//����״̬���	



//�������鳤��
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

//GPS���ݽṹ��
typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} _SaveData;


extern char rxdatabufer;
extern uint16_t point1;
extern _SaveData Save_Data;

void ClrGPSBuf(void);
uint8_t Hand(char *a);
void clrGPSStruct(void);

void errorLog(void);
void parseGpsBuffer(void);
void disposeGpsBuffer(void);
void GPSDataToJson(_SaveData gps_param_struct);

#endif


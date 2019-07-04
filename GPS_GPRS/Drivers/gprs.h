/*
 * gprs.h
 *
 *  Created on: 2019��7��3��
 *      Author: Beyonderwei
 *      Email: beyonderwei@163.com
 *      Website: http://beyonderwei.com
 */

#ifndef GPRS__H_
#define GPRS__H_

#include "headfile.h"
#include "gps.h"
#include "my_uart.h"
/*--cJSON hub--*/
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define CHECK_CARD "AT+cpin?\r\n"	//���SIM��
#define SET_APN "AT$MYNETCON=0,APN,cmnet\r\n"		//����APN
#define ACTIVATE_NETWORK "AT$MYNETACT=0,1\r\n"	//��������
#define SET_HTTP_URL "AT+HTTPPARA=url,118.25.50.73/bus_1Post\r\n"		//����http��URL
#define SET_port "AT+HTTPPARA=port,8080\r\n"		//���ö˿�
#define START_HTTP_CONNECTION "AT+HTTPSETUP\r\n"		//����HTTP����
#define POST_DATA "AT+HTTPACTION=2,120,2\r\n"  //2:post���� 20:���ݳ��� 2:Json���ݸ�ʽ
#define HTTP_CLOSE "AT+HTTPCLOSE\r\n"		//�ر�HTTP����


void Init_GPRS(void);
void postGPSdata(void);
char GPRSReturnOK(const char * data_buffer);
char GPRSReturnERROR(const char * data_buffer);
#endif

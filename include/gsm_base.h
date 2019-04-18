
#ifndef __GSM_BASE_H__
#define __GSM_BASE_H__


#ifdef __GSM_BASE_C__
#define __GSM_BASE_EXT__
#else
#define __GSM_BASE_EXT__	extern
#endif

#include "all.h"


//#define Buf1_Max 60 					  //����1���泤��
#define Buf2_Max 200 					  //����2���泤��


/*************  ���ر�������	**************/
//char Uart1_Buf[Buf1_Max];
__GSM_BASE_EXT__ char Uart2_Buf[Buf2_Max];

__GSM_BASE_EXT__ u8 Times,First_Int ,shijian;
__GSM_BASE_EXT__ u16 Heartbeat;

__GSM_BASE_EXT__ vu8 Timer0_start;	//��ʱ��0��ʱ����������
__GSM_BASE_EXT__ vu8 Uart2_Start;	//����2��ʼ��������
__GSM_BASE_EXT__ vu8 Uart2_End;	  //����2�������ݽ���
__GSM_BASE_EXT__ vu8 Heart_beat;		//��������֡��־λ


void set_Heartbeat_time(u8 time);
void clean_delay_uart(void);
u16 recv_gsm_data(u8 *buf, u16 size);


#endif/*__GSM_BASE_H__*/



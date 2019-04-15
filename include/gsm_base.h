
#ifndef __GSM_BASE_H__
#define __GSM_BASE_H__


#ifdef __GSM_BASE_C__
#define __GSM_BASE_EXT__
#else
#define __GSM_BASE_EXT__	extern
#endif


//#define Buf1_Max 60 					  //����1���泤��
#define Buf2_Max 200 					  //����2���泤��


/*************  ���ر�������	**************/
//char Uart1_Buf[Buf1_Max];
__GSM_BASE_EXT__ char Uart2_Buf[Buf2_Max];

__GSM_BASE_EXT__ u8 Times=0,First_Int = 0,shijian=0;
__GSM_BASE_EXT__ u16 Heartbeat=0;

__GSM_BASE_EXT__ vu8 Timer0_start;	//��ʱ��0��ʱ����������
__GSM_BASE_EXT__ vu8 Uart2_Start;	//����2��ʼ��������
__GSM_BASE_EXT__ vu8 Uart2_End;	  //����2�������ݽ���
__GSM_BASE_EXT__ vu8 Heart_beat;		//��������֡��־λ




#endif/*__GSM_BASE_H__*/



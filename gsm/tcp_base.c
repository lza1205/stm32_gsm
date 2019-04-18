

#define __TCP_BASE_C__

#include "all.h"
#include "gsm_base.h"

//TCP ����
const char *__string = "AT+CIPSTART=\"TCP\",\"106.13.62.194\",9015";	//IP��¼������



/*******************************************************************************
* ������ : Connect_Server
* ����   : GPRS���ӷ���������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Connect_Server(char *string)
{
	UART2_SendString("AT+CIPCLOSE=1");	//�ر�����
  Delay_nMs(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//�ر��ƶ�����
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//����PDP������,��������Э��,��������Ϣ
	Second_AT_Command("AT+CGATT=1","OK",2);//����GPRSҵ��
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//����ΪGPRS����ģʽ
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
	if(string != NULL)
	{
		Second_AT_Command((char*)string,"OK",5);
	}
	else
	{
		Second_AT_Command((char*)__string,"OK",5);
	}
	Delay_nMs(100);
	CLR_Buf2();
}
/*******************************************************************************
* ������ : Rec_Server_Data
* ����   : ���շ��������ݺ���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	if(strstr(Uart2_Buf,"+IPD")!=NULL)   		//�������ַ����к���^SISR
	{	
		Heartbeat=0;	//�������֡������
		Heart_beat=0;
		Delay_nMs(100);
		if(strstr(Uart2_Buf,"onled")!=NULL)
		{
			LED1_ON();
		}
		else if(strstr(Uart2_Buf,"offled")!=NULL)
		{
			LED1_OFF();
		}
		UART1_SendString("�յ�����Ϣ��\r\n");
		UART1_SendString(Uart2_Buf);
		CLR_Buf2();
		Heart_beat=1;//����Ӧ�����ݣ����߷������յ�����		
	}
}
/*******************************************************************************
* ������ : tcp_heart_beat
* ����   : ��������Ӧ���������ָ��ú�������������
					1�����յ������������ݺ�Ӧ�������
					2�����������·�����ʱ��ÿ��10�뷢��һ֡���������������������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

static void Send_OK(void)
{
	Second_AT_Command("AT+CIPSEND",">",2);
	Second_AT_Command("OK\32\0","SEND OK",8);;			//�ظ�OK 
}

void tcp_heart_beat(void)
{
	if(Heart_beat)
	{
		Send_OK();
		Heart_beat=0;
	}
}





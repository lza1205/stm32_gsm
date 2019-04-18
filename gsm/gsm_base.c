

#define __GSM_BASE_C__

#include "all.h"


static vu32 __delay_uart = 0;
static vu8 _gsm_data_flg = 0;

/*******************************************************************************
* ������  : USART2_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
	u8 Res=0;
	Res =USART_ReceiveData(USART2);
	Uart2_Buf[First_Int] = Res;  	  //�����յ����ַ����浽������
	First_Int++;                			//����ָ������ƶ�
	if(First_Int > Buf2_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
	{
		First_Int = 0;
	}

	__delay_uart = 50000;
	_gsm_data_flg = 1;
}


void clean_delay_uart(void)
{
	__delay_uart = 0;
	_gsm_data_flg = 0;
}

#define min(x,y) ( (x)>(y)?(y):(x) )

u16 recv_gsm_data(u8 *buf, u16 size)
{
	u32 len = min(First_Int, size);
	
	if(__delay_uart > 0)
	{
		__delay_uart -- ;

		return 0;
	}
	else
	{
		if(_gsm_data_flg)
		{
			_gsm_data_flg = 0;

			memcpy(buf , Uart2_Buf, len);

			return len;
		}
		return 0;
	}
}

vu16 Heartbeat_time = 90;

void set_Heartbeat_time(u8 time)
{
	Heartbeat_time = time;
}

/*******************************************************************************
* ������  : TIM2_IRQHandler
* ����    : ��ʱ��2�ж϶Ϸ�����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
		if(Timer0_start)
		{
			Times++;
		}
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}
		
		Heartbeat++;
		if(Heartbeat > Heartbeat_time)//ÿ90�뷢������֡
		{
			Heartbeat=0;
			Heart_beat=1;
		}
		if(flag)
		{
			LED4_ON(); 
			flag=0;
		}
		else
		{
			LED4_OFF(); 
			flag=1;
		}
	}	
}



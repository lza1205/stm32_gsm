
#define __TCP_3A_C__

#include "tcp_3a.h"


static MACHINE_STATUS_TYPE machine_status = MACHINE_STATUS_CONNECT;

//=========================================

//������
u8 sys_id[5] = {0x01, 0x02, 0x03, 0x04, 0x5};
//�汾
#define VERSION_3A		0x1

void tcp_send(u8 *buf, int len)
{
	Second_AT_Command("AT+CIPSEND",">",2);
	Second_AT_Data((char *)buf,"SEND OK",len, len * 2);;			//�ظ�OK 

	/*
	�����õĺ���
	*/
}

u32 tcp_recv(u8 * buf)
{
	u32 ret = 0;
	u8 offset = 0;
	char *p;
	
	if(strstr(Uart2_Buf,"+IPD")!=NULL)   		//�������ַ����к���^SISR
	{
		Delay_nMs(100);

		/*
		printf_s("�յ�����Ϣ��\r\n");

		printf_s(Uart2_Buf);
		printf_s("\r\n");
		*/
		p = Uart2_Buf;
		while(1)
		{
			offset ++;
			if(*p == ':')
			{
				break;
			}
			p ++;
			
		}
		
		memcpy(buf, Uart2_Buf + offset, First_Int);
		ret = First_Int - offset;
/*
		printf_hex((u8 *)Uart2_Buf, First_Int);
		printf_s("\r\n");
		*/
		
		CLR_Buf2();
	}

	return ret;
	/*
	�����õĺ���
	*/
}
//====================================================


u8 check_crc(u8 *buf, int len)
{
	u8 i;
	u8 crc;

	crc = buf[0];
	
	for(i = 1; i < len; i++)
	{
		crc ^= buf[i];
	}

	return crc;
}

void send_3a_server(u8 cmd, u8 *buf, u16 len)
{
	u8 data[100];
	u32 i = 0;

	data[i ++] = PRO_3A_HEAD_1;
	data[i ++] = PRO_3A_HEAD_2;

	data[i ++] = cmd;

	data[i ++] = (u8)(len >> 8);
	data[i ++] = (u8)(len & 0x00ff);
	
	memcpy(data + i, buf, len);
	i += len;

	data[i ++] = check_crc(&data[3], 2 + len);

	data[i ++] = PRO_3A_END_1;
	data[i ++] = PRO_3A_END_2;

	tcp_send(data, i + 1);
}


void heart_3a(void)
{
	if((machine_get_status() == MACHINE_STATUS_CONNECT) || (machine_get_status() == MACHINE_STATUS_LOGIN))
	{
		return ;
	}
	
	if(Heart_beat)
	{
		u8 buf[100];
		u16 i = 0;

		memcpy(buf, sys_id, sizeof(sys_id));
		i += sizeof(sys_id);

		buf[i++] = 100;//����
		buf[i++] = 1;//�źŵ���

		send_3a_server(CLIENT_HEART, buf, i);

		Heart_beat=0;
	}
}

void connect_3a_server(void)
{
	Connect_Server(SERVER_3A_ADDR);
	machine_set_status(MACHINE_STATUS_LOGIN);

	UART1_SendString("���ӳɹ�\r\n");

	set_Heartbeat_time(10);
}

/*
���������û��Ӧ���ظ���������
*/
void login_3a_server(void)
{
	static vu32 flg = 0;
	
	u8 buf[100];

	if((Heart_beat) || (flg == 0))
	{
		flg = 1;

		memcpy(buf, sys_id, sizeof(sys_id));
		buf[sizeof(sys_id)] = VERSION_3A;

		printf_s("login_3a_server ... \r\n");
		
		send_3a_server(CLIENT_LOGIN, buf, sizeof(sys_id) + 1);

		Heart_beat=0;
	}
/*
	delay_ms ++;
	if(delay_ms >= 2000)
	{
		delay_ms = 0;
		delay ++;
	}
	
	if(delay == 6000)
	{
		delay = 0;
	}
	*/
}


void set_3a_switch(u8 swh, u8 of)
{
	lock_switch(swh, of);

	//�������ݵ㶯������
	if(of != 0x2)
	{
		u8 buf[2];

		buf[0] = swh;
		buf[1] = of;

		send_3a_server(CLIENT_SET_SWITCH_ACK, buf, 2);
	}
}


void get_3a_switch(void)
{

}


//����3a��������������
void tcp_recv_3a_server(void)
{
	u8 buf[100];
	u32 ret; 
	
	ret = tcp_recv(buf);

	//printf_data(buf, ret);
	printf_hex(buf, ret);

	if(ret == 0)
	{
		return ;
	}

	
	if((buf[INDEX_HEAD_1] == PRO_3A_HEAD_1) && (buf[INDEX_HEAD_2] == PRO_3A_HEAD_2))
	{
		switch(buf[INDEX_CMD])
		{
			case SERVER_LOGIN_ACK:
				machine_set_status(MACHINE_STATUS_IDLE);
				set_Heartbeat_time(90);
				printf_s("��������¼Ӧ�� \r\n");
				break;

			case SERVER_HEART_ACK:
				printf_s("����������Ӧ�� \r\n");
				break;
				
			case SERVER_SET_SWITCH:
				printf_s("Զ�̿����ն�(00�� 01�� 02�㶯) \r\n");

				set_3a_switch(buf[INDEX_DATA_1], buf[INDEX_DATA_2]);

				
				break;

			case SERVER_GET_SWITCH:
				printf_s("״̬��ѯ���ն�ÿ�ε�¼ʱ�·� \r\n");

				get_3a_switch();
				
				break;

			case SERVER_GET_ICCID:
				printf_s("ICCID��ѯ \r\n");
				break;
				
		}
	}

}

MACHINE_STATUS_TYPE machine_get_status(void)
{
	return machine_status;
}

void machine_set_status(MACHINE_STATUS_TYPE status)
{
	machine_status = status;
}


//ʹ��״̬����ʽ
void loop_3a_machine(void)
{
	switch(machine_get_status())
	{
		case MACHINE_STATUS_CONNECT:
			connect_3a_server();
			break;
			
		case MACHINE_STATUS_LOGIN:
			login_3a_server();
			break;
			
		case MACHINE_STATUS_IDLE:

			break;
			
		default:
			
			break;
	}

	tcp_recv_3a_server();
	heart_3a();
	
}



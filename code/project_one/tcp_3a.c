
#define __TCP_3A_C__

#include "tcp_3a.h"


static MACHINE_STATUS_TYPE machine_status = MACHINE_STATUS_LOGIN;

//=========================================

//������
u8 sys_id[5] = {0x01, 0x02, 0x03, 0x04, 0x5};
//�汾
#define VERSION_3A		0x1

void tcp_send(u8 *buf, int len)
{
	/*
	�����õĺ���
	*/
}

void tcp_recv(void)
{
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

void send_3a_server(u8 cmd, u8 buf, u16 len)
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

	tcp_send(data, i);
}


void login_3a_server(void)
{
	u8 buf[100];
	
	Connect_Server(SERVER_3A_ADDR);

	memcpy(buf, sys_id, sizeof(sys_id));
	buf[sizeof(sys_id)] = VERSION_3A;

	send_3a_server(CLIENT_LOGIN, buf, sizeof(sys_id) + 1);
}


//����3a��������������
void tcp_recv_3a_server(void)
{
	tcp_recv();
}

MACHINE_STATUS_TYPE machine_get_status(void)
{
	return machine_status;
}

void machine_set_status(MACHINE_STATUS_TYPE status)
{
	return machine_status;
}


//ʹ��״̬����ʽ
void loop_3a_machine(void)
{
	switch(machine_get_status())
	{
		case MACHINE_STATUS_LOGIN:
			login_3a_server();
			break;
			
		case MACHINE_STATUS_IDLE:

			break;
			
		default:
			
			break;
	}

	tcp_recv_3a_server();
}




#define __TCP_3A_C__

#include "tcp_3a.h"


static MACHINE_STATUS_TYPE machine_status = MACHINE_STATUS_INIT;

//=========================================

//待处理
u8 sys_id[5] = {0x01, 0x02, 0x03, 0x04, 0x5};
//版本
#define VERSION_3A		0x1

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

		buf[i++] = 100;//电量
		buf[i++] = 1;//信号电量

		send_3a_server(CLIENT_HEART, buf, i);

		Heart_beat=0;
	}
}

void connect_3a_server(void)
{
	//不需要连接了，登录的时候连接即可
	//Connect_Server(SERVER_3A_ADDR);
	//UART1_SendString("连接成功\r\n");

	machine_set_status(MACHINE_STATUS_LOGIN);

	set_Heartbeat_time(10);
}

/*
如果服务器没有应答，重复发送数据
*/
void login_3a_server(void)
{
	static vu32 flg = 0;
	
	u8 buf[100];

	set_Heartbeat_time(2);

	if((Heart_beat) || (flg == 0))
	{
		flg = 1;

		memcpy(buf, sys_id, sizeof(sys_id));
		buf[sizeof(sys_id)] = VERSION_3A;

		printf_s("Connect_Server ... \r\n");

		//需要重新连接服务器
		Connect_Server(SERVER_3A_ADDR);

		printf_s("login_3a_server ... \r\n");
		send_3a_server(CLIENT_LOGIN, buf, sizeof(sys_id) + 1);

		printf_s("exit  login_3a_server ... \r\n");
		
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

	//返回数据点动不返回
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
	u8 buf[2];

	buf[0] = 0x1;
	buf[1] = get_output_port();

	send_3a_server(CLIENT_GET_OUT_POINT_ACK, buf, 2);


	buf[0] = 0x1;
	buf[1] = get_check_port();
	
	send_3a_server(CLIENT_GET_CHECK_POINT_ACK, buf, 2);
}


void get_iccid(void)
{
	u8 buf[10];

	get_local_iccid(buf, sizeof(buf));

	send_3a_server(CLIENT_GET_ICCID_ACK, buf, 2);
}


void sys_id_init(void)
{
	u8 buf[100];
	
	__sys_id_init(buf);

	memcpy(sys_id, buf, sizeof(sys_id));
}

//接收3a服务器，并处理
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
				printf_s("服务器登录应答 \r\n");
				break;

			case SERVER_HEART_ACK:
				printf_s("服务器心跳应答 \r\n");
				break;
				
			case SERVER_SET_SWITCH:
				printf_s("远程开启终端(00关 01开 02点动) \r\n");

				set_3a_switch(buf[INDEX_DATA_1], buf[INDEX_DATA_2]);

				
				break;

			case SERVER_GET_SWITCH:
				printf_s("状态查询，终端每次登录时下发 \r\n");

				get_3a_switch();
				
				break;

			case SERVER_GET_ICCID:
				printf_s("ICCID查询 \r\n");

				get_iccid();
				
				break;
				
		}
	}

}


void idle_3a_server(void)
{
	
}

void init_3a_server(void)
{
	sys_id_init();

	machine_set_status(MACHINE_STATUS_CONNECT);
}



void recv_gsm(void)
{
	u8 buf[100];
	u16 ret;
	
	ret = recv_gsm_data(buf, sizeof(buf));

	if(ret != 0)
	{
		printf_s("recv gsm data : ");
		printf_s((char *)buf);
		printf_s("\r\n");

		if(Find("CLOSED"))		//服务器关闭了
		{
			//进入重新连接状态
			machine_set_status(MACHINE_STATUS_CONNECT);
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


//使用状态机方式
void loop_3a_machine(void)
{
	switch(machine_get_status())
	{
		case MACHINE_STATUS_INIT:
			init_3a_server();
			break;
			
		case MACHINE_STATUS_CONNECT:
			connect_3a_server();
			break;
			
		case MACHINE_STATUS_LOGIN:
			login_3a_server();
			break;
			
		case MACHINE_STATUS_IDLE:
			idle_3a_server();
			break;
			
		default:
			
			break;
	}

	tcp_recv_3a_server();
	heart_3a();

	//不知道什么意思
	//check_switch();

	recv_gsm();
}



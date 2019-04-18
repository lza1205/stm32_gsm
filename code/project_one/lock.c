
#include "all.h"


/*
锁初始化函数，主要初始化IO口
*/

void lock_init(void)
{
	
}

/*
lock_switch	控制锁的功能
swh :	 	哪个锁，
of:			00关 01开 02点动
*/
void lock_switch(u8 swh, u8 of)
{
	
}


/*
get_output_port	返回输出口的电平状态 0/1
*/

u8 get_output_port(void)
{
	return 0;
}


/*
get_check_port	返回检测口的电平状态
*/

u8 get_check_port(void)
{
	return 0;
}


/*
get_local_iccid 		获取 iccid
*/
void get_local_iccid(u8 *buf, u8 len)
{
	
}

/*
__sys_id_init 		初始化ID
*/
void __sys_id_init(u8 *buf)
{
	u8 sys_id[5] = {0x01, 0x02, 0x03, 0x04, 0x5};
	memcpy(buf, sys_id, sizeof(sys_id));
}




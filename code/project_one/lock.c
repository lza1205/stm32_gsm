
#include "all.h"


/*
����ʼ����������Ҫ��ʼ��IO��
*/

void lock_init(void)
{
	
}

/*
lock_switch	�������Ĺ���
swh :	 	�ĸ�����
of:			00�� 01�� 02�㶯
*/
void lock_switch(u8 swh, u8 of)
{
	
}


/*
get_output_port	��������ڵĵ�ƽ״̬ 0/1
*/

u8 get_output_port(void)
{
	return 0;
}


/*
get_check_port	���ؼ��ڵĵ�ƽ״̬
*/

u8 get_check_port(void)
{
	return 0;
}


/*
get_local_iccid 		��ȡ iccid
*/
void get_local_iccid(u8 *buf, u8 len)
{
	
}

/*
__sys_id_init 		��ʼ��ID
*/
void __sys_id_init(u8 *buf)
{
	u8 sys_id[5] = {0x01, 0x02, 0x03, 0x04, 0x5};
	memcpy(buf, sys_id, sizeof(sys_id));
}




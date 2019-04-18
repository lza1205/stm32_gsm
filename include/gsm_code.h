
#ifndef __GSM_CODE_H__
#define __GSM_CODE_H__

#include "all.h"


void CLR_Buf2(void);
u8 Find(char *a);
void Second_AT_Command(char *b,char *a,u8 wait_time) ;
u8 Second_AT_Command_Try(char *b,char *a,u8 wait_time)     ;

void Second_AT_Data(char *b,char *a, u32 len, u8 wait_time);

void Wait_CREG(void);
void Set_ATE0(void);

void printf_hex(u8 *buf, int len);



#endif




#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

#define USART1_REC_MAXLEN 200	//最大接收数据长度

void USART1_Init_Config(u32 bound);
void UART1_SendString(char* s);
void USART2_Init_Config(u32 bound);
void UART2_SendString(char* s);
void UART1_SendLR(void);
void UART2_SendLR(void);
void UART1_SendByte(char s);
void UART1_SendData(u8* s, u32 len);


#define printf_s(s)		UART1_SendString(s)
#define printf_c(c)		UART1_SendByte(c)
#define printf_data(buf, len)	UART1_SendData(buf, len)

//串口1发送回车换行
#define UART1_SendLR() UART1_SendString("\r\n")

//											 USART_SendData(USART1,0X0D);\
//											 
//											 USART_SendData(USART1,0X0A)
//串口2发送回车换行
#define UART2_SendLR() UART2_SendString("\r\n")
//											 USART_SendData(USART2,0X0D);\
//											 USART_SendData(USART2,0X0A)
#endif



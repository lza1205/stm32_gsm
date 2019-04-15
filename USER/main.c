/**********************************************************************************
 * 工程名  :GPRS
 * 作者    :lza1205
 * 博客    :https://blog.csdn.net/aa120515692
**********************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "Led.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"

#include "gsm_code.h"
#include "gsm_base.h"
#include "tcp_base.h"


/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与MG323模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
int main(void)
{
	SysTick_Init_Config();
	GPIO_Config();

	USART1_Init_Config(115200);
	USART2_Init_Config(115200);
	Timer2_Init_Config();
	UART1_SendString("GPRS模块GPRS测试程序\r\n");
	UART1_SendString("GPRS模块在注册网络\r\n");
	Wait_CREG();
	UART1_SendString("GPRS模块注册成功\r\n");
	UART1_SendString("GPRS模块开始连接服务器\r\n");
	Set_ATE0();
	Connect_Server(NULL);
	UART1_SendString("连接成功\r\n");
	while(1)
	{
		Rec_Server_Data();
		
		tcp_heart_beat();
	}
}


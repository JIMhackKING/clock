#include "SystemFunc.h"
#include <string.h>

uchar data_len=0;  // 指令长度标志位
uchar data_len_bak=0;
bit receive_complete=0;  // 数据接收完成标志位
char recv[11];  // 接收数据缓冲区， char 类型用于比较字符串, 长度加 1 是为了加 \0
extern int clock[4];

/********************************************************************
* 名称 : UART_init()
* 功能 : 初始化串口程序，晶振11.0592, 波特率9600
* 输入 : 无
* 输出 : 无
***********************************************************************/
void UART_init(void)
{
	TMOD|=0x20;  // 设置定时器1为工作方式2（8位自动重载），高八位为重载常数，溢出时低八位自动从高八位取初值
	TH1=0xfd;  // 9600 波特率
	TL1=0xfd;
	TR1=1;
	EA=1;
	ES=1;  // 串口中断允许
	REN=1;
	SM0=0;
	SM1=1;
}

void send(uchar d)
{
	SBUF=d;
	while(!TI);
	TI=0;
}

void sendarr(uchar *pd, uchar len)
{
	// 发送一个数组
	while(len--)
	{
		send(*pd);
		pd++;
	}
}

/************************************************
作用：串口中断函数
************************************************/
void uart(void) interrupt 4 // 串口中断
{
	if(RI)  // 收到数据
	{
		if(data_len==0)
		{
			data_len=SBUF;  // 如果 data_len 为0，即没收到数据，于是就初始化数据长度
			data_len_bak=data_len;
		}
		else
		{
			recv[data_len_bak - data_len] = SBUF;
			data_len--;
			if(data_len==0) receive_complete=1;  // 如果 data_len 减到0，即数据已接收完成
		}
		RI=0;
	}
}

void deal_cmd(void)
{
	if(data_len_bak>10)
	{
		ES=0;
		sendarr("Error: Command length too long!\r\nThe maximum allowable length is 10\r\n", 69);
		Delay(500);
		ES=1;
		data_len = 0;
		data_len_bak=0;
	}
	else if(receive_complete)
	{
		receive_complete=0;
		ES=0;
		recv[data_len_bak] = '\0';  // 给接收到的数据最后加上 \0 用于比较字符串
		if(strcmp(recv, "clock")==0)  sendarr(clock, 8);
		//send('Y');
		ES=1;
	}
}

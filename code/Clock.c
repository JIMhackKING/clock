/************************************************
功能介绍：带闹钟功能的定时浇花装置
端口：
	P0  -->  数码管段位显示
	P2.0 ~ P2.3  -->  数码管位码
	P1.0  -->  继电器（闹钟响起时为低电平)
	P3.2 ~ P3.5  -->  按钮开关
*************************************************/

#include <stdio.h>
#include "SystemFunc.h"

/************************************************
常量定义
************************************************/
sbit output=P1^0;
uint count=0;  // 计数总值
uchar Dis_code[]
={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar dis_buff[4];  //数码管显示的所有数字
uchar MODE=0;  // 模式选择，设置闹钟
int time;  // 分为单位时间
int clock[4]={0,5,10,1};  // 第一个实时时间，2个闹钟设置，第三个时浇花时长
bit open;  // 继电器端口开启标志
bit Clock_Status=0;  // 闹钟开关标志
/************************************************/

void EXT1(void) interrupt 2
{
	Clock_Status=!Clock_Status;
	P1=~((char)Clock_Status<<7); 
}

/************************************************
作用：定时器0中断函数，定时时间为分
************************************************/
void TC0(void) interrupt 1
{
	// 11.0592MHz 晶振频率 50ms 计数值为 46080
	TH0=TH0+0x4c;  // 初始化初值
	TL0=TL0+0x00;
	count++;
	if(count==1200)  // 1m
	{
		count=0;
		clock[0]++;
		if(clock[0]==1440)
			clock[0]=0;
	}
}

// 检查时间是否到了闹钟时间
void check_clock(void)
{
	uchar j;
	for(j=1;j<3;j++)
	{
		if(clock[0]==clock[j]&&open==0)
		{
			time=clock[0];
			output=0;
			open=1;
		}
	}
	if(open)
	{
		if(clock[0]-time==clock[3]||clock[0]<time)
		{
			output=1;
			open=0;
		}
	}
}

void main(void)
{
	INT_Init();
	UART_init();
	PT0=1;  // 设置定时器0为最高优先级
	while(1)
	{
		Bin2Bcd(clock[MODE]);
		display();
		deal_cmd();
		key_press();
		if(Clock_Status==0) continue;
		check_clock();
	}
}
/************************************************
功能介绍：带闹钟功能的定时浇花装置
端口：
	P0  -->  数码管段位显示
	P2.0 ~ P2.3  -->  数码管位码
	P2.5  -->  蜂鸣器（闹钟响起时为低电平)
	P3.2 ~ P3.5  -->  按钮开关
*************************************************/

#include <stdio.h>
#include "SystemFunc.h"
#include "eeprom.h"

/************************************************
常量定义
************************************************/
uint count=0;  // 计数总值
uchar Dis_code[]
={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar dis_buff[4];  //数码管显示的所有数字
uchar MODE=0;  // 模式选择，设置闹钟
uint clock[3]={0,1,2};  // 第一个实时时间，2个闹钟设置，第三个时浇花时长
bit Clock_Status=0;  // 闹钟开关标志
bit is_open = 0;  // 闹钟已经响起的标志位
uint this_time;  // 闹钟时间已经到达，防止关闭闹钟但时间还是在闹钟时间的时候重复响闹钟

// buzzer
sbit output=P2^5;
// buzzer control button
sbit KEY_CONTROL = P3^3;
// clock status led
sbit status_led = P1^7;
/************************************************/

/*
外部中断1响应函数
闹钟控制按钮，闹钟没响时控制闹钟开关，闹钟已经响起时关闭闹钟
*/
void EXT1(void) interrupt 2
{
	Delay(2);
	if(KEY_CONTROL == 0)  // 延时消抖
	{
		if (is_open)  // 如果闹钟已经响了
		{
			output = 1;  // 关闭闹钟
			is_open = 0;  // 清除标志位
		}
		else  // 如果闹钟没响
		{
			Clock_Status=!Clock_Status;  // 关闭闹钟开关
			status_led = !Clock_Status; 
		}
	}
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
	for(j=1;j<3;j++)  // 轮流检查两个闹钟时间
	{
			if((clock[0] == clock[j]) && clock[0] != this_time)  // 当前时间是否已经到了设定的闹钟时间
			{
				output=0;  // 开启蜂鸣器
				is_open = 1;  // 设置标志位
				this_time = clock[0];
			}
	}
}

void main(void)
{
	// 程序开始时先从的eeprom加载设置的闹钟时间
	clock[1] = eeprom_read_byte(0x2800) | ((uint)eeprom_read_byte(0x2801) << 8);
	clock[2] = eeprom_read_byte(0x2802) | ((uint)eeprom_read_byte(0x2803) << 8);
	INT_Init();  // 中断初始化
	PT0=1;  // 设置定时器0为最高优先级
	P1 &= 0xfe;  // Init the start led status
	output = 1;
	while(1)
	{
		Bin2Bcd(clock[MODE]);  // 数码管编码
		display();  // 数码管显示
		//deal_cmd();
		key_press();  // 检查按键是否按下
		if(Clock_Status==0) continue;  // 如果闹钟被关闭就不执行下面的语句
		check_clock();
	}
}
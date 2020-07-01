#include "SystemFunc.h"

extern uchar dis_buff[4];
extern uchar Dis_code[];


/************************************************
作用：毫秒级延时函数
输入：del 延时时长
************************************************/
void Delay(uint del)
{
	uint i,j;
	for(i=0; i<del; i++)
	for(j=0; j<192; j++)   
	;
}

/************************************************
作用：转换数码管显示
输入：时间
************************************************/
void Bin2Bcd(int time)
{
	int hour, minute;
	// 计算时分
	hour=time/60;
	minute=time%60;
	dis_buff[3]=hour/10;
	dis_buff[2]=hour%10;
	dis_buff[1]=minute/10;
	dis_buff[0]=minute%10;
}

/************************************************
作用：数码管显示
************************************************/
void display(void)
{
	uchar i;
	uchar is_open = 0;
	for(i=0;i<4;i++)
	{
		is_open = (P2 & 0x20) | 0xdf;
		P2 = is_open & ~(0x08>>i);
		P0=Dis_code[dis_buff[i]];
		Delay(1);
		P0 = 0xff;
	}
}

/************************************************
作用：初始化中断
************************************************/
void INT_Init(void)
{
	TMOD=0x01;  // 设置定时器0为工作方式1，16位定时/计数器
	// 50ms 请求一次中断
	TH0=0x4c;  // 高8位定时器初值
	TL0=0x00;  // 低8位定时器初值
	EA=1;  // 开总中断
	ET0=1;  // 定时器0中断允许
	TR0=1;  // 启动定时器0，开始计数
	EX1=1;
	IT1=1;
}

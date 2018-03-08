#include "SystemFunc.h"

extern uchar MODE;
extern int clock[4];

// 加时
void add_time(void)
{
	clock[MODE]++;
	if(clock[MODE]==1440)
		clock[MODE]=0;
}

// 减时
void sub_time(void)
{
	clock[MODE]--;
	if(clock[MODE]<0)
		clock[MODE]=1439;
}

/************************************************
作用：按钮一响应函数
************************************************/
void key1_down(void)
{
	uchar i,j=20,n=3;
	Delay(5);
	while(P3==0xef)
	{
		add_time();
		if(n==0)
		{
			j-=5;
			n=3;
		}
		if(j<5) j=5;
		n-=1;
		for(i=0;i<j;i++)
		{
			Bin2Bcd(clock[MODE]);
			display();
		}
	}
}

/************************************************
作用：按钮二响应函数
************************************************/
void key2_down(void)
{
	uchar i,j=20,n=3;
	Delay(5);
	while(P3==0xdf)
	{
		sub_time();
		if(n==0)
		{
			j-=5;
			n=3;
		}
		if(j<5) j=5;
		n-=1;
		for(i=0;i<j;i++)
		{
			Bin2Bcd(clock[MODE]);
			display();
		}
	}
}

/************************************************
作用：按钮三响应函数
************************************************/
void key3_down(void)
{
	uchar i;
	Delay(2);
	if(P3==0xfb)
	{
		MODE++;
		if(MODE==4)
			MODE=0;
	}
	for(i=0;i<30;i++)
	{
		Bin2Bcd(clock[MODE]);
		display();
	}
}

// 判断哪个按钮被按下并作出处理
void key_press(void)
{
	if(P3==0xef) key1_down();
	else if(P3==0xdf) key2_down();
	else if(P3==0xfb) key3_down();
}

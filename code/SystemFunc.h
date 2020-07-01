#ifndef __SystemFunc_H
#define __SystemFunc_H

#include <reg52.h>

/************************************************
宏变量定义
************************************************/
#define uchar unsigned char
#define uint unsigned int

void Delay(uint del);
void Bin2Bcd(int time);
void display(void);
void INT_Init(void);
void add_time(void);
void sub_time(void);
void key_press(void);
void deal_cmd(void);
void UART_init(void);

#endif
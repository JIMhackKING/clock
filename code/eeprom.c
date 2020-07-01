#include "eeprom.h"

/*┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
函数：擦除某一扇区（每个扇区512字节）
入口：addr = 某一扇区首地址                          
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈*/
void eeprom_erase(uint addr)
{
	// 打开 IAP 功能(ISP_CONTR.7)=1:允许编程改变Flash, 设置Flash操作等待时间
	// 0x83(晶振<5M)   0x82(晶振<10M)   0x81(晶振<20M)   0x80(晶振<40M)
	ISP_CONTR = 0x81;
	ISP_CMD   = 0x03;                  // 用户可以对"Data Flash/EEPROM区"进行扇区擦除
	ISP_ADDRL = addr;         // ISP/IAP操作时的地址寄存器低八位，
	ISP_ADDRH = addr>>8;      // ISP/IAP操作时的地址寄存器高八位。
	EA =0;	// 关闭中断保证写数据稳定性
	ISP_TRIG = 0x46;          // 在ISPEN(ISP_CONTR.7)=1时,对ISP_TRIG先写入46h，
	ISP_TRIG = 0xB9;          // 再写入B9h,ISP/IAP命令才会生效。
	_nop_();
	Q0();                                          // 关闭ISP/IAP
}

/*┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
函数：写一字节
入口：addr = 扇区单元地址 , dat = 待写入数据
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈*/
void eeprom_write_byte(uint addr,uchar dat)
{
	ISP_CONTR = 0x81;                  
    ISP_CMD   = 0x02;              // 用户可以对"Data Flash/EEPROM区"进行字节编程
    ISP_ADDRL = addr;        
    ISP_ADDRH = addr>>8;      
    ISP_DATA  = dat;          // 数据进ISP_DATA
    EA = 0;
    ISP_TRIG = 0x46;         
    ISP_TRIG = 0xB9;         
    _nop_();
    Q0();                                          // 关闭ISP/IAP
}

/*┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
函数：读一字节
入口：addr = 扇区单元地址
出口：dat  = 读出的数据
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈*/
uchar eeprom_read_byte(uint addr)
{   
    uchar dat;
       
    ISP_CONTR = 0x81;                  
    ISP_CMD   = 0x01;         // 用户可以对"Data Flash/EEPROM区"进行字节读
    ISP_ADDRL = addr;         
    ISP_ADDRH = addr>>8;      
    EA = 0;
    ISP_TRIG = 0x46;         
    ISP_TRIG = 0xB9;         
    _nop_();
    dat = ISP_DATA;                          // 取出数据
    Q0();                                          // 关闭ISP/IAP                   
    return dat;
}

/*┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
函数：关闭ISP/IAP操作
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈*/
void Q0()
{
    ISP_CONTR = 0;            // 关闭IAP功能
    ISP_CMD   = 0;            // 待机模式，无ISP操作
    ISP_TRIG  = 0;            // 关闭IAP功能, 清与ISP有关的特殊功能寄存器
        EA=1;
}

/*
STC89C52RC内部EEPROM详细地址表：
     第一扇区            第二扇区             第三扇区              第四扇区
起始地址  结束地址   起始地址   结束地址   起始地址   结束地址   起始地址    结束地址
2000h      21FFh    2200h     23FFh     2400h      25FFh      2600h      27FFH
     第五扇区            第六扇区               第七扇区              第八扇区
起始地址   结束地址   起始地址   结束地址    起始地址   结束地址   起始地址    结束地址
2800h     29FFh      2A00h     2BFFh      2C00h      2DFFh      2E00h      2FFFh
*/

#ifndef __EEPROM_H
#define __EEPROM_H

#include "SystemFunc.h"
#include <intrins.h>

sfr ISP_DATA = 0xe2;   
sfr ISP_ADDRH = 0xe3;     
sfr ISP_ADDRL = 0xe4;   
sfr ISP_CMD = 0xe5;   
sfr ISP_TRIG = 0xe6;      
sfr ISP_CONTR = 0xe7;

void eeprom_erase(uint addr);
void eeprom_write_byte(uint addr,uchar dat);
uchar eeprom_read_byte(uint addr);
void Q0();

#endif
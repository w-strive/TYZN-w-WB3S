#ifndef _eeprom_H_
#define _eeprom_H_

#include "public.h"


#define WT_30M 0x80
#define WT_24M 0x81
#define WT_20M 0x82
#define WT_12M 0x83
#define WT_6M 0x84
#define WT_3M 0x85
#define WT_2M 0x86
#define WT_1M 0x87


/* 自定义eeprom大小  */

#define flash_addr_start 0x0000		//起始地址



char IapRead(int addr);
void IapProgram(int addr, char dat);
void IapErase(int addr);


#endif

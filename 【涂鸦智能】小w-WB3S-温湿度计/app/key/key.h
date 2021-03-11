#ifndef _KEY_H_
#define _KEY_H_

#include "public.h"

sbit LED = P2^0;

/* 功能键定义 */
#define K1 	(CCON & 0x01)
#define K2 	(CCON & 0x02)
#define K3 	(CCON & 0x04)
#define K4 	(CCON & 0x08)



/* 蜂鸣器定义 */
sbit BEEP = P6^7;


extern char K4_bits;
extern bit BEEP_button_bit;

void Key_Init(void);
void Key_Beep_sound(void);

void Key_Updata(void);



#endif

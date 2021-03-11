#ifndef VEML6030_h
#define VEML6030_h

#include "public.h"


#define VEML6030_Write_Addr 0x20

//寄存器
#define CONF_CMD 0x00		//配置寄存器
#define WINDOW_H_CMD 0x01
#define WINDOW_L_CMD 0x02
#define PWR_CMD 0x03		//未定义，保留
#define ALS_CMD 0x04
#define WHITE_CMD 0x05
#define INT_CMD 0x06

//Integration Times bit masks
#define IT25 0x300  //25ms
#define IT50 0x200  //50ms
#define IT100 0x000 //100ms
#define IT200 0x040 //200ms
#define IT400 0x080 //400ms
#define IT800 0x0C0 //800ms

#define GAIN_1 0x0000 //Gain x1
#define GAIN_2 0x0800 //Gain x2
#define GAIN_1_8 0x1000 //Gain x1/8
#define GAIN_1_4 0x1800 //Gain x1/4

//Persistance will define how man consecutive values required to trigger interrupt
#define PER_1 0x00 //Persistance of 1
#define PER_2 0x10 //Persistance of 2
#define PER_4 0x20 //Persistance of 4
#define PER_8 0x30 //Persistance of 8


void VEML6030_Init(void);
uint VEML6030_Read_reg_WHITE(void);

#endif

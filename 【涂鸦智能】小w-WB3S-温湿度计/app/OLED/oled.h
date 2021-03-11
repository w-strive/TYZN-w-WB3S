#ifndef _oled_H_
#define _oled_H_

#include "public.h"
#include "GUI.h"

//#define OLED_GDDRAM_mode //采用缓冲刷新整个屏幕方式
//extern unsigned char  OLED_GDDRAM[];

#define OLED_Page_mode	//页控制模式



/* SPI方式引脚定义 */
sbit OLED_SCK   = P2^5;	//SCK
sbit OLED_MOSI  = P2^3;	//MOSI

sbit OLED_CS  = P2^1;	//片选
sbit OLED_DC  = P2^2;	//0指令与1数据选择
sbit OLED_RES = P4^2;	//复位



/* 函数声明 */
void OLED_Init(void);
void OLED_Draw_BMP(const uchar* bmp);
void OLED_Draw_BMP_part_up(const uchar* bmp);
void OLED_Draw_BMP_part_down(const uchar* bmp);

void OLED_Draw_Roll(bit state);


void OLED_WriteDat(uchar dat);
void OLED_WriteCmd(uchar cmd);

void LCD_Fill(void);

/* spi驱动方式函数定义 */
#define oled_spi_SendRead SPI_SendRead

#endif

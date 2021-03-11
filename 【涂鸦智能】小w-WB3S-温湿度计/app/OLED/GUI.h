#ifndef _GUI_H_
#define _GUI_H_

#include "public.h"

/* 定义屏幕 */
#define OLED_X_max 128
#define OLED_Y_max 64



#ifdef OLED_GDDRAM_mode	//采用缓冲刷新整个屏幕方式

void Draw_pixel(int x,int y, bit color);
void Bresenham_Draw_gui_Line(int x1,int y1,int x2,int y2, bit color);
void Bresenham_Draw_gui_circle(int xc, int yc, int r, bit fill, bit color);
void Draw_gui_Triangle(int x1,int y1,int x2,int y2,
					   int x3,int y3,bit fill, bit color);
void Draw_gui_Quadrilateral(int x1,int y1,int x2,int y2,
							int x3,int y3,int x4,int y4,
							bit fill, bit color			);

void Draw_gui_F6_8(int x,int y,char ascll,bit color);
void Draw_gui_F8_16(int x,int y,char ascll,bit color);
void Draw_gui_F16_16(int x,int y,uchar *ascll,char len,bit color);


#else


							
							
void Draw_P6x8(unsigned char x,unsigned char y,unsigned char ch);
void Draw_P6x8Str(unsigned char x,unsigned char y,unsigned char *ch);
void Draw_P8x16(uchar x,uchar y,uchar ch);
void Draw_P8x16Str(unsigned char x,unsigned char y,unsigned char *ch);
void Draw_P16x16Str(uchar x,uchar y,uchar const *ch,uchar len);

void Draw_RSSI(uchar strong);
void Draw_POWER(uchar num);
void Draw_TS(uchar num);
void Draw_TA(uchar num);



#endif




extern uchar code arrows[];
extern uchar code mark[];

extern uchar code RPD[];
extern uchar code OBSERVE[];

extern uchar code YM_1[];
extern uchar code FX_1[];
extern uchar code FX_2[];



#endif

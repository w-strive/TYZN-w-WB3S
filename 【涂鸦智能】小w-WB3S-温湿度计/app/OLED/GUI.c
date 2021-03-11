#include "GUI.h"
#include "font.h"



#ifdef OLED_GDDRAM_mode

/*
	注意：左上角点坐标不是从0起，是从1起， 不要写入0，0为第一个点
	
	
			写入0，0不会改变屏幕数据，会直接return出去

*/



/*
	画点
	
	说明：左上角点坐标不是从0起，是从1起
*/
void Draw_pixel(int x,int y, bit color)
{
	uchar page = 0;	//保存页
	uint  coord = 0;//保存点坐标对应的数组下标
	uchar DDRAM = 0;//保存要写入地址的原内容
	
	/* 检查点位置是否错误 */
	if(x == 0 || y == 0)
		return;
	if(x > OLED_X_max || y > OLED_Y_max)	
		return;
	
	/* 先确定行 */
	page = (uchar)((y-1) * 0.125f);	//求出页
	coord = (page << 7);	// *128
	
	/* 确定列 */
	coord += (x-1);		
	
	DDRAM = OLED_GDDRAM[coord];
	/* 确定具体行 */
	if(y > 8)
	{
		y = y - (page << 3);	//求出页    page / 8
	}
	
	/* 判断点是否点亮 */
	if(color)
		DDRAM |= 0x01<<(y-1);
	else
		DDRAM &= ~(0x01<<(y-1));
	
	OLED_GDDRAM[coord] = DDRAM;
}


/*
	Bresenham画线算法(此程序为任意方向,非必须0<=k<=1)
*/
void Bresenham_Draw_gui_Line(int x1,int y1,int x2,int y2, bit color)
{
	int t;					//临时变量
	int xerr = 0, yerr = 0;
	int delta_x, delta_y;
	int distance;			//保存差值大的坐标轴

	int incx;	//判断x轴方向  （ incx>0:递增   incx==0:不变   incx<0:递减）
	int incy;	//判断y轴方向

	int uRow;	//要画的x点坐标
	int uCol;	//要画的y点坐标

	delta_x = x2 - x1;	//计算X坐标增量
	delta_y = y2 - y1;	//计算Y坐标增量

	uRow = x1;	//保存起始坐标点x
	uCol = y1;	//保存起始坐标点y

	/* 判断x轴方向 */
	if (delta_x > 0)		//设置单步方向(正向)
		incx = 1; 
	else if (delta_x == 0)  //垂直线 
		incx = 0;
	else					//设置单步方向(反向)
	{ 
		incx = -1; 
		delta_x = -delta_x; //将数值变成正数
	}

	/* 判断y轴方向 */
	if (delta_y > 0)		//设置单步方向(正向)
		incy = 1;
	else if (delta_y == 0)	//水平线 
		incy = 0;
	else					//设置单步方向(反向)
	{ 
		incy = -1; 
		delta_y = -delta_y; //将数值变成正数
	}

	/* 选取基本增量坐标轴 */
	if (delta_x > delta_y)
		distance = delta_x;
	else 
		distance = delta_y;
	
	/* 开始画线 */
	for (t = 0; t <= distance + 1; t++)//画线输出 
	{
		Draw_pixel(uRow, uCol, color);//画点

		xerr += delta_x;
		yerr += delta_y;

		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;	//改变x坐标值
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;	//改变y坐标值
		}
	}
}



/********************************************************
 --函  数：_draw_circle_8
 --功  能：OLED画圆8点法
 --参  数:  xc：圆形中心的x坐标
			yc：圆形中心的y坐标
			x：相对于圆形中心的x坐标
			y：相对于圆形中心的y坐标
			c：是否点亮
 --返回值：无
 --备  注：无
********************************************************/
static void OLED_Draw_circle_8(int xc, int yc, int x, int y, bit color)
{
	Draw_pixel(xc + x, yc + y, color);
	Draw_pixel(xc - x, yc + y, color);
	Draw_pixel(xc + x, yc - y, color);
	Draw_pixel(xc - x, yc - y, color);
	Draw_pixel(xc + y, yc + x, color);
	Draw_pixel(xc - y, yc + x, color);
	Draw_pixel(xc + y, yc - x, color);
	Draw_pixel(xc - y, yc - x, color);
}
/********************************************************
 --函  数：Bresenham_Draw_gui_circle
 --功  能：OLED画圆
 --参  数:  xc：圆形中心的x坐标
			yc：圆形中心的y坐标
			r： 圆形半径
		 fill： 是否填充
			c:	点亮或熄灭
 --返回值：无
 --备  注：无
********************************************************/
void Bresenham_Draw_gui_circle(int xc, int yc, int r, bit fill, bit color)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;

	if (fill)
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				OLED_Draw_circle_8(xc, yc, x, yi, color);

			if (d < 0) {
				d = d + 4 * x + 6;
			}
			else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
	else
	{
		// 如果不填充（画空心圆）
		while (x <= y) {
			OLED_Draw_circle_8(xc, yc, x, y, color);
			if (d < 0) {
				d = d + 4 * x + 6;
			}
			else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}





/********************************************************
 --函  数：Draw_gui_Quadrilateral
 --功  能：OLED画三角形
 --参  数:  x1：x坐标
			y1：y坐标
			...
		 fill： 是否填充
			c:	点亮或熄灭
 --返回值：无
 --备  注：无
********************************************************/
void Draw_gui_Triangle(int x1,int y1,int x2,int y2,
					   int x3,int y3,bit fill, bit color)
{
	if(fill)
	{
		
	}
	Bresenham_Draw_gui_Line(x1,y1,x2,y2,color);
	
	Bresenham_Draw_gui_Line(x2,y2,x3,y3,color);
	
	Bresenham_Draw_gui_Line(x3,y3,x1,y1,color);
}




/********************************************************
 --函  数：Draw_gui_Quadrilateral
 --功  能：OLED画4边形
 --参  数:  x1：x坐标
			y1：y坐标
			...
		 fill： 是否填充
			c:	点亮或熄灭
 --返回值：无
 --备  注：无
********************************************************/
void Draw_gui_Quadrilateral(int x1,int y1,int x2,int y2,
							int x3,int y3,int x4,int y4,
							bit fill, bit color			)
{
	
	if(fill)
	{
		
	}
	
	Bresenham_Draw_gui_Line(x1,y1,x2,y2,color);
	
	Bresenham_Draw_gui_Line(x2,y2,x3,y3,color);
	
	Bresenham_Draw_gui_Line(x3,y3,x4,y4,color);
	
	Bresenham_Draw_gui_Line(x4,y4,x1,y1,color);
	
}









/********************************************************
 --函  数：Draw_gui_6_8
 --功  能：OLED画6*8
 --参  数:  x：x坐标
			y：y坐标
		ascll: 要显示的字符
			c: 点亮或熄灭
 --返回值：无
 --备  注：无
********************************************************/
void Draw_gui_F6_8(int x,int y,char ascll,bit color)
{
	uchar con = 0;//字符数组的下标
	int F_x = x,F_y = y;//要写入的点坐标
	uchar acsll = 0;	  //保存当前字符数组的一个字节
	
	uchar i,j;
	
	con = ascll - ' ';
	
	for(i=0;i<6;i++)
	{
		acsll = Font_6x8[con][i];
		for(j=0;j<8;j++)
		{
			if(color)
			{
				if(acsll & (0x01 << j))
					Draw_pixel(F_x,F_y++,1);
				else
					Draw_pixel(F_x,F_y++,0);
			}
			else
				Draw_pixel(F_x,F_y++,0);
		}
		F_x++;
		F_y = y;
	}	
}




/********************************************************
 --函  数：Draw_gui_F8_16
 --功  能：OLED画8*16
 --参  数:  x：x坐标
			y：y坐标
		ascll: 要显示的字符
			c:	点亮或熄灭
 --返回值：无
 --备  注：无
********************************************************/
void Draw_gui_F8_16(int x,int y,char ascll,bit color)
{
	uchar con = 0;//字符数组的下标
	int F_x = x,F_y = y;//要写入的点坐标
	uchar acsll = 0;	  //保存当前字符数组的一个字节
	
	uchar i,j;
	
	con = ascll - ' ';
	
	for(i=0;i<16;i++)
	{
		acsll = Font_8x16[con][i];
		for(j=0;j<8;j++)
		{
			if(color)
			{
				if(acsll & (0x01 << j))
					Draw_pixel(F_x,F_y++,1);
				else
					Draw_pixel(F_x,F_y++,0);
			}
			else
				Draw_pixel(F_x,F_y++,0);
		}
		
		F_x++;
		
		if(i == 7)
			F_x = x;
		
		if(i >= 7)
			F_y = y + 8;
		else
			F_y = y;
	}
	
}




/********************************************************
 --函  数：Draw_gui_F8_16
 --功  能：OLED画16*16汉字
 --参  数:  x：x坐标
			y：y坐标
		ascll: 要显示的字符
		  len：写入字个数
			c: 点亮或熄灭
 --返回值：无
 --备  注：汉字取模   （阴码  列行式  逆向）
********************************************************/
void Draw_gui_F16_16(int x,int y,uchar *ascll,char len,bit color)
{
	char con;
	uchar val = 0;		//取数组数据
	int F_x = x,F_y = y;//要写入的点坐标
	
	uchar i,j;
	
	/* 坐标判断 */
	if(F_x == 0 || F_y == 0)
		return;
	

	for(con = 0;con < len; con++)
	{
		/* 送入显示 */
		for(i=0;i<32;i++)
		{
			val = *ascll;	//读取当前位置数据
			
			for(j=0;j<8;j++)	//按位写入一个字节
			{
				if(color)
				{
					if(val & (0x01 << j))
						Draw_pixel(F_x,F_y++,1);
					else
						Draw_pixel(F_x,F_y++,0);
				}
				else
					Draw_pixel(F_x,F_y++,0);
			}

			F_x++;	//列加1
			
			if(i == 15)
				F_x = x + 16*con;
			
			if(i >= 15)
				F_y = y + 8;
			else
				F_y = y;
			
			ascll++;
		}
	}
	
	
}












#endif

















#ifdef OLED_Page_mode  //页寻址模式
		
	

void LCD_Set_Pos(unsigned char x, unsigned char y)
{ 
	OLED_WriteCmd(0xb0+y);
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);
	OLED_WriteCmd((x&0x0f)|0x00); 
}

//==============================================================
//函数名：LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符
//返回：无
//==============================================================  
void Draw_P6x8(uchar x,uchar y,uchar ch)
{
	unsigned char c=0,i=0,j=0;      
	
	c = ch - ' ';
	if(x>126)
	{
		x=0;
		y++;
	}
	LCD_Set_Pos(x,y);
	for(i=0;i<6;i++)     
	  OLED_WriteDat(Font_6x8[c][i]);  
}


//==============================================================
//函数名：LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void Draw_P6x8Str(uchar x,uchar y,uchar *ch)
{
	unsigned char c=0,i=0,j=0;      
	while (ch[j]!='\0')
	{    
		c =ch[j]-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		LCD_Set_Pos(x,y);
		for(i=0;i<6;i++)     
		  OLED_WriteDat(Font_6x8[c][i]);  
		x+=6;
		j++;
	}
}


//==============================================================
//函数名：Draw_P8x16(unsigned char x,unsigned char y,unsigned char p)
//功能描述：写入一个标准ASCII字符
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符
//返回：无
//==============================================================  
void Draw_P8x16(uchar x,uchar y,uchar ch)
{
	unsigned char c=0,i=0,j=0;      
	
	c = ch - ' ';
	
	if(x>126)
	{
		x=0;
		y++;
	}
	LCD_Set_Pos(x,y);
	for(i=0;i<8;i++)     
		OLED_WriteDat(Font_8x16[c][i]);
	LCD_Set_Pos(x,y+1);    
	for(i=0;i<8;i++)     
		OLED_WriteDat(Font_8x16[c][i+8]); 
}



//==============================================================
//函数名：LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//============================================================== 
void Draw_P8x16Str(unsigned char x,unsigned char y,unsigned char *ch)
{
  unsigned char c=0,i=0,j=0;
        
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120)
		{
			x=0;
			y++;
		}
		LCD_Set_Pos(x,y);
		for(i=0;i<8;i++)     
			OLED_WriteDat(Font_8x16[c][i]);
		LCD_Set_Pos(x,y+1);    
		for(i=0;i<8;i++)     
			OLED_WriteDat(Font_8x16[c][i+8]);  
		x+=8;
		j++;
	}
}



//==============================================================
//函数名：LCD_P16x16Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入多个汉字
//参数：显示的位置（x,y），y为页范围0～7，要显示的汉字 ,  写入汉字个数
//返回：无
//============================================================== 
void Draw_P16x16Str(uchar x,uchar y,uchar const *ch,uchar len)
{
	uchar c,i=0;
        
	for(c = 0; c < len; c++)
	{
		/* 写入上半字 */
		LCD_Set_Pos(x,y);
		for(i=0;i<16;i++)
			OLED_WriteDat(*(ch++));
		/* 写入下半字 */
		LCD_Set_Pos(x,y+1);
		for(i=0;i<16;i++)
			OLED_WriteDat(*(ch++));  
		
		x+=16;
	}
}





	
#endif







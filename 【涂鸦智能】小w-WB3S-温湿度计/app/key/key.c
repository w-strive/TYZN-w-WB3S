#include "key.h"


/* 
	按键是用PCB做圆形铜线，用带有碳膜按键
	所有按键触发采用PCA中断
*/

bit BEEP_button_bit = 0;	//按键响标志


char K4_bits = 0;

void Key_Beep_sound(void)
{
	BEEP_button_bit = 1;	//置位标志位
	BEEP = 1;				//开始响
}



void Key_Init(void)
{
	/* 四个按键使用PCA中断 */
	P_SW1 |= 0x20;	//切换到P7口
	
	
	CCON = 0x00;
	CMOD = 0x08;
	CL = 0x00;
	CH = 0x00;
	CCAPM0 = 0x11;	//下降沿触发
	CCAPM1 = 0x11;
	CCAPM2 = 0x11;
	CCAPM3 = 0x11;
	CCAP0L = 0;
	CCAP0H = 0;
	CR = 1;
	
}



void Key_Updata(void)
{

}


void PCA_Isr() interrupt 7
{
	if(K1)
	{
		OLED_WriteCmd(0x81);//--设置对比度 
		OLED_WriteCmd(0xef);//  对比度的值（值越大越亮）
		
		CCON &= 0xfe;
	}
	if(K2)
	{
		OLED_WriteCmd(0x81);//--设置对比度 
		OLED_WriteCmd(0x8f);//  对比度的值（值越大越亮）
		
		CCON &= 0xfd;
	}
	if(K3)
	{
		OLED_WriteCmd(0x81);//--设置对比度 
		OLED_WriteCmd(0x1f);//  对比度的值（值越大越亮）
		
		CCON &= 0xfb;
	}
	if(K4)
	{
		OLED_WriteCmd(0x81);//--设置对比度 
		OLED_WriteCmd(0x08);//  对比度的值（值越大越亮）
		
		CCON &= 0xf7;
	}

	
	
}




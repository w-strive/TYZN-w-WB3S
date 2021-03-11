#include "time.h"

/*
	说明：STC8A8K系列共有 5个 16位定时器/计数器
	0\1\4
	
	定时器2用作 串口1产生波特率
	定时器3用作 串口3产生波特率
*/

/* 定时器0初始化 */
void Timer0_Init(void)		//2毫秒@27.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x6C;		//设置定时初值
	TH0 = 0xEE;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	
	ET0 = 1;		//打开中断
//	IP |= 0x02;	//设置优先级为较低
	
	TR0 = 1;		//定时器0开始计时
}



/* 定时器1初始化 */
void Timer1_Init(void)		//100微秒@30.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0xff;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
}



/* 定时器4初始化 */
//用来使用蜂鸣器
void Timer4_Init(void)	//20毫秒@24.000MHz
{
	T4T3M &= 0xDF;		//定时器时钟12T模式
	T4L = 0xC0;			//设置定时初值
	T4H = 0x63;			//设置定时初值
	
//	IE2 |= ET4;			//使能中断
	
//	T4T3M |= 0x80;		//定时器4开始计时
}



void TM4_Isr() interrupt 20
{
	static char count = 0;
	
	/* 判断蜂鸣器是否触发 */
	if(BEEP_button_bit != 0)
	{
		count++;
		if(count >= 4)  //4*20ms = 80ms
		{
			BEEP = 0;	//关闭蜂鸣器
			BEEP_button_bit = 0;
			count = 0;
		}
	}
	
	AUXINTIF &= ~T4IF;	//清中断
	
}







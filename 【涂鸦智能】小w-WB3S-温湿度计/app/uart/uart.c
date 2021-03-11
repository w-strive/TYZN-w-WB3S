#include "uart.h"


uchar S3_uart_dat = 0; //数据
bit busy = 0;



/*
	函数名称：UsartInit
	功能：	串口初始化
	参数：Baudrate:当前设置的波特率
			
	返回值：无
*/

void Uart1_Init(void)		//9600bps@27.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T
	T2L = 0xC5;		//设定定时初值
	T2H = 0xFF;		//设定定时初值

	ES = 1;			//打开串口中断
	PS = 1;
	
	AUXR |= 0x10;		//启动定时器2
}

/*******************************************
**	函  数：Uart3_Init
**	功  能：串口3初始化 模式0
**	参  数：无
**	返回值：无
********************************************/
void Uart3_Init(void)		//9600bps@27.000MHz
{
	S3CON = 0x10;		//8位数据,可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M |= 0x02;		//定时器3时钟为Fosc,即1T
	T3L = 0x41;		//设定定时初值
	T3H = 0xFD;		//设定定时初值
	
	IE2 |= 0x08;	//打开中断
	
	T4T3M |= 0x08;		//启动定时器3
}



void Uart_Sendbyte(uchar c)
{
	ES = 0;
	SBUF = c;
	while(!TI);
	TI = 0;
	ES = 1;
}

/* 串口3发送 */
void Uart3_Sendbyte(uchar c)
{
	while (busy);
    busy = 1;
	S3BUF = c;
}


char putchar(char c)
{
	Uart_Sendbyte(c);
	return c;
}


void Uart_SendNbyte(Pint8_t str)
{
	while(*str != '\0')
	{
		SBUF = *str;
		while(!TI);
		TI = 0;
		++str;
	}
}



/* 串口1中断 */
void UART1_Isr() interrupt 4
{
	uchar buf;
	
	if(RI)
	{
		RI = 0;
		buf = SBUF;
		if(buf == 0x56)
		{
			Delay_ms(100);
			IAP_CONTR = 0x60;
		}
		
		
//		ESP_receive_buff[ESP_count] = buf;
//		ESP_count++;
//		/* ESP8266基本返回结果*/
//		if(ESP_count >= 4)
//		{
//			if(ESP_receive_buff[ESP_count - 1] == 0x0A && ESP_receive_buff[ESP_count - 2] == 0x0D && 
//			   ESP_receive_buff[ESP_count - 3] == 0x4B && ESP_receive_buff[ESP_count - 4] == 0x4F)
//			//0D 0A 4F 4B 0D 0A 
//			{
//				ESP_count = 0;
//				ESP8266_bit = 1;
//			}
//		}

		
	}

}


/* 串口3中断 */
void Uart3_Isr() interrupt 17
{
	
	if(S3CON & 0x02)	//发送中断
	{
		S3CON &= ~0x02;	//清中断	
		busy = 0;
	}
	
	if(S3CON & 0x01)	//接收中断
	{	
		S3CON &= ~0x01;	//清中断
		S3_uart_dat = S3BUF;
		uart_receive_input(S3_uart_dat);

//		IE2 &= ~0x08;	//关闭中断
	}
	
}


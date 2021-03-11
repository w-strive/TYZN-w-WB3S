#include "spi.h"


/**
**	注意：以15F和15L开头且有SPI功能的芯片，只支持”spi主机模式“。
**		  以15W开头且有spi功能的芯片，SPI主/从模式均支持
**		  因为15W与15F/L系列分频系数不一样所以需要传入相关参数
**		  15W/8A		15F/L
**	1		 4			  4
**	2		 8			 16
**	3		16			 64
**	4		32			 128
**	函数：SPI_Init
**	功能：硬件SPI   做主机初始化
**	参数：num:分频系数  (1~4  1最小)
**	返回值：无
**/
void SPI_Init(uchar num)
{

	switch(num)
	{
		case 0x00:
			SPCTL = 0xd0;
			break;
		case 0x01:
			SPCTL = 0xd1;
			break;
		case 0x02:
			SPCTL = 0xd2;
			break;
		case 0x03:
			SPCTL = 0xd3;
			break;
	}
	SPSTAT = 0xc0;                      //清除中断
//	ESPI = 1;  //允许SPI中断
//	PSPI = 1;  //SPI中断优先级高
}

/*******************************************
**	函数：SPI_which
**	功能：SPI引脚映射选择
**	参数：num: 0,1,2,3 (分别对应P1\P2\P7\P3)
**	返回值：无
********************************************/
void SPI_which(uchar num)
{
	P_SW1 &= 0xf3;	//先恢复默认设置
	switch(num)
	{
		case 0:	//SS/P1.2  MOSI/P1.3  MISO/P1.4  SCLK/P1.5
			break;
		case 1:	//SS_2/P2.2  MOSI_2/P2.3  MISO_2/P2.4  SCLK_2/P2.5
			P_SW1 |= 0x04;		
			break;
		case 2:	//SS_3/P7.4  MOSI_3/P7.5  MISO_3/P7.6  SCLK_3/P7.7
			P_SW1 |= 0x08;
			break;
		case 3:	//SS_4/P3.5  MOSI_4/P3.4  MISO_4/P3.3  SCLK_4/P3.2
			P_SW1 |= 0x0c;
			break;
	}
}




/****************************************
**	函数：SPI_SendRead
**	功能：SPI发送函数
**	参数：dat: 数据
**	返回值：无
****************************************/
uchar SPI_SendRead(uchar dat)
{
	
	#ifdef Enable_hardware_spi

		SPSTAT|=0xc0;
		SPDAT=dat;
	
		while(!(spi_complete));	//等待发送成功
	
		return SPDAT;
	
	#else
	
		uchar bit_ctr;
		for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
		{
			MOSI = (dat & 0x80);         // output 'uchar', MSB to MOSI
			dat = (dat << 1);           // shift next bit into MSB..
			SCK = 1;                      // Set SCK high..
			_nop_();
			dat |= MISO;       		  // capture current MISO bit
			_nop_();
			SCK = 0;            		  // ..then set SCK low again
		}
		return(dat);           		  // return read uchar
	
	#endif
}


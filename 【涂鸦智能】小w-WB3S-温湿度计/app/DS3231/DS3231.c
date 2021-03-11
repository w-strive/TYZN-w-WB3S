#include "DS3231.h"

DS3231_struct DS3231_;


/*******************************************
**	函  数：DS3231_Init
**	功  能：DS3231初始化
**	参  数：无
**	返回值：无
********************************************/
void DS3231_Init(void)
{
//	INTCLKO |= 0x20;	//使能外部中断3
	
	DS3231_readReg_time();

}

/*******************************************
**	函  数：BCD_HEX
**	功  能：BCD转HEX
**	参  数：无
**	返回值：无
********************************************/
uint8_t BCD_HEX(uint8_t dat)
{
	dat = (dat >> 4) * 10 + (dat & 0x0f);
	
	return dat;
}

/*******************************************
**	函  数：HEX_BCD
**	功  能：HEX转BCD
**	参  数：无
**	返回值：无
********************************************/
uint8_t HEX_BCD(uint8_t dat)
{
	dat = ((dat/10) << 4) | (dat % 10);
	
	return dat;
}

/*******************************************
**	函  数：DS3231_readReg_time
**	功  能：读取DS3231时间寄存器
**	参  数：无
**	返回值：无
********************************************/
void DS3231_readReg_time(void)
{
	uchar buff[7];
	
	/*  地址指针自增 00h ~ 06h  */
	if(DS3231_I2C_Read_multibyte(DS3231_WriteAddr,DS3231_SECOND,buff,7) == 0)
	{
		/* 数据处理 */
		DS3231_.second = BCD_HEX(buff[0]);
		DS3231_.minute = BCD_HEX(buff[1]);
		DS3231_.hour   = buff[2] & 0x0f;
		DS3231_.week   = buff[3];
		DS3231_.day    = BCD_HEX(buff[4]);
		DS3231_.month  = BCD_HEX(buff[5] & 0x1f);
		DS3231_.year   = BCD_HEX(buff[6]);
	}
	else
		printf("DS3231_err1\r\n");
	
}


/*******************************************
**	函  数：DS3231_readReg_Alarm1
**	功  能：读取DS3231 闹钟1和闹钟2 时间寄存器
**	参  数：无
**	返回值：无
********************************************/
void DS3231_readReg_Alarm(void)
{
	uchar buff[7];
	
	/*  地址指针自增 07h ~ 0Dh  */
	if(DS3231_I2C_Read_multibyte(DS3231_WriteAddr,DS3231_SALARM1ECOND,buff,7) == 0)
	{
		/* 数据处理 */
		DS3231_.Alarm_second  = BCD_HEX(buff[0] & 0x7f);
		
		DS3231_.Alarm1_minute = BCD_HEX(buff[1] & 0x7f);
		DS3231_.Alarm1_hour   = BCD_HEX(buff[2] & 0x1f);
		DS3231_.Alarm1_week   = BCD_HEX(buff[3] & 0x3f);
		
		DS3231_.Alarm2_minute = BCD_HEX(buff[4] & 0x7f);
		DS3231_.Alarm2_hour   = BCD_HEX(buff[5] & 0x1f);
		DS3231_.Alarm2_week   = BCD_HEX(buff[6] & 0x3f);
		
	}
	else
		printf("DS3231_err2\r\n");
	
}


/*******************************************
**	函  数：DS3231_readReg_control
**	功  能：读取DS3231 控制寄存器
**	参  数：无
**	返回值：无
********************************************/
void DS3231_readReg_control(void)
{
	uchar buff;
	
	/*  0eh  */
	if(DS3231_I2C_Read_onebyte(DS3231_WriteAddr,DS3231_CONTROL,&buff) == 0)
	{
		/* 数据处理 */
		DS3231_.Control = buff;
	}
	else
		printf("DS3231_err3\r\n");
	
}

/*******************************************
**	函  数：DS3231_readReg_Status
**	功  能：读取DS3231 状态寄存器
**	参  数：无
**	返回值：无
********************************************/
void DS3231_readReg_Status(void)
{
	uchar buff;
	
	/*  0fh  */
	if(DS3231_I2C_Read_onebyte(DS3231_WriteAddr,DS3231_STATUS,&buff) == 0)
	{
		/* 数据处理 */
		DS3231_.Status = buff;
	}
	else
		printf("DS3231_err4\r\n");
	
}


/*******************************************
**	函  数：DS3231_readReg_Temp
**	功  能：读取DS3231 温度寄存器
**	参  数：无
**	返回值：无
********************************************/
void DS3231_readReg_Temp(void)
{
	uchar buff[2];
	
	/*  11h ~ 12h */
	if(DS3231_I2C_Read_multibyte(DS3231_WriteAddr,DS3231_TEMPERATUREH,buff,2) == 0)
	{
		/* 数据处理 */
		DS3231_.Temp = buff[0];
		DS3231_.Temp *= 100;
		if(buff[1] != 0xc0)
			DS3231_.Temp += (buff[1] >> 6) * 25;
	}
	else
		printf("DS3231_err5\r\n");
	
}



/*******************************************
**	函  数：DS3231_writeReg_Time
**	功  能：写入DS3231 当前时间寄存器
**	参  数：无
**	返回值：无
********************************************/
void DS3231_writeReg_Time(void)
{
	uchar buff[7];
	
	/* 数据处理 */
	buff[0] = DS3231_.second;
	buff[1] = DS3231_.minute;
	buff[2] = DS3231_.hour;
	buff[3] = DS3231_.week;
	buff[4] = DS3231_.day;
	buff[5] = DS3231_.month;
	buff[6] = DS3231_.year;
	
	/*  00h ~ 06h */
	if(I2C_Write_multibyte(DS3231_WriteAddr,DS3231_SECOND,buff,7) == 0)
	{
		
	}
	else
		printf("DS3231_err6\r\n");
	
}







/*******************************************
**	函  数：DS3231_INT3_Isr
**	功  能：外部中断3入口函数
**	参  数：无
**	返回值：无
********************************************/
void DS3231_INT3_Isr() interrupt 11
{
	
	
	
}


#include "sht3x.h"

//-- Defines -------------------------------------------------------------------
// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

//-- Global variables ----------------------------------------------------------
u8 _i2cWriteHeader;
u8 _i2cReadHeader;


/******************************************************************************
* 函  数：SHT3x_I2C_Write_oneword
* 功  能：向I2C写入一个字
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  Data：写入数据
* 返回值：bit类型（1：失败  0：成功）
* 备  注：SHT3x专用I2C写操作函数   高位在前  低为在后
*******************************************************************************/
bit SHT3x_I2C_Write_oneword(uchar Device_Address,uint Data)
{

    I2C_Start();                  //起始信号
    I2C_SendByte(Device_Address); //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		printf("1");
		return 1; //从机地址写入失败
	}
    I2C_SendByte((u8)(Data >> 8));       	  //内部寄存器数据，
	if(I2C_RecvACK())
	{
		printf("2");
		return 1; //从机地址写入失败
	}
	I2C_SendByte((u8)Data);       	  //内部寄存器数据，
	if(I2C_RecvACK())
	{
		printf("3");
		return 1; //从机地址写入失败
	}
    I2C_Stop();                   //发送停止信号

	return 0;
}



/******************************************************************************
* 函  数：SHT3x_I2C_Read_oneword
* 功  能：从I2C读取一个字
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  *Data：读取数据存放地址
* 返回值：bit类型（1：失败  0：成功）
* 备  注：SHT3x专用I2C写操作函数   高位在前  低为在后
*******************************************************************************/
bit SHT3x_I2C_Read_oneword(uchar Device_Address,uint* Data)
{

	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address);  //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		printf("4");
		return 1; //从机地址写入失败
	}
	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address+1);//发送设备地址+读信号
	if(I2C_RecvACK())
	{
		printf("5");
		return 1; //从机地址写入失败
	}
	*Data = I2C_RecvByte();        //读出寄存器数据  低8位
	I2C_SendACK(0);				   //发送应答
	*Data |= ((uint)I2C_RecvByte() << 8);//读出寄存器数据  高8位
	I2C_SendACK(1);				   //发送非应答
	I2C_Stop();                    //停止信号
	
	return 0;
}

/******************************************************************************
* 函  数：SHT3x_I2C_Read_multibyte
* 功  能：从I2C读取多个字节
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  *Data：读取数据存放地址
* 返回值：bit类型（1：失败  0：成功）
* 备  注：SHT3x专用I2C写操作函数   高位在前  低为在后
*******************************************************************************/
bit SHT3x_I2C_Read_multibyte(uchar Device_Address,uint REG_Address,uchar* Data,uchar len)
{

	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address);  //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		printf("1 /");
		return 1; //从机地址写入失败
	}
	I2C_SendByte(REG_Address >> 8);       	  //内部寄存器数据，
	if(I2C_RecvACK())
	{
		printf("2 /");
		return 1; //从机地址写入失败
	}
	I2C_SendByte(REG_Address);       	  //内部寄存器数据，
	if(I2C_RecvACK())
	{
		printf("3 /");
		return 1; //从机地址写入失败
	}

	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address+1);//发送设备地址+读信号
	if(I2C_RecvACK())
	{
		printf("4 /");
		return 1; //从机地址写入失败
	}
	while(len)
	{
		len--;
		
		*Data = I2C_RecvByte();        //读出寄存器数据  低8位
		if(len != 0)
			I2C_SendACK(0);				   //发送应答
		else
			I2C_SendACK(1);				   //发送非应答
		Data++;
	}

	I2C_Stop();                    //停止信号
	
	return 0;
}


/*
 * crc8校验函数，data为要校验的数据，len为要校验的数据的字节数
 */
uint8_t crc8(uint8_t *dat, int len)
{
	uint8_t crc_ = 0xFF;
	int i, j;

	for (i=0; i<len; ++i) 
	{
	crc_ ^= *dat++;

	for (j=0; j<8; ++j) 
	{
	crc_ = ( crc_ & 0x80 )? (crc_ << 1) ^ POLYNOMIAL: (crc_ << 1);
	}
	}

	return crc_;
}



/******************************************************************************
* 函  数：SHT3x_I2C_ReadTheWords
* 功  能：单词读取
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  *Data：读取数据存放地址
* 返回值：bit类型（1：失败  0：成功）
* 备  注：SHT3x专用I2C写操作函数   高位在前  低为在后
*******************************************************************************/
bit SHT3x_I2C_ReadTheWords(uint REG_Address,uchar* Data,uchar len)
{

	I2C_Start();                   //起始信号
	I2C_SendByte(SHT3x_Write_Addr);  //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
	I2C_SendByte(REG_Address >> 8);       	  //内部寄存器数据，
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
	I2C_SendByte(REG_Address);       	  //内部寄存器数据，
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
	I2C_Stop();                    //停止信号

	
	Delay_ms(1);
	I2C_Start();                   //起始信号
	I2C_SendByte(SHT3x_Write_Addr+1);//发送设备地址+读信号
	I2C_RecvACK();
	Delay_ms(3);
	
	/* 判断是否带时钟伸展 */
	if((REG_Address >> 8) == 0x2c)	//带有
	{
		SCL = 1;
		while(SCL == 0);	//等待变高
		SCL = 0;
	}
	else
	{
		I2C_Start();                   //起始信号
		I2C_SendByte(SHT3x_Write_Addr+1);//发送设备地址+读信号
		if(I2C_RecvACK())
		{
			printf("111\r\n");
			return 1; //从机地址写入失败
		}
	}
	while(len)
	{
		len--;
		
		*Data = I2C_RecvByte();        //读出寄存器数据  低8位
		if(len != 0)
			I2C_SendACK(0);				   //发送应答
		else
			I2C_SendACK(1);				   //发送非应答
		Data++;
	}

	I2C_Stop();                    //停止信号
	
	return 0;
}




//==============================================================================
void SHT3X_Init(void)
{
	/* 硬复位 */
//	SHT3x_RET = 0;
//	Delay_ms(100);
//	SHT3x_RET = 1;
//	Delay_ms(50);
	
	//进入周期测量模式
//	while(SHT3x_I2C_Write_oneword(SHT3x_Write_Addr,CMD_MEAS_PERI_10_M) == 1)
//	{
//		Delay_ms(1);
//	}
	//进入单次测量
//	while(SHT3x_I2C_ReadTheWords(CMD_MEAS_CLOCKSTR_M,buf,6) == 1)
//	{
//		Delay_ms(1);
//	}
	printf("SHT30_OK\r\n");
	
	Delay_ms(200);
}

////==============================================================================
//ft SHT3X_CalcTemperature(u16 rawValue){
////==============================================================================
//  // calculate temperature [°C]
//  // T = -45 + 175 * rawValue / (2^16-1)
//  return 175 * (ft)rawValue / 65535 - 45;
//}

////==============================================================================
//ft SHT3X_CalcHumidity(u16 rawValue){
////==============================================================================
//  // calculate relative humidity [%RH]
//  // RH = rawValue / (2^16-1) * 100
//  return 100 * (ft)rawValue / 65535;
//}

////==============================================================================
//u16 SHT3X_CalcRawTemperature(ft temperature){
////==============================================================================
//  // calc raw value from a temperature [°C]
//  // rawValue = (T + 45) / 175 * (2^16-1)
//	return (u16)((temperature + 45) / 175 * 65535);
//}

////==============================================================================
//u16 SHT3X_CalcRawHumidity(ft humidity){
////==============================================================================
//  // calc raw value from a relative humidity [%RH]
//  // rawValue = RH / 100 * (2^16-1)
//	return (u16)(humidity / 100 * 65535);
//}

#include "VEML6030.h"


/******************************************************************************
* 函  数：VEML6030_I2C_Write_oneword
* 功  能：向I2C写入一个字
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  Data：写入数据
* 返回值：bit类型（1：失败  0：成功）
* 备  注：VEML6030专用I2C写操作函数   低位在前  高为在后
*******************************************************************************/
bit VEML6030_I2C_Write_oneword(uchar Device_Address,uchar REG_Address,uint Data)
{

    I2C_Start();                  //起始信号
    I2C_SendByte(Device_Address); //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		printf("1");
		return 1; //从机地址写入失败
	}
    I2C_SendByte(REG_Address);    //内部寄存器地址，
	if(I2C_RecvACK())
	{
		printf("2");
		return 1; //从机地址写入失败
	}
    I2C_SendByte(Data);       	  //内部寄存器数据，
	if(I2C_RecvACK())
	{
		printf("3");
		return 1; //从机地址写入失败
	}
	I2C_SendByte(Data >> 8);       	  //内部寄存器数据，
	if(I2C_RecvACK())
	{
		printf("4");
		return 1; //从机地址写入失败
	}
    I2C_Stop();                   //发送停止信号

	return 0;
}



/******************************************************************************
* 函  数：VEML6030_I2C_Read_oneword
* 功  能：从I2C读取一个字
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  *Data：读取数据存放地址
* 返回值：bit类型（1：失败  0：成功）
* 备  注：VEML6030专用I2C写操作函数   低位在前  高为在后
*******************************************************************************/
bit VEML6030_I2C_Read_oneword(uchar Device_Address,uchar REG_Address,uint* Data)
{

	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address);  //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		printf("5");
		return 1; //从机地址写入失败
	}
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始
	if(I2C_RecvACK())
	{
		printf("6");
		return 1; //从机地址写入失败
	}
	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address+1);//发送设备地址+读信号
	if(I2C_RecvACK())
	{
		printf("7");
		return 1; //从机地址写入失败
	}
	*Data = I2C_RecvByte();        //读出寄存器数据  低8位
	I2C_SendACK(0);				   //发送应答
	*Data |= ((uint)I2C_RecvByte() << 8);//读出寄存器数据  高8位
	I2C_SendACK(1);				   //发送非应答
	I2C_Stop();                    //停止信号
	
	return 0;
}





void VEML6030_Init(void)
{
	/* 设置 */
	if(VEML6030_I2C_Write_oneword(VEML6030_Write_Addr,CONF_CMD,0x0000) == 0)
	{
		printf("VEML6030_OK\r\n");
	}
	
	/* 设置省电模式 */
	if(VEML6030_I2C_Write_oneword(VEML6030_Write_Addr,PWR_CMD,0x0007) == 0)
	{
		printf("VEML6030_OK\r\n");
	}
	
}


uint VEML6030_Read_reg_WHITE(void)
{
	uint value = 0;
	
	if(VEML6030_I2C_Read_oneword(VEML6030_Write_Addr,WHITE_CMD,&value) == 0)
	{
		//printf("白色光:%d\r\n",value);
		
		/* 量化0~1000 */
		value = ((float)value / 65536.0f) * 1000;
		
	}
	
	return value;
}



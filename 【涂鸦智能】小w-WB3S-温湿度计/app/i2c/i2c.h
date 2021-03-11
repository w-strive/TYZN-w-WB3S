#ifndef _i2c_H_
#define _i2c_H_

#include "public.h"



/*
	2种方式只能同时用1种，不可以2种都不注释
*/
#define I2C_MODE_Simulation		//使用模拟I2C
//#define I2C_MODE_hardware		//使用外设I2C


sbit    SCL=P3^2;			//IIC时钟引脚定义
sbit    SDA=P3^3;			//IIC数据引脚定义

	
	
#ifdef I2C_MODE_Simulation

	#define Read_I2C_Busy_bit (I2CMSST & 0x80)	//读取I2C控制器状态
	#define Read_I2C_MSIF_bit (I2CMSST & 0x40)	//读取I2C中断请求
	#define Read_I2C_MSACKI_bit (I2CMSST & 0x02)	//读取I2C发送后的接收应答

	extern void Init_I2C(uchar i2c_io_switch);
	
#endif


void I2C_Start(void);
void I2C_Stop(void);
bit I2C_RecvACK(void);
void I2C_SendByte(uchar dat);
uchar I2C_RecvByte(void);
void I2C_SendACK(bit ACK);



bit I2C_Write_onebyte(uchar Device_Address,uchar REG_Address,uchar Data);
bit I2C_Write_multibyte(uchar Device_Address,uchar REG_Address,uchar* Data,uchar len);
bit I2C_Read_onebyte(uchar Device_Address,uchar REG_Address,uchar* Data);
bit I2C_Read_multibyte(uchar Device_Address,uchar REG_Address,uchar* Data,uchar len);


#endif

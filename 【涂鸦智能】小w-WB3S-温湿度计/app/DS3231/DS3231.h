#ifndef _ds3231_H_
#define _ds3231_H_


#include "public.h"


/*******************************************************/
//** 引脚定义
//

//sbit DS3231_RST = P2^7;
sbit DS3231_INT = P3^7;

/*******************************************************/
//** 全局宏
//
#define DS3231_WriteAddr 0xD0 	//器件写地址

#define BSY 2 //忙
#define OSF 7 //振荡器停止标志

#define DS3231_SECOND 0x00 			//秒
#define DS3231_MINUTE 0x01 			//分
#define DS3231_HOUR   0x02 			//时
#define DS3231_WEEK   0x03 			//星期
#define DS3231_DAY    0x04 			//日
#define DS3231_MONTH  0x05 			//月
#define DS3231_YEAR   0x06 			//年
//闹铃1
#define DS3231_SALARM1ECOND 0x07 	//秒
#define DS3231_ALARM1MINUTE 0x08 	//分
#define DS3231_ALARM1HOUR   0x09 	//时
#define DS3231_ALARM1WEEK   0x0A 	//星期/日
//闹铃2
#define DS3231_ALARM2MINUTE 0x0b	//分
#define DS3231_ALARM2HOUR   0x0c 	//时
#define DS3231_ALARM2WEEK   0x0d 	//星期/日

#define DS3231_CONTROL 0x0e 		//控制寄存器
#define DS3231_STATUS  0x0f 		//状态寄存器

#define DS3231_XTAL 0x10 			//晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11 	//温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12 	//温度寄存器低字节(高2位)


//I2C驱动函数宏
#define DS3231_I2C_Read_onebyte    I2C_Read_onebyte		//单个读取
#define DS3231_I2C_Read_multibyte  I2C_Read_multibyte	//多个连续读取





/*******************************************************/
//** 结构体定义
//
typedef struct 
{
	uint8_t second;	//秒
	uint8_t minute;	//分
	uint8_t hour;	//时
	uint8_t week;	//星期
	uint8_t day;	//日
	uint8_t month;	//月
	uint8_t year;	//年
	
	uint8_t updat_time; //更新时间标志
	
	uint8_t Alarm_second;	//闹钟 秒
	
	uint8_t Alarm1_minute;	//闹钟1 分
	uint8_t Alarm1_hour;	//闹钟1 时
	uint8_t Alarm1_week;	//闹钟1 日期
	
	uint8_t Alarm2_minute;	//闹钟2 分
	uint8_t Alarm2_hour;	//闹钟2 时
	uint8_t Alarm2_week;	//闹钟2 日期
	
	uint8_t Control;		//控制寄存器
	uint8_t Status;			//状态寄存器
	
	uint16_t Temp;			//温度(扩大100倍)
	
}DS3231_struct;





/*******************************************************/
//** 函数声明
//
extern DS3231_struct DS3231_;

void DS3231_Init(void);

void DS3231_readReg_time(void);
void DS3231_readReg_Alarm(void);
void DS3231_readReg_control(void);
void DS3231_readReg_Status(void);
void DS3231_readReg_Temp(void);

void DS3231_writeReg_Time(void);

#endif

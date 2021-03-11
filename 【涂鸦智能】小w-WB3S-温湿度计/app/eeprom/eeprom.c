#include "eeprom.h"


/*

	EPROM 操作 最短时间  最长时间
		编程 	 6us 	   7.5us
		擦除 	 4ms 	   6ms


	擦除操作：以扇区为最小单位（每个扇区512字节）
	读操作	: 按单个字节操作
	写操作	：按单个字节操作


*/

static void IapIdle(void)
{
    IAP_CONTR = 0;           //关闭IAP功能
    IAP_CMD = 0;             //清除命令寄存器
    IAP_TRIG = 0;            //清除触发寄存器
    IAP_ADDRH = 0x80;        //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

//IAP读操作
char IapRead(int addr)
{
    char dat;

    IAP_CONTR = WT_30M;      //使能IAP
    IAP_CMD = 1;             //设置IAP读命令
    IAP_ADDRL = addr;        //设置IAP低地址
    IAP_ADDRH = addr >> 8;   //设置IAP高地址
    IAP_TRIG = 0x5a;         //写触发命令(0x5a)
    IAP_TRIG = 0xa5;         //写触发命令(0xa5)
	_nop_();
    dat = IAP_DATA;          //读IAP数据
    IapIdle();               //关闭IAP功能

    return dat;
}

//IAP写操作
void IapProgram(int addr, char dat)
{
    IAP_CONTR = WT_30M;      //使能IAP
    IAP_CMD = 2;             //设置IAP写命令
    IAP_ADDRL = addr;        //设置IAP低地址
    IAP_ADDRH = addr >> 8;   //设置IAP高地址
    IAP_DATA = dat;          //写IAP数据
    IAP_TRIG = 0x5a;         //写触发命令(0x5a)
    IAP_TRIG = 0xa5;         //写触发命令(0xa5)
    _nop_();
    IapIdle();               //关闭IAP功能
}

//IAP擦除操作
void IapErase(int addr)
{
    IAP_CONTR = WT_30M;      //使能IAP
    IAP_CMD = 3;             //设置IAP擦除命令
    IAP_ADDRL = addr;        //设置IAP低地址
    IAP_ADDRH = addr >> 8;   //设置IAP高地址
    IAP_TRIG = 0x5a;         //写触发命令(0x5a)
    IAP_TRIG = 0xa5;         //写触发命令(0xa5)
    _nop_();                 //
    IapIdle();               //关闭IAP功能
}



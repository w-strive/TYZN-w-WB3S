#ifndef _spi_H_
#define _spi_H_


#include "public.h"


/* 取消注释则使用IO口模拟通信，不取消则采用硬件驱动 */
#define Enable_hardware_spi


#define spi_complete (SPSTAT & 0x80)    //判断SPI传输是否完成
#define spi_write    (SPSTAT & 0x40)    //判断是否写冲突（是否忙状态）


void SPI_Init(uchar num);
void SPI_which(uchar num);
uchar SPI_SendRead(uchar dat);


#endif

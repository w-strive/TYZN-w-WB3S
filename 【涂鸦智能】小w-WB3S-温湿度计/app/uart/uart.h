#ifndef _uart_H_
#define _uart_H_


#include "public.h"



void Uart1_Init(void);
void Uart3_Init(void);
void Uart_Sendbyte(uchar c);
void Uart_SendNbyte(Pint8_t);

void Uart3_Sendbyte(uchar c);



#endif

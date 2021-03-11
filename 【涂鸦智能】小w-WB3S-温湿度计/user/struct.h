#ifndef _struct_H_
#define _struct_H_

#include "STC8.h"



//三轴整型
typedef struct
{
	int X;
	int Y;
	int Z;
}INT_XYZ;

//三轴浮点型
typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT_XYZ;




//AD采样
typedef struct
{
	int YM_1;
	int YM_2;
	int FX_1;
	int FX_2;
	
	int Power;

}AD_struct;



//无线模块
typedef struct
{
	uchar nrf_TS;	//每秒发送成功次数
	uchar nrf_TA;	//每秒发送总次数
	
	uchar PLOS_CNT;	//丢包数
	uchar ARC_CNT;	//重发次数
	
	
	
}NRF_struct;




//按键
//typedef struct
//{
//	/* 短按标志 */
//	uchar K_up_Short_bit:1;
//	uchar K_down_Short_bit:1;
//	uchar K_left_Short_bit:1;
//	uchar K_right_Short_bit:1;
//	
//	/* 长按标志 */
//	uchar K_up_Long_bit:1;
//	uchar K_down_Long_bit:1;
//	uchar K_left_Long_bit:1;
//	uchar K_right_Long_bit:1;
//	
//	/* 计数 主要看定时器多长时间中断一次*/
//	uchar K_up_count;
//	uchar K_down_count;
//	uchar K_left_count;
//	uchar K_right_count;
//	

//}KEY_struct;





#endif

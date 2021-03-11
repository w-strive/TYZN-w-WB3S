#include "adc.h"

/*
	STC8A
	内部参考电压: 1343 mV (参考范围: 1270~1410mV)
*/

float ADC_ratio = ADC_Scaler_num / ADC_Scaler_REF;

/*******************************************
**	函数：ADC_Init
**	功能：ADC初始化
**	参数：无
**	返回：无
**  说明：配置在ADC头文件里面
********************************************/
void ADC_Init(void)
{

#if (RESFMT == 0)
	ADCCFG &= 0x0f;
#else
	ADCCFG |= 0x20;
#endif
	
	ADCCFG = (ADCCFG & 0xf0) | (SPEED - 1);
	
	//打开ADC电源
	ADC_CONTR = 0x80;
	//清空转换结果寄存器
	ADC_RES = 0;ADC_RESL = 0;
	//ADC中断
	EADC = ADC_INT;
	
}


/*******************************************
**	函数：ADC_Conversion
**	功能：读取ADC值
**	参数：num-对应ADC通道号
**	返回：转换后的ADC值
**  说明：无
********************************************/
uint ADC_Conversion(uchar num)
{
	uint value = 0;
	
	//清除转换结果
	ADC_RES  = 0;	ADC_RESL = 0;
	
	//选择通道
	ADC_CONTR = 0x80 | num;
	
	ADC_CONTR |= 0x40;	//启动ADC转换

#if (ADC_INT == 0)
	
	_nop_();
	_nop_();
	while(!(ADC_CONTR & 0x20)); //等待转换完成
	ADC_CONTR &= ~0x20;		//清除转换完成标志位
	
	#if (RESFMT == 0)
		value = ADC_RES;//结果左处理 12位
		value <<= 4;
		value |= (ADC_RESL >> 4);
	#else
		value = ADC_RES;//结果右处理 12位
		value <<= 8;
		value |= ADC_RESL;
	#endif
	
	//ADC_CONTR = 0;	//关闭电源
	
#endif

	return value;
}


/*******************************************
**	函数：ADC_Isr
**	功能：ADC转换中断
**	参数：无
**	返回：无
**  说明：无
********************************************/
void ADC_Isr() interrupt 5
{
	ADC_CONTR &= ~0x20;	//清除标志位
	
	#if (RESFMT == 0)
//		value = ADC_RES;//结果左处理 12位
//		value <<= 4;
//		value |= (ADC_RESL >> 4);
	#else
//		value = ADC_RES;//结果右处理 12位
//		value <<= 8;
//		value |= ADC_RESL;
	#endif
}




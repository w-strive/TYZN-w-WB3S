#ifndef _adc_h_
#define _adc_h_

#include "public.h"



/*** 配置ADC设置 ***/
#define RESFMT  1   // 0：结果左对齐  1：结果右对齐
#define SPEED   8   // ADC时钟频率（1~16）  计算:SYSclk/2/16/(ADC_SPEED+1)  

#define ADC_INT     0   // ADC转换中断  0：关闭  1：开启

/*** ADC转换通道 ***/
#define ADC0  0			//P1^0/ADC0
#define ADC10 0xa		//P0^2/ADC10
#define ADC15 0xf		//内部参考电压 1.343V


//参考源电源
#define ADC_REF  3.32f
//内部参考电压
#define ADC_Scaler_REF 1.343f
#define ADC_Scaler_num 1658		//3.3V外部电源
#define ADC_compensation 0.012f	//单位 毫伏

extern float ADC_ratio;

void ADC_Init(void);
uint ADC_Conversion(uchar num);



#endif

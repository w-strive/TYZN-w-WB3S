#include "public.h"

unsigned int F32K_HZ = 32000;	//保存内部32K频率
unsigned int SetTime = 200;	//设置掉电唤醒时间
unsigned int WKTC_HL = 0; 		//保存设置掉电寄存器值


/* 任务标志位 */
bit T_8_bit = 0;		//8ms标志
bit T_10_bit = 0;		
bit T_20_bit = 0;		
bit T_100_bit = 0;		
bit T_200_bit = 0;		
bit T_1000_bit = 0; 	
bit T_2000_bit = 0; 

/* 27M主频 */

sbit PROG = P6^5;


/* IO口模式设置 */
void IOmode_Init(void)
{
	/* 未使用的IO口设置为高阻模式 */
	P0M0 = 0x02;		
	P0M1 = 0x04;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x02;	//串口1 TxD推挽  蜂鸣器推挽
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	P6M0 = 0xa0;
	P6M1 = 0x00;
	P7M0 = 0x00;
	P7M1 = 0x00;
	
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;
	P5 = 0xff;
	P6 = 0xff;
	P7 = 0xff;
	/* 关闭各个模块片选 */
	OLED_CS = 1;	//关闭片选
	BEEP = 0;		//关闭蜂鸣器

	LED = 0;
	
	PROG = 1;
	
	//复位无线模块
	P2 = 0xff;
}
//-------------------------------------------------------------

//设置掉电唤醒定时器
void Set_WKTCL(void)
{
	int *F32K;
	
	//从RAM获取内部32K出厂频率
	F32K = (int idata *)0xf8;
	F32K_HZ = (*F32K);
	
	WKTC_HL = (u16)(((long)F32K_HZ * (long)SetTime) / 16000UL);	  //计算计数次数
	
	/* 设置定时器掉电唤醒时钟 */
//	WKTCL = (u8)WKTC_HL;
//	WKTCH = (u8)(WKTC_HL >> 8) | 0x80;  //赋值WKTCH寄存器并允许内部停机唤醒定时器
	
//	printf("t:%d\r\n",WKTC_HL);
	
//	P_SW2 = 0x80;
//	IRC32KCR = 0x80;		//启动内部32K IRC
//	while(!(IRC32KCR & 1));	//等待时钟稳定
//	CLKDIV = 0x00;			//时钟不分频
//	CKSEL = 0x03;			//选择内部32K
//	P_SW2 = 0x00;
	
}

void main(void)
{
	uint value = 0;
	uchar buf[6] = 0;
	uint SHT30_temperature,SHT30_humidity;
	uint dat;
	
	Delay_us(10);
	
	ELVD = 0;
	RSTCFG = 0x02;	//关闭低压检测
	VOCTRL = 0x00;	//选择内部静态电流
	CMPCR1 = 0x00;  //关闭比较器
	
	
	IOmode_Init();	//IO口状态初始化
	Delay_ms(300);	//兼容模块上电复位时间
	Uart1_Init();	//串口1初始化
	
	SPI_Init(0);	//硬件SPI初始化  4分频
	SPI_which(1);	//切换SPI硬件IO口
	ADC_Init();
	Key_Init();
	OLED_Init();	//OLED初始化

	Draw_P16x16Str(2,2,mark,2);  	//温度
	Draw_P8x16(34,2,':');
	Draw_P16x16Str(2,4,mark+64,2);  //湿度
	Draw_P8x16(34,4,':');
	
	DS3231_Init();
	VEML6030_Init();
	SHT3X_Init();
	
	wifi_protocol_init();
	Uart3_Init();
	
//	Set_WKTCL();	//启动掉电唤醒定时器
	
	Timer0_Init();	//打开定时器0
//	Timer4_Init();	//打开定时器4
	
	EA = 1;			//打开总中断
	
	while(1)
	{
	
		wifi_uart_service();
		
		/* 8ms事件 */
		if(T_8_bit)
		{
			T_8_bit = 0;
		}
		
		/* 10ms事件 */
		if(T_10_bit)
		{
			T_10_bit = 0;
		}
		
		/* 20ms事件 */
		if(T_20_bit)
		{
			/* 按键扫描 */
			Key_Updata();
			
			T_20_bit = 0;
		}
		
		/* 100ms事件 */
		if(T_100_bit)
		{
			T_100_bit = 0;
		}
		
		/* 200ms事件 */
		if(T_200_bit)
		{
			//DS3231_readReg_time();	//读取当前时间
			
			T_200_bit = 0;
		}
		
		/* 1s事件 */
		if(T_1000_bit)
		{
			switch(mcu_get_wifi_work_state())
			{
				case SMART_CONFIG_STATE:
					printf("Smart配置状态\r\n");break;
				case AP_STATE:
					printf("AP 配置状态\r\n");break;
				case WIFI_NOT_CONNECTED:
					printf("Wi-Fi 已配置但未连上路由器\r\n");break;
				case WIFI_CONNECTED:
					printf("Wi-Fi 已配置且连上路由器，局域网可控制\r\n");break;
				case WIFI_CONN_CLOUD:
					printf("已连上路由器且连接到云端，局域网和外网均可控制\r\n");break;
				case WIFI_LOW_POWER:
					printf("Wi-Fi 设备处于低功耗模式\r\n");break;
				case SMART_AND_AP_STATE:
					printf("Wi-Fi 设备处于 Smart 和 AP 共存配置状态\r\n");break;
			}
			
			
			DS3231_readReg_time();
			VEML6030_Read_reg_WHITE();

			value = ADC_Conversion(ADC10);
			value = ((float)value / ADC_ratio + ADC_compensation) * 200;

			Draw_P8x16(32,0,value/100 + 0x30);
			Draw_P8x16(40,0,'.');
			Draw_P8x16(48,0,value/10%10 + 0x30);
			Draw_P8x16(56,0,value%10 + 0x30);
			Draw_P8x16(64,0,'V');
			
			//SHT3x_I2C_Read_multibyte(SHT3x_Write_Addr,CMD_FETCH_DATA,buf,6);
			SHT3x_I2C_ReadTheWords(CMD_MEAS_POLLING_M,buf,6);
			//把2个8位数据拼接为一个16位的数据
			dat=((uint)buf[0] << 8) | buf[1];
			  //温度转换，将16位温度数据转化为10进制的温度数据，这里保留了一位小数。
			SHT30_temperature = (int)((175.0 * ((float)dat) / 65535.0 - 45.0) *10.0);
			dat=((uint)buf[3] << 8) | buf[4];
			SHT30_humidity = (int)((100.0 * (float)dat / 65535.0) *10.0); 
			

			mcu_dp_value_update(DPID_HUMIDITY_VALUE,SHT30_humidity/10); //VALUE型数据上报;
			mcu_dp_value_update(DPID_TEMP_CURRENT,SHT30_temperature); //VALUE型数据上报;
			mcu_dp_enum_update(DPID_BATTERY_STATE,0x02); //枚举型数据上报;
			mcu_dp_value_update(DPID_BATTERY_PERCENTAGE,0x64); //VALUE型数据上报;
			mcu_dp_value_update(DPID_TEMP_SAMPLING,0x01); //VALUE型数据上报;
			mcu_dp_value_update(DPID_HUMIDITY_SAMPLING,0x01); //VALUE型数据上报;
			mcu_dp_bool_update(DPID_CHARGE_STATE,0x01); //BOOL型数据上报;
			
			mcu_dp_value_update(DPID_BRIGHT_VALUE,VEML6030_Read_reg_WHITE()); //VALUE型数据上报;
		
			
			Draw_P8x16(42,2,SHT30_temperature/100%10 + 0x30);
			Draw_P8x16(50,2,SHT30_temperature/10%10 + 0x30);
			Draw_P8x16(58,2,'.');
			Draw_P8x16(66,2,SHT30_temperature%10 + 0x30);
			
			Draw_P8x16(42,4,SHT30_humidity/100%10 + 0x30);
			Draw_P8x16(50,4,SHT30_humidity/10%10 + 0x30);
			Draw_P8x16(58,4,'.');
			Draw_P8x16(66,4,SHT30_humidity%10 + 0x30);
			
			Draw_P8x16(10,6,DS3231_.hour/10 + 0x30);
			Draw_P8x16(18,6,DS3231_.hour%10 + 0x30);
			Draw_P8x16(26,6,':');
			Draw_P8x16(34,6,DS3231_.minute/10 + 0x30);
			Draw_P8x16(42,6,DS3231_.minute%10 + 0x30);
			Draw_P8x16(50,6,':');
			Draw_P8x16(58,6,DS3231_.second/10 + 0x30);
			Draw_P8x16(66,6,DS3231_.second%10 + 0x30);
			
			
			
			
			T_1000_bit = 0;
			
		
		}
		
		/* 2s事件 */
		if(T_2000_bit)
		{
			
//			mcu_set_wifi_mode(SMART_CONFIG);
			T_2000_bit = 0;
		}
		
		
//		if(S3_rev_bit != 0)
//		{
			/* 串口1发送 */
//			buff_con += 7;
//			for(i=0;i<buff_con;i++)
//				Uart_Sendbyte(S3BUFF[i]);
//			
//			
//			S3BUFF[0] = 0;
//			S3BUFF[1] = 0;
//			
//			S3_count = 0;
//			checks = 0;
//			buff_i = 0;
//			buff_con = 0;
//			
//			S3_rev_bit = 0;
			
//			IE2 |= 0x08;	//打开中断
//		}
		
		
		/* 结束 */
	}
	
}




/*
	定时器0中断
*/
void Time0_interrupt() interrupt 1
{
	static uchar T_8 = 1;
	static uchar T_10 = 1;
	static uchar T_20 = 1;
	static uchar T_100 = 1;
	static uchar T_200 = 1;
	static int T_1000 = 1;
	static int T_2000 = 1;
	
	if(T_8++ >= 4)
	{
		T_8_bit = 1;
		T_8 = 1;
	}
	
	if(T_10++ >= 5)
	{
		T_10_bit = 1;
		T_10 = 1;
	}
	
	if(T_20++ >= 10)
	{
		T_20_bit = 1;
		T_20 = 1;
	}

	if(T_100++ >= 50)
	{
		T_100_bit = 1;
		T_100 = 1;
	}
	
	if(T_200++ >= 100)
	{
		T_200_bit = 1;
		T_200 = 1;
	}
	
	if(T_1000++ >= 500)
	{
		T_1000_bit = 1;
		T_1000 = 1;
	}
	
	if(T_2000++ >= 1000)
	{
		T_2000_bit = 1;
		T_2000 = 1;
	}
	
//	TF0 = 0;		//清除TF0标志
}








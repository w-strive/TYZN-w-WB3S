#include "i2c.h"


#ifdef I2C_MODE_Simulation	//使用IO口模拟I2C

/******************************************************************************
* 函  数：void Delay2us(void)
* 功  能：I2C延时
* 参  数：无
* 返回值：无
* 备  注：I2C时序中延时设置，具体参见各芯片的数据手册  6050推荐最小1.3us 
		  但是会出问题，这里延时实际1.9us左右
*******************************************************************************/
static void Delay2us(void)
{
	unsigned char i;
	i = 11;		  //大约是1.3us		//  9   11
	while (--i);
}
//************************************** 	
//I2C起始信号
//**************************************
void I2C_Start(void)
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay2us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay2us();                 //延时
    SCL = 0;                    //拉低时钟线
}
//**************************************
//I2C停止信号
//**************************************
void I2C_Stop(void)
{
	SCL = 0;
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay2us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay2us();                 //延时
}
//**************************************
//从I2C总线读取应答信号
//**************************************
bit I2C_RecvACK(void)
{
	uint ucErrTime = 0;
	
	SDA = 1;
    SCL = 1;                    //拉高时钟线
    Delay2us();                 //延时
	while(SDA)
	{
		ucErrTime++;
		if(ucErrTime > 500)	//250
		{
			I2C_Stop(); 
			return 1;
		}
	}
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay2us();                 //延时
    return CY;
}
//**************************************
//向I2C总线发送一个字节数据
//**************************************
void I2C_SendByte(uchar dat)
{
    uchar i;
	SCL = 0;                //拉低时钟线
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
		_nop_();_nop_();
        SCL = 1;                //拉高时钟线
        Delay2us();             //延时
        SCL = 0;                //拉低时钟线
        Delay2us();             //延时
    }
}
//**************************************
//从I2C总线接收一个字节数据
//**************************************
uchar I2C_RecvByte(void)
{
    uchar i;
    uchar dat = 0;
    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
		SCL = 0;                //拉低时钟线
		Delay2us();             //延时
        SCL = 1;                //拉高时钟线
		dat <<= 1;
		if(SDA)
			dat++;
        Delay2us();             //延时
    }
    return dat;
}
//**************************************
//向I2C设备发送应答
//**************************************
void I2C_SendACK(bit ACK)
{
	SCL = 0;                    //拉低时钟线
	SDA = ACK;                  //写应答信号
	Delay2us();                 //延时
	SCL = 1;                    //拉高时钟线
	Delay2us();                 //延时
	SCL = 0;                    //拉低时钟线
	Delay2us();                 //延时
}

/******************************************************************************
* 函  数：I2C_Write_onebyte
* 功  能：向I2C写入一个字节
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  Data：写入数据
* 返回值：bit类型（1：失败  0：成功）
* 备  注：无
*******************************************************************************/
bit I2C_Write_onebyte(uchar Device_Address,uchar REG_Address,uchar Data)
{

    I2C_Start();                  //起始信号
    I2C_SendByte(Device_Address); //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
    I2C_SendByte(REG_Address);    //内部寄存器地址，
	I2C_RecvACK();
    I2C_SendByte(Data);       	  //内部寄存器数据，
	I2C_RecvACK();
    I2C_Stop();                   //发送停止信号

	return 0;
}


/******************************************************************************
* 函  数：I2C_Write_multibyte
* 功  能：向I2C写入多个字节
* 参  数：Device_Address：设备地址	
		  REG_Address：寄存器地址	  
		  *Data：读取数据存放地址
		  len：要读取的个数
* 返回值：bit类型（1：失败  0：成功）
* 备  注：无
*******************************************************************************/
bit I2C_Write_multibyte(uchar Device_Address,uchar REG_Address,uchar* Data,uchar len)
{

    I2C_Start();                  //起始信号
    I2C_SendByte(Device_Address); //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
    I2C_SendByte(REG_Address);    //内部寄存器地址，
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
	while(len)
	{
		I2C_SendByte(*Data);       	  //内部寄存器数据，
		if(I2C_RecvACK())
		{
			return 1; //从机地址写入失败
		}
		len--;
		Data++;
	}
    I2C_Stop();                   //发送停止信号

	return 0;
}

/******************************************************************************
* 函  数：I2C_Read_onebyte
* 功  能：从I2C读取一个字节
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  *Data：读取数据存放地址
* 返回值：bit类型（1：失败  0：成功）
* 备  注：无
*******************************************************************************/
bit I2C_Read_onebyte(uchar Device_Address,uchar REG_Address,uchar* Data)
{

	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address);  //发送设备地址+写信号
	I2C_RecvACK();
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始
	I2C_RecvACK();
	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address+1);//发送设备地址+读信号
	I2C_RecvACK();
	*Data = I2C_RecvByte();        //读出寄存器数据
	I2C_SendACK(1);				   //发送非应答
	I2C_Stop();                    //停止信号
	
	return 0;
}

/******************************************************************************
* 函  数：I2C_Read_multibyte
* 功  能：从I2C读取多个字节
* 参  数：Device_Address：设备地址	
		  REG_Address：寄存器地址	  
		  *Data：读取数据存放地址
		  len：要读取的个数
* 返回值：bit类型（1：失败  0：成功）
* 备  注：无
*******************************************************************************/
bit I2C_Read_multibyte(uchar Device_Address,uchar REG_Address,uchar* Data,uchar len)
{

	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address);  //发送设备地址+写信号
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
	I2C_Start();                   //起始信号
	I2C_SendByte(Device_Address+1);//发送设备地址+读信号
	if(I2C_RecvACK())
	{
		return 1; //从机地址写入失败
	}
	
	while(len)
	{
		*Data = I2C_RecvByte();        	//读出寄存器数据
		
		if(len == 1)
			I2C_SendACK(1);				//发送非应答
		else
			I2C_SendACK(0);				//发送应答
		
		len--;
		Data++;							//地址加1
	}

	I2C_Stop();                    		//停止信号
	
	return 0;
}




#else 	//************************************************使用外设I2C



//I2C等待
static void Wait(void)
{
	uchar i = 1;
	
    while (!(I2CMSST & 0x40) && i++);
    I2CMSST &= ~0x40;
}
//I2C起始信号
static void I2C_Start(void)
{
    I2CMSCR = 0x01;			//发送START命令
    Wait();
}
//I2C写信号
static void I2C_SendByte(uchar dat)
{
    I2CTXD = dat;			//写数据到数据缓冲区
    I2CMSCR = 0x02;			//发送SEND命令
    Wait();
}
//I2C读应答信号
static bit I2C_RecvACK(void)
{
	I2CMSCR = 0x03;			//发送读ACK命令
    Wait();
	
	return (bit)Read_I2C_MSACKI_bit;
}
//I2C读信号
static uchar I2C_RecvByte(void)
{
    I2CMSCR = 0x04;			//发送RECV命令 读取
    Wait();
    return I2CRXD;
}
//I2C写应答信号
static void I2C_SendACK(bit ACK)
{
	if(ACK)
	{
		I2CMSST = 0x01;		//设置非ACK信号
	}
	else
	{
		I2CMSST = 0x00;		//设置NAK信号
	}	
	I2CMSCR = 0x05;			//发送ACK命令
    Wait();
}
//I2C停止信号
static void I2C_Stop(void)
{
    I2CMSCR = 0x06;			//发送STOP命令
    Wait();
}


/******************************************************************************
* 函  数：void I2C_Write_onebyte(uchar Device_Address,uchar REG_Address,uchar Data)
* 功  能：向I2C写入一个字节
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  Data：写入数据
* 返回值：bit类型（1：失败  0：成功）
* 备  注：无
*******************************************************************************/
void Init_I2C(uchar i2c_io_switch)
{
	// 访问逻辑地址位于XDATA区域的特殊功能寄存器前需要
	// 将 P_SW2（BAH）寄存器的最高位（EAXFR）置 1
	P_SW2 = 0x80; 
	
    I2CCFG  = 0xc0;         //使能I2C主机模式  
    I2CMSST = 0x00;			//清空主机状态寄存器	
	
	I2CCFG |= 0x30;				//设置时钟数：1
	
	switch(i2c_io_switch)
	{
		case 0:				   break;	//SCL   SDA:P1^5  SCL:P1^4
		case 1: P_SW2 |= 0x10; break;	//SCL_2 SDA:P2^5  SCL:P2^4
		case 2: P_SW2 |= 0x20; break;	//SCL_3 SDA:P7^7  SCL:P7^6
		case 3: P_SW2 |= 0x30; break;	//SCL_4 SDA:P3^2  SCL:P3^3
	}
	
}

/******************************************************************************
* 函  数：void I2C_Write_onebyte(uchar Device_Address,uchar REG_Address,uchar Data)
* 功  能：向I2C写入一个字节
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  Data：写入数据
* 返回值：bit类型（1：失败  0：成功）
* 备  注：无
*******************************************************************************/
bit I2C_Write_onebyte(uchar Device_Address,uchar REG_Address,uchar Data)
{
	bit ack = 1;
	
	I2C_Start();
	I2C_SendByte(Device_Address);
	I2C_RecvACK();
	I2C_SendByte(REG_Address);
	I2C_RecvACK();
	I2C_SendByte(Data);
	I2C_RecvACK();
	I2C_Stop();
	
	return ack;
}

/******************************************************************************
* 函  数：void I2C_Write_onebyte(uchar Device_Address,uchar REG_Address,uchar* Data)
* 功  能：从I2C读取一个字节
* 参  数：Device_Address：设备地址	REG_Address：寄存器地址	  *Data：读取数据存放地址
* 返回值：bit类型（1：失败  0：成功）
* 备  注：无
*******************************************************************************/
bit I2C_Read_onebyte(uchar Device_Address,uchar REG_Address,uchar* Data)
{
	bit ack = 1;
	
	I2C_Start();
	I2C_SendByte(Device_Address);
	I2C_RecvACK();
	I2C_SendByte(REG_Address);
	I2C_RecvACK();
	
	I2C_Start();
	I2C_SendByte(Device_Address + 0x01);
	I2C_RecvACK();
	*Data = I2C_RecvByte();
	I2C_SendACK(1);
	I2C_Stop();

	return ack;
}





#endif

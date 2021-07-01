/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2008.7
* ****************************/
#include	"CH455I2C.H"		// 修改该文件以适应硬件环境/单片机型号等
#include <reg52.H>
#include <INTRINS.h>

void CH455_I2c_Start( void )  // 操作起始
{
	DISABLE_KEY_INTERRUPT;  //禁止键盘中断,防止开始时被CH455中断而进入中断服务程序中的START
	CH455_SDA_SET;   /*发送起始条件的数据信号*/
	CH455_SDA_D_OUT;   /* 设置SDA为输出方向 */
	CH455_SCL_SET;
	CH455_SCL_D_OUT;   /* 设置SCL为输出方向 */
	DELAY;
	CH455_SDA_CLR;   /*发送起始信号*/
	DELAY;      
	CH455_SCL_CLR;   /*钳住I2C总线，准备发送或接收数据 */
}

void CH455_I2c_Stop( void )  // 操作结束
{
	CH455_SDA_CLR;
	CH455_SDA_D_OUT;   /* 设置SDA为输出方向 */
	DELAY;
	CH455_SCL_SET;
	DELAY;
	CH455_SDA_SET;  /*发送I2C总线结束信号*/
	DELAY;
	CH455_SDA_D_IN;   /* 设置SDA为输入方向 */
	ENABLE_KEY_INTERRUPT;
}

void CH455_I2c_WrByte( UINT8 dat )	//写一个字节数据
{
	UINT8 i;
	CH455_SDA_D_OUT;   /* 设置SDA为输出方向 */
	for( i = 0; i != 8; i++ )  // 输出8位数据
	{
		if( dat & 0x80 ) 
		{
		    CH455_SDA_SET;
		}
		else 
		{
		    CH455_SDA_CLR;
		}
		DELAY;
		CH455_SCL_SET;
		dat <<= 1;
		DELAY;  // 可选延时
		CH455_SCL_CLR;
	}
	CH455_SDA_D_IN;   /* 设置SDA为输入方向 */
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_SET;  // 接收应答
	DELAY;
	CH455_SCL_CLR;
}

UINT8  CH455_I2c_RdByte( void )		//读一个字节数据
{
	UINT8 dat,i;
	CH455_SDA_SET;
	CH455_SDA_D_IN;   /* 设置SDA为输入方向 */
	dat = 0;
	for( i = 0; i != 8; i++ )  // 输入8位数据
	{
		DELAY;  // 可选延时
		CH455_SCL_SET;
		DELAY;  // 可选延时
		dat <<= 1;
		if( CH455_SDA_IN ) dat++;  // 输入1位
		CH455_SCL_CLR;
	}
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_SET;  // 发出无效应答
	DELAY;
	CH455_SCL_CLR;
	return dat;
}

void CH455_Write( UINT16 cmd )	//写命令
{
	CH455_I2c_Start();               //启动总线
   	CH455_I2c_WrByte(((UINT8)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
   	CH455_I2c_WrByte((UINT8)cmd);               //发送数据
  	CH455_I2c_Stop();                 //结束总线 
}

UINT8 CH455_Read( void )		//读取按键
{
	UINT8 keycode;
   	CH455_I2c_Start();                //启动总线
   	CH455_I2c_WrByte((UINT8)(CH455_GET_KEY>>7)&CH455_I2C_MASK|0x01|CH455_I2C_ADDR);
   	keycode=CH455_I2c_RdByte();      //读取数据
	CH455_I2c_Stop();                //结束总线
	return keycode;
}


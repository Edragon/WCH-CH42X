/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2008.7
******************************/
#include	"PIN.H"			// 修改该文件以适应硬件环境/单片机型号等
#include	"CH454CMD.H"	// CH454常量定义
#include <INTRINS.h>

#define DELAY  { _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_(); }

void CH454_I2c_Start( void )  // 操作起始
{
	DISABLE_KEY_INTERRUPT;  //禁止键盘中断,防止开始时被CH454中断而进入中断服务程序中的START
	CH454_SDA_SET;   /*发送起始条件的数据信号*/
    CH454_SDA_D_OUT;   /* 设置SDA为输出方向 */
	DELAY;
	CH454_SCL_SET;
    CH454_SCL_D_OUT;   /* 设置SCL为输出方向 */
	DELAY;

	CH454_SDA_CLR;   /*发送起始信号*/
	DELAY;    

	CH454_SCL_CLR;   /*钳住I2C总线，准备发送或接收数据 */
	DELAY;
}

void CH454_I2c_Stop( void )  // 操作结束
{
	CH454_SDA_CLR;
    CH454_SDA_D_OUT;   /* 设置SDA为输出方向 */
	DELAY;

	CH454_SCL_SET;
	DELAY;

	CH454_SDA_SET;  /*发送I2C总线结束信号*/
	DELAY;

    CH454_SDA_D_IN;   /* 设置SDA为输入方向 */
    DELAY;

  	ENABLE_KEY_INTERRUPT;
}

void CH454_I2c_WrByte( UINT8 dat )	//写一个字节数据
{
	UINT8 i;
	CH454_SDA_D_OUT;   /* 设置SDA为输出方向 */
	DELAY;

	for( i = 0; i != 8; i++ )  // 输出8位数据
	{
		if( dat & 0x80 ) 
		{
		    CH454_SDA_SET;
		}
		else 
		{
		    CH454_SDA_CLR;
		}
		DELAY;

		CH454_SCL_SET;
		dat <<= 1;
		DELAY;

		CH454_SCL_CLR;
		DELAY;
	}
	CH454_SDA_D_IN;   /* 设置SDA为输入方向 */
    CH454_SDA_SET;
	DELAY;

	CH454_SCL_SET;  // 接收应答
	DELAY;

	CH454_SCL_CLR;
	DELAY;
}

UINT8  CH454_I2c_RdByte( void )		//读一个字节数据
{
	UINT8 dat,i;
    CH454_SDA_SET;
	CH454_SDA_D_IN;   /* 设置SDA为输入方向 */
    DELAY;

	dat = 0;
	for( i = 0; i != 8; i++ )  // 输入8位数据
	{
		CH454_SCL_SET;
		DELAY;

		dat <<= 1;
		if( CH454_SDA_IN ) dat++;  // 输入1位

		CH454_SCL_CLR;
		DELAY;
	}

	CH454_SDA_SET;
    
	DELAY;

	CH454_SCL_SET;  // 发出无效应答

	DELAY;

	CH454_SCL_CLR;
	DELAY;

	return dat;
}

void ch454_write( UINT16 cmd )	//写命令
{
	CH454_I2c_Start();               /*启动总线*/

   	CH454_I2c_WrByte((UINT8)(cmd>>7)&CH454_I2C_MASK|CH454_I2C_ADDR); 

   	CH454_I2c_WrByte((UINT8)cmd);     /*发送数据*/
  	CH454_I2c_Stop();                 /*结束总线*/ 
}





UINT8 CH454_Read( UINT16 read_cmd )		//读取按键
{
	UINT8 keycode;
   	CH454_I2c_Start();                /*启动总线*/

	CH454_I2c_WrByte((UINT8)(read_cmd>>7)&CH454_I2C_MASK|CH454_I2C_ADDR|0x01);
	
   	keycode = CH454_I2c_RdByte();      /*读取数据*/
	CH454_I2c_Stop();                /*结束总线*/ 

	return keycode;
}

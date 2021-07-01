/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2008.7
* ****************************/
#include <reg52.h>
#include "CH455I2C.H"	// 修改该文件以适应硬件环境/单片机型号等

UINT8 ch455_key;//CH455的按键键值

const UINT8 BCD_decode_tab[0x10] = { 0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 
									 0X77, 0X7C, 0X58, 0X5E, 0X79, 0X71 };//BCD

void mDelaymS( UINT8 ms )// 延时毫秒
{
	UINT16 i;
	while ( ms -- ) 
	{
		for( i = 0; i != 1000; i++ );
	}
}

void init_CH455( void ) //初始化CH455
{
#ifdef USE_CH455_KEY
	IE1=0;
	EX1=1;
	EA=1;
#endif
}

void int1( void ) interrupt 2 using 1 // INT0中断服务程序
{
	ch455_key = CH455_Read();
}

void main( void )//主程序
{
	UINT8 i;
    UINT8 showH[2],showL[2];

	mDelaymS( 40 );
	init_CH455();//初始化

	CH455_Write( CH455_SYSON );// 开启显示和键盘，8段显示方式

	//发显示数据
	CH455_Write( CH455_DIG0 | BCD_decode_tab[0] );	
	CH455_Write( CH455_DIG1 | BCD_decode_tab[1] ); 
	CH455_Write( CH455_DIG2 | BCD_decode_tab[2] ); 
	CH455_Write( CH455_DIG3 | BCD_decode_tab[3] ); 

    //亮度变化
    for( i = 1; i < 8; i++ )
	{
	     mDelaymS( 50 );
 	     CH455_Write( CH455_SYSON | i<<4 );	// 1-7级亮度显示
    }
	mDelaymS( 50 );
	CH455_Write( CH455_SYSON_8 );	// 8级亮度显示

	//4位数码管全部显示0
	showL[0] = BCD_decode_tab[0];
	showH[0] = BCD_decode_tab[0];
	showL[1] = BCD_decode_tab[0];
	showH[1] = BCD_decode_tab[0];
	CH455_Write( CH455_DIG3 | showL[0] );	
	CH455_Write( CH455_DIG2 | showH[0] ); 
	CH455_Write( CH455_DIG1 | showL[1] ); 
	CH455_Write( CH455_DIG0 | showH[1] ); 
    
    
	while(1)
	{
	#ifdef USE_CH455_KEY
		 ch455_key = 0xff;
		 while( ch455_key == 0xff );

		 i = ch455_key & 0x3f;//按键值

		 //数码管显示之间的移位操作
		 showH[1] = showH[0];
		 showL[1] = showL[0];
		 showH[0] = BCD_decode_tab[i/10];
		 showL[0] = BCD_decode_tab[i%10];
         CH455_Write( CH455_DIG3 | showL[0] );	
	     CH455_Write( CH455_DIG2 | showH[0] ); 
	     CH455_Write( CH455_DIG1 | showL[1] ); 
	     CH455_Write( CH455_DIG0 | showH[1] );

		 //判断按键有没有释放
		 while(1)
		 {
		      i = CH455_Read( );//读按键数值
			  if( i & 0x40 )//按键按下没有释放
			  {
			      //闪烁
			      CH455_Write( CH455_DIG3 );	
	              CH455_Write( CH455_DIG2 );
                  mDelaymS(50);
				  CH455_Write( CH455_DIG3 | showL[0] );	
	              CH455_Write( CH455_DIG2 | showH[0] );
			      mDelaymS(50);
			  }
			  else//按键已经释放
			  {
			      break;
			  }
		  }

    #endif
	}
												
}

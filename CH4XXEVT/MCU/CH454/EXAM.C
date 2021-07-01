/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2008.7
******************************/
#pragma noaregs
#include "PIN.H"
#include "CH454CMD.H"	// 定义常用命令码及外部子程序
#include <reg52.h>

UINT8 mBCD[16] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 
	  		   	   0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71 };//BCD

UINT8 ch454_key = 0xff;//按键数值

void delayms( UINT8 i )//延时mS
{	
    UINT16	j;
	do
	{	
	    for( j = 0; j != 1000; j++ )
		{
		   ;
		}
	}
	while( --i );
}


void init( void )//初始化
{	
	IE1 = 0;
	EX1 = 1;
	EA = 1;
}


// INT1中断服务程序
void int1( void ) interrupt 2 using 1
{
	ch454_key = CH454_Read( 0x0700 );  
}


void main( void )//主程序
{
   UINT8 a,b;
   UINT8 vL[4],vH[4];

   delayms(40);

#ifdef USE_CH454_KEY
   init();//初始化
   ch454_write(0x443);//开显示键盘
#else
   ch454_write(0x441);//开显示
#endif

   ch454_write(CH454_NO_BCD);//直接译码方式   
  
   //发送显示命令
   ch454_write( CH454_DIG0_LO | mBCD[0] );
   ch454_write( CH454_DIG1_LO | mBCD[1] ); 
   ch454_write( CH454_DIG2_LO | mBCD[2] ); 
   ch454_write( CH454_DIG3_LO | mBCD[3] );
   ch454_write( CH454_DIG4_LO | mBCD[4] );
   ch454_write( CH454_DIG5_LO | mBCD[5] );
   ch454_write( CH454_DIG6_LO | mBCD[6] );
   ch454_write( CH454_DIG7_LO | mBCD[7] );
   ch454_write( CH454_DIG0_HI | mBCD[8] );
   ch454_write( CH454_DIG1_HI | mBCD[9] ); 
   ch454_write( CH454_DIG2_HI | mBCD[10] );
   ch454_write( CH454_DIG3_HI | mBCD[11] );
   ch454_write( CH454_DIG4_HI | mBCD[12] );
   ch454_write( CH454_DIG5_HI | mBCD[13] );
   ch454_write( CH454_DIG6_HI | mBCD[14] );
   ch454_write( CH454_DIG7_HI | mBCD[15] );

   delayms(240);
         
#ifdef USE_CH454_KEY
   //显示0
   ch454_write( CH454_DIG0_LO | mBCD[0]);
   ch454_write( CH454_DIG1_LO | mBCD[0]); 
   ch454_write( CH454_DIG2_LO | mBCD[0]); 
   ch454_write( CH454_DIG3_LO | mBCD[0]);
   ch454_write( CH454_DIG4_LO | mBCD[0]);
   ch454_write( CH454_DIG5_LO | mBCD[0]);
   ch454_write( CH454_DIG6_LO | mBCD[0]);
   ch454_write( CH454_DIG7_LO | mBCD[0]);
   ch454_write( CH454_DIG0_HI );
   ch454_write( CH454_DIG1_HI ); 
   ch454_write( CH454_DIG2_HI );
   ch454_write( CH454_DIG3_HI );
   ch454_write( CH454_DIG4_HI );
   ch454_write( CH454_DIG5_HI );
   ch454_write( CH454_DIG6_HI );
   ch454_write( CH454_DIG7_HI );
   vL[0]=0; 
   vL[1]=0; 
   vL[2]=0; 
   vL[3]=0;
   vH[0]=0; 
   vH[1]=0; 
   vH[2]=0; 
   vH[3]=0;
 
   while(1)
   {
       ch454_key = 0xff;
       while( ch454_key == 0xff );	                //等待中断

       a = ch454_key & 0x3F;//按键数值	            
       b = a;//转换十进制数据
       a /= 10;//十位
	   b %= 10;//个位

	   //移位
	   vL[3] = vL[2]; vL[2] = vL[1]; vL[1] = vL[0]; vL[0] = a;
	   vH[3] = vH[2]; vH[2] = vH[1]; vH[1] = vH[0]; vH[0] = b;
       ch454_write( CH454_DIG1_LO | mBCD[vL[0]] );
       ch454_write( CH454_DIG0_LO | mBCD[vH[0]] );
       ch454_write( CH454_DIG3_LO | mBCD[vL[1]] );
       ch454_write( CH454_DIG2_LO | mBCD[vH[1]] );
	   ch454_write( CH454_DIG5_LO | mBCD[vL[2]] );
       ch454_write( CH454_DIG4_LO | mBCD[vH[2]] );
	   ch454_write( CH454_DIG7_LO | mBCD[vL[3]] );
       ch454_write( CH454_DIG6_LO | mBCD[vH[3]] );

	   while(1)
	   {
			
		    a = CH454_Read( 0x0700 );
			if( a & 0x40 )//未释放
			{
				ch454_write( CH454_TWINKLE | 0x03 );//全闪烁
			}
			else//释放
			{
				ch454_write( CH454_TWINKLE );//全不闪烁
			    break;
			}
		}
   }
#else
   while(1);
#endif
}


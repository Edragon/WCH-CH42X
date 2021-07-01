// *******************************************************************
//Website:  http://wch.cn
//Email:    tech@wch.cn
//Author:   W.ch 2007.6
// *******************************************************************
// 片选译码方式驱动16个数码管/128只LED发光管，MCS51单片机例子，使用定时器定时切换显示

#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#pragma NOAREGS    // 如果MCS51在中断服务程序中调用CH423子程序，那么建议加入此编译选项

#include "CH423IF.H"    // 修改该文件以适应硬件环境/单片机型号等
//#include "CH423IF.C"

const unsigned char BCD_decode_tab[ 29 ] = { 0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X58, 0X5E, 0X79, 0X71, 0x00, 0x46, 0x40, 0x41, 0x39, 0x0F, 0x08, 0x76, 0x38, 0x73, 0x80, 0xFF, 0x00 };
#define SPACE             0x10       // 空格     

unsigned char CH423_buf[ 16 ];    // 定义16个数码管的数据映象缓存区

void mDelaymS( unsigned char ms )    // 延时毫秒
{
    unsigned short i;
    while ( ms -- )
    {
    for( i = 0; i != 1000; i++ );
    }
}

void mDelayS( unsigned char s )    // 延时秒
{
    while ( s -- ) 
    {    
        mDelaymS( 250 );
        mDelaymS( 250 );
        mDelaymS( 250 );
        mDelaymS( 250 );
    }
}

void CH423_buf_index( unsigned char index, unsigned char dat )  // 向CH423指定的数码管输出数据,自动建立数据映象
{
    if( index >= sizeof( CH423_buf ) ) index = 0;
    if( dat >= sizeof( BCD_decode_tab ) ) dat = SPACE;    // 数据表格大小，写空格
    CH423_buf[index ] = BCD_decode_tab[ dat ];    // 备份数据到相应的映象单元
}

void CH423_set_bit( unsigned char bit_addr )    // 段位点亮
{
    unsigned char byte_addr;
    byte_addr = ( bit_addr>>3 ) & 0x0F;
    CH423_buf[ byte_addr ] |= 1<<( bit_addr&0x07 );
}

void CH423_clr_bit( unsigned char bit_addr )    // 段位熄灭
{
    unsigned char byte_addr;
    byte_addr = ( bit_addr>>3 ) & 0x0F;
    CH423_buf[ byte_addr ] &= ~ (1<<(bit_addr&0x07 ) );
}

void LEFTMOV()    // 左移                   
{
    unsigned short i;
    for ( i = 0x000F; i >= 0x0001; i-- ) CH423_buf[ i ] = CH423_buf[ i - 1 ];
    CH423_buf[ 0x00 ] = 0x00;
}

void RIGHTMOV()    // 右移
{
    unsigned short i;
    for ( i = 0x0000; i < 0x000F; i++ ) CH423_buf[ i ] = CH423_buf[ i + 1 ];
    CH423_buf[ 0x0F ] = 0x00;
}

void TWINKLE( unsigned char dig_number )    // 闪烁
{
    unsigned char old;
    dig_number &= 0x0F;
    old = CH423_buf[ dig_number ];
    CH423_buf[ dig_number ] = 0x00;
    mDelaymS( 250 );
    CH423_buf[ dig_number ] = old;
    mDelaymS( 250 );
}

void mInitSTDIO( )
{
    TMOD = 0x10;    // T/C1工作在定时器方式1 
    TL1 = 0x30;
    TH1 = 0xf8;    // 24MHz晶振，预置计数初值, 1ms定时 
    TR1 = 1;       // 启动T/C1开始定时 
    EA = 1;        // CPU开中断  
    ET1 = 1;       // T/C1开中断 
}

void int1() interrupt 3    //定时器1中断,定时切换数码管显示驱动
{
    static unsigned char i = 0;
    TL1 = 0x30;
    TH1 = 0xf8;  /* 24MHz晶振，预置计数初值, 1ms定时 */
    CH423_WriteByte( CH423_SET_IO_CMD + ( ( unsigned short ) i << 9 ) | CH423_buf[ i ] );    // 发出 
    if( ++i >= 16 ) i = 0;
}

main()
{
    unsigned char i;
    mInitSTDIO( );
    mDelaymS(30);
    for( i = 0; i < 16; i++ ) CH423_buf_index( i, 0x10 );
    mDelaymS(10);
    CH423_WriteByte( CH423_SYS_CMD | BIT_DEC_H | BIT_DEC_L | BIT_IO_OE );    // 设置系统参数命令
    mDelaymS( 250 );
    for( i = 0; i < 16; i++ ) CH423_buf_index( i, i );    // 显示0，1，2，3，4，5，6，7，8，9，A, B, C, D, E, F
    mDelaymS( 250 );
    mDelaymS( 250 );
    for( i = 0; i < 16; i++ ) CH423_buf_index( i,0x10 );    // 显示空格
    mDelaymS( 250 );
    for( i = 0; i != 16*8; i++ )    // 依次置段位
    {
        CH423_set_bit( i );
        mDelaymS( 50 );
    }
    mDelaymS( 250 );
    mDelaymS( 250 );
    for( i = 0; i < 16; i++ ) CH423_buf_index( i,15-i );    // 显示F，E，D，C，B，A，9，8，7，6，5, 4, 3, 2, 1, 0
    mDelaymS( 250 );
    mDelaymS( 250 );
    while( 1 ) {
	    mDelaymS( 10 );
    }
}

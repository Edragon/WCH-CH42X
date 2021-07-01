// *******************************************************************
//Website:  http://wch.cn
//Email:    tech@wch.cn
//Author:   W.ch 2007.6
// *******************************************************************

/*****************CH423全自动显示扫描例子程序 *************************/

#include <reg52.h>
#include <intrins.h>
#include <stdio.h>

#include "CH423IF.H"    // 修改该文件以适应硬件环境/单片机型号等
//#include "CH423IF.C"

#define CH423_SYSON1    0x0417    //开启自动扫描显示

unsigned char CH423_buf[16];    //定义16个数码管的数据映象缓存区
const unsigned char BCD_decode_tab[ 0x10 ] = { 0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X58, 0X5E, 0X79, 0X71 };

void mDelaymS( unsigned char ms )    // 延时毫秒
{
    unsigned short i;
    while ( ms -- ) 
    {
        for ( i = 0; i != 1000; i++ );
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

void CH423_buf_write( unsigned short cmd )    // 向CH423输出数据或者操作命令，自动建立数据映象
{
    if ( cmd & 0x1000 ) 
    {    // 加载数据的命令,需要备份数据到映象缓冲区
        CH423_buf[ (unsigned char)( cmd >> 8 ) & 0x0F ] = (unsigned char)( cmd & 0xFF );    // 备份数据到相应的映象单元
    }
    CH423_Write( cmd );    // 发出
}

void CH423_buf_index( unsigned char index, unsigned char dat )    // 向CH423指定的数码管输出数据,自动建立数据映象
{    // index 为数码管序号,有效值为0到15,分别对应DIG0到DIG15
    unsigned short cmd;
    CH423_buf[ index ] = dat;    // 备份数据到相应的映象单元
    cmd = ( CH423_DIG0 + ( ( unsigned short)index << 8 ) ) | dat;    // 生成操作命令
    CH423_Write( cmd );    // 发出
}

void CH423_set_bit( unsigned char bit_addr )    // 段位点亮
{
    unsigned char byte_addr;
    byte_addr = ( bit_addr>>3 ) & 0x0F;
    CH423_buf_index( byte_addr, CH423_buf[byte_addr] | ( 1<<( bit_addr&0x07 ) ) );
}

void CH423_clr_bit( unsigned char bit_addr )    // 段位熄灭
{
    unsigned char byte_addr;
    byte_addr = ( bit_addr>>3 ) & 0x0F;
    CH423_buf_index( byte_addr, CH423_buf[byte_addr] & ~(1<<(bit_addr&0x07 ) ) );
}

void LEFTMOV()    // 左移                   
{
    unsigned short i;
    for ( i = 0x000F; i >= 0x0001; i-- )
    {
        CH423_buf_index( i, CH423_buf[i-1] ); 
    }
    CH423_Write( 0x1000 );
}

void LEFTCYC()    // 左循
{
    unsigned short i, j;
    j = CH423_buf[15];
    for ( i = 0x000F; i >= 0x0001; i-- )
    {
        CH423_buf_index( i,CH423_buf[i-1] ); 
    } 
        CH423_Write( 0x1000 | j );
}

void RIGHTMOV()    // 右移
{
    unsigned short i;
    for ( i = 0x0000; i < 0x000F; i++ )
    {
        CH423_buf_index( i,CH423_buf[i+1] ); 
    }
    CH423_Write( 0x1F00 );
}

void RIGHTCYC()    // 右循
{
    unsigned char i, j;
    j=CH423_buf[0];
    for ( i = 0x0000; i < 0x000F; i++ )
    {
        CH423_buf_index( i, CH423_buf[i+1] ); 
    }
    CH423_Write( 0x1F00 | j );
}

void TWINKLE( unsigned char dig_number )    // 闪烁
{
    unsigned char old;
    old = CH423_buf[dig_number & 0x0F];
    CH423_buf_index( dig_number, 0x00 );
    mDelaymS( 250 );
    CH423_buf_index( dig_number, old );
    mDelaymS( 250 );
}

main()
{
    unsigned char i, j;
    mDelaymS( 50 );
    for ( i = 0; i < 16; i ++ ) CH423_buf_index( i, 0 );    // 因为CH423复位时不清空显示内容，所以刚开电后必须人为清空，再开显示
    CH423_buf_write( CH423_SYSON1 );    // 开启显示
    // 如果需要定期刷新显示内容，那么只要执行17个命令，包括16个数据加载命令，以及1个开启显示命令
    CH423_buf_write( CH423_DIG15 | BCD_decode_tab[15] );    // 显示BCD码1
    CH423_buf_write( CH423_DIG14 | BCD_decode_tab[14] );
    CH423_buf_write( CH423_DIG13 | BCD_decode_tab[13] );
    CH423_buf_write( CH423_DIG12 | BCD_decode_tab[12] );
    CH423_buf_write( CH423_DIG11 | BCD_decode_tab[11] );    // 显示BCD码1
    CH423_buf_write( CH423_DIG10 | BCD_decode_tab[10] );
    CH423_buf_write( CH423_DIG9  | BCD_decode_tab[9] );
    CH423_buf_write( CH423_DIG8  | BCD_decode_tab[8] );
    CH423_buf_write( CH423_DIG7  | BCD_decode_tab[7] );
    CH423_buf_write( CH423_DIG6  | BCD_decode_tab[6] );
    CH423_buf_write( CH423_DIG5  | BCD_decode_tab[5] );    // 显示BCD码1
    CH423_buf_write( CH423_DIG4  | BCD_decode_tab[4] );
    CH423_buf_write( CH423_DIG3  | BCD_decode_tab[3] );
    CH423_buf_write( CH423_DIG2  | BCD_decode_tab[2] );
    CH423_buf_write( CH423_DIG1  | BCD_decode_tab[1] );
    CH423_buf_write( CH423_DIG0  | BCD_decode_tab[0] );
    while ( 1 ) 
    {    // 演示
        for( i = 0; i < 16; i++ ) CH423_buf_index( i, 0xFF );    // 全亮
        mDelayS( 1 );
        for( i = 0; i < 16; i++ ) CH423_buf_index( i, 0x00 );    // 全灭
        mDelaymS( 500 );
        for ( j = 0; j != 8; j ++ ) 
        {    // 依次扫描每段，演示
            for ( i = 0; i != 16; i++ ) CH423_buf_index( i, 1<<j );
            mDelaymS( 250 );
        }
        for( i = 0; i < 16; i++ ) CH423_buf_index( i, 0xFF );    // 全亮
        mDelaymS( 500 );
        for( i = 0; i < 16; i++ ) CH423_buf_index( i, BCD_decode_tab[i] );    // 依次显示BCD译码0、1、2、3、4、5、6、7、8、9、A、b、c、d、E、F
        mDelaymS( 500 );
        for( i = 0; i != 8; i++ )    // 左循环
        {
            LEFTCYC(); mDelaymS( 250 );
        }
        for( i = 0;i != 8; i++ )    // 右循环
        {
            RIGHTCYC(); mDelaymS( 250 );
        }
        for(i = 0; i != 8; i++)    // 左移
        {
            LEFTMOV(); mDelaymS( 250 );
        }
        for( i = 0; i != 8; i++)    // 右移
        {
             RIGHTMOV(); mDelaymS(250);
        }
        for( i = 0; i < 16; i++ ) CH423_buf_index( i, BCD_decode_tab[i] );    // 依次显示BCD译码0、1、2、3、4、5、6、7、8、9、a、b、c、d、E、F
        mDelaymS( 250 );
        for( i = 0; i != 16; i++ )    // 闪烁
        {
            TWINKLE( i ); mDelaymS( 100 );
        }
        for( i = 0; i < 16; i++ ) CH423_buf_index( i, 0x00 );    // 全灭
        mDelaymS( 250 );
        for( i = 0; i != 16*8; i++ )    // 依次置段位 
        {
            CH423_set_bit( i );
            mDelaymS( 100 );
        }
        mDelaymS( 500 );
        for( i = 0; i != 16*8; i++ )    // 依次清段位
        {
            CH423_clr_bit( i );
            mDelaymS( 100 );
        }
        mDelaymS( 500 );
    }
}

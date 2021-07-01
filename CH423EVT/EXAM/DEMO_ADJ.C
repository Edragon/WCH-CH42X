// *******************************************************************
//Email:    tech@wch.cn
//Author:   W.ch 2007.6
// *******************************************************************

/*********CH423做数字电位器功能使用的 MCS-51单片机C语言示例程序，参考数据手册中的原理图 ********/

#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#pragma NOAREGS    // 如果MCS51在中断服务程序中调用CH423子程序，那么建议加入此编译选项

#include "CH423IF.H"    // 修改该文件以适应硬件环境/单片机型号等
//#include "CH423IF.C"

unsigned char SaveCurrentIO = 0x00;

unsigned char code VOL_TAB[] =    // 电压大小
{
    0x00, 0xf8, 0x9c, 0x12, 0x7a, 0xb6, 0xfe, 0x01,
    0xf1, 0xd9, 0x45, 0xb5, 0x6d, 0xfd, 0x03, 0x13,
    0xf3, 0xab, 0x5b, 0xfb, 0x07, 0x67, 0x97, 0x37,
    0xf7, 0x0f, 0xcf, 0xaf, 0xef, 0x1f, 0x9f, 0x5f,
    0xdf, 0x3f, 0xbf, 0x7f, 0xff,
};

void Select_Chanel( unsigned char Index )    // 选择输入通道，IO1，IO0组成四种通道
{
	if ( Index > 0x03 ) return;
	SaveCurrentIO &= 0xFC;
	SaveCurrentIO |= Index;
    CH423_WriteByte( CH423_SET_IO_CMD | SaveCurrentIO );
}

void Set_Enable( unsigned char Enable )    // 使能模拟开关
{
	SaveCurrentIO &= 0xFB;
	if ( ! Enable ) SaveCurrentIO |= BIT_IO2_DAT;
    CH423_WriteByte( CH423_SET_IO_CMD | SaveCurrentIO );
}

void Select_VOL( unsigned  char VOL )    // 选择电压值
{
	unsigned char ctrl;
    if( VOL >= sizeof( VOL_TAB ) ) VOL = sizeof( VOL_TAB ) - 1;    // 电压最大级别不能超过表的大小
    ctrl = VOL_TAB[VOL];
    CH423_WriteByte( CH423_OC_L_CMD | ctrl );    
    CH423_WriteByte( CH423_OC_H_CMD | ctrl );   
}

void mDelaymS( unsigned char ms )     // 延时毫秒
{
    unsigned short i;
    while ( ms -- ) 
    {
        for( i = 0; i != 1000; i++ );
    }
}

void mDelayS( unsigned char s )      // 延时秒
{
    while ( s -- ) 
    {   mDelaymS( 250 );
        mDelaymS( 250 );
        mDelaymS( 250 );
        mDelaymS( 250 );
    }
}

main()
{
    mDelaymS( 50 );
    Select_Chanel( 2 );    // 选择输入通道2
    Select_VOL( 5 );    // 选择数字电位器的输出,选择5级音量
    Set_Enable( 1 );    // 打开模拟开关
    CH423_WriteByte( CH423_SYS_CMD | BIT_IO_OE );    // 设置系统参数命令
    mDelayS( 2 );
    Select_Chanel( 1 );    // 选择输入通道1
    Select_VOL( 20 );    // 选择数字电位器的输出,选择20级音量
    while( 1 );
}

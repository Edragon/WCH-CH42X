/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2008.7
******************************/
#include <reg52.h>
#include <intrins.h>
#pragma NOAREGS		// 如果MCS51使用键盘中断功能，那么建议加入此编译选项

/* 2线接口的连接,与实际电路有关 */
sbit	CH454_SCL=P1^7;
sbit	CH454_SDA=P1^6;   
sbit	CH454_INT=P3^3;   


/* 2线接口的位操作,与单片机有关 */
#define		CH454_SCL_SET		{CH454_SCL=1;}
#define		CH454_SCL_CLR		{CH454_SCL=0;}
#define		CH454_SCL_D_OUT		{}				// 设置SCL为输出方向,对于双向I/O需切换为输出
#define		CH454_SDA_SET		{CH454_SDA=1;}
#define		CH454_SDA_CLR		{CH454_SDA=0;}
#define		CH454_SDA_IN		(CH454_SDA)
#define		CH454_SDA_D_OUT		{}				// 设置SDA为输出方向,对于双向I/O需切换为输出
#define		CH454_SDA_D_IN		{CH454_SDA=1;}	// 设置SDA为输入方向,对于双向I/O需切换为输入


/* 与单片机有关,与中断连接方式有关 */
#define		USE_CH454_KEY		1			// 使用了CH454的按键中断
#ifdef USE_CH454_KEY
#define		DISABLE_KEY_INTERRUPT	{EX1=0;}
#define		ENABLE_KEY_INTERRUPT	{EX1=1;}
#define		CLEAR_KEY_INTER_FLAG	{IE1=0;}
#else
#define		DISABLE_KEY_INTERRUPT	{}
#define		ENABLE_KEY_INTERRUPT	{}
#define		CLEAR_KEY_INTER_FLAG	{}
#endif

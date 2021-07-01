/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2008.7
*****************************/
typedef unsigned char  UINT8;
typedef unsigned short UINT16;


/* CH454的常用命令码 */
#define CH454_RESET     0x0201					// 复位
#define CH454_CLR_BIT	0x0180					// 段位清0,需另加6位数据
#define CH454_SET_BIT	0x01C0					// 段位置1,需另加6位数据
#define CH454_NO_BCD    0x0500					// 设置直接译码方式   
#define CH454_TWINKLE   0x0600		            // 设置闪烁控制,需另加8位数据
#define CH454_GET_KEY	0x0700					// 获取按键,返回按键代码
#define CH454_DIG0_LO   0x0800					// 数码管位0LO显示,需另加8位数据
#define CH454_DIG1_LO   0x0900		            // 数码管位1LO显示,需另加8位数据
#define CH454_DIG2_LO   0x0a00		            // 数码管位2LO显示,需另加8位数据
#define CH454_DIG3_LO   0x0b00		            // 数码管位3LO显示,需另加8位数据
#define CH454_DIG4_LO   0x0c00		            // 数码管位4LO显示,需另加8位数据
#define CH454_DIG5_LO   0x0d00					// 数码管位5LO显示,需另加8位数据
#define CH454_DIG6_LO   0x0e00					// 数码管位6LO显示,需另加8位数据
#define CH454_DIG7_LO   0x0f00		            // 数码管位7LO显示,需另加8位数据
#define CH454_DIG0_HI   0x1000					// 数码管位0HI显示,需另加8位数据
#define CH454_DIG1_HI   0x1100		            // 数码管位1HI显示,需另加8位数据
#define CH454_DIG2_HI   0x1200		            // 数码管位2HI显示,需另加8位数据
#define CH454_DIG3_HI   0x1300		            // 数码管位3HI显示,需另加8位数据
#define CH454_DIG4_HI   0x1400		            // 数码管位4HI显示,需另加8位数据
#define CH454_DIG5_HI   0x1500					// 数码管位5HI显示,需另加8位数据
#define CH454_DIG6_HI   0x1600					// 数码管位6HI显示,需另加8位数据
#define CH454_DIG7_HI   0x1700		            // 数码管位7HI显示,需另加8位数据

// 2线接口的CH454定义
#define		CH454_I2C_ADDR		0x40			// CH454的地址
#define		CH454_I2C_MASK		0x3E			// CH454的2线接口高字节命令掩码

// 对外子程序
extern	UINT8 CH454_Read( UINT16 read_cmd );		//读取按键
extern  void ch454_write( UINT16 cmd );	//写命令

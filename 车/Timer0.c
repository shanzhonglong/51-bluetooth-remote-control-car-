#include <at89c51RC2.h>


void Timer0_Init(void)		//100us
{
	AUXR &= 0xF7;			
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL0 = 0xA4;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;        //定时器0开始计时
			
}

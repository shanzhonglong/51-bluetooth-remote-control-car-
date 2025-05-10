#ifndef __TIMER0_H_
#define __TIMER0_H_

void Timer0_Init(void);
#endif




/*
void Timer0_Routine() interrupt 1
{  
	 static unsigned int T0Count;
	 TL1 = 0xA4;				//设置定时初始值
	 TH1 = 0xFF;				//设置定时初始值
	 T1Count++;
	 if(T0Count>=1000)
	 { 
		 
		 T0Count=0;
	  
	 }
 }	 
	*/
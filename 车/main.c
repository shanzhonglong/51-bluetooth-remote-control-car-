#include <at89c51RC2.h>
#include "Timer0.h"

// 电机控制引脚定义
sbit STBY = P2^4;  // 待机控制引脚
sbit PWMA = P2^5;  // 电机 A 的 PWM 控制引脚
sbit AIN1 = P2^6;  // 电机 A 的方向控制引脚 1
sbit AIN2 = P2^7;  // 电机 A 的方向控制引脚 2
sbit BIN1 = P1^0;  // 电机 B 的方向控制引脚 1
sbit BIN2 = P1^1;  // 电机 B 的方向控制引脚 2
sbit PWMB = P1^2;  // 电机 B 的 PWM 控制引脚



unsigned char motor1_active = 0;  // 电机1是否处于活动状态
void Uart1_Init(void)	//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
}

char UART_ReceiveChar(void) {
    while (!RI);      // 等待接收数据完成
    RI = 0;           // 清除接收标志
    return SBUF;      // 返回接收到的数据
}

// 电机控制函数
void MotorControl(char command) {
    // 先关闭所有电机
    AIN1 = 0;
    AIN2 = 0;
    BIN1 = 0;
    BIN2 = 0;
    PWMA = 0;
    PWMB = 0;
    STBY = 1;  // 保持 STBY 为高电平，使能电机驱动

    switch (command) {
        case 'F':  // 前进
            AIN1 = 1;
            AIN2 = 0;
            PWMA = 1;  // 设置 PWM 为高电平
            break;

        case 'B':  // 后退
            AIN1 = 0;
            AIN2 = 1;
            PWMA = 1;  // 设置 PWM 为高电平
            break;

        case 'L':  // 左转
            BIN1 = 1;
            BIN2 = 0;
            PWMB = 1;  // 设置 PWM 为高电平
            break;

        case 'R':  // 右转
            BIN1 = 0;
            BIN2 = 1;
            PWMB = 1;  // 设置 PWM 为高电平
            break;

        case 'S':  // 停止
            AIN1 = 0;
            AIN2 = 0;
            BIN1 = 0;
            BIN2 = 0;
            PWMA = 0;
            PWMB = 0;
            break;

        default:
            // 无效命令，不做任何操作
            break;
    }
}

void main()
{
	char command;
  Uart1_Init();  // 初始化串口
	while(1)
	{
	   command = UART_ReceiveChar();  // 接收来自单片机A的指令
     MotorControl(command);  // 根据接收到的指令控制电机
	}
}	




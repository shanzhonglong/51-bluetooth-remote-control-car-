#include <at89c51RC2.h>
#include "UART.H"

// 按键引脚定义
sbit KEY1 = P2^0;  // 前进按键
sbit KEY2 = P2^1;  // 后退按键
sbit KEY3 = P3^2;  // 左转按键
sbit KEY4 = P3^3;  // 右转按键

// 用于标记按键是否按下
bit key1_pressed = 0;
bit key2_pressed = 0;
bit key3_pressed = 0;
bit key4_pressed = 0;

// 记录前进状态（是否正在前进）
bit moving_forward = 0;
// 记录后退状态（是否正在后退）
bit moving_backward = 0;

void main()
{
    Uart1_Init();  // 初始化串口
    IT0 = 1;        // 设置 INT0 为下降沿触发
    IT1 = 1;        // 设置 INT1 为下降沿触发
    EX0 = 1;        // 启用外部中断 0
    EX1 = 1;        // 启用外部中断 1
    EA = 1;         // 启用总中断

    while(1)
    {
        // 处理前进和后退按键
        if (KEY1 == 0 && !key1_pressed) {  // 前进按键按下
            UART_SendChar('F');
            key1_pressed = 1;  // 标记为按下
            moving_forward = 1;  // 记录电机正在前进
            moving_backward = 0; // 停止后退
        } else if (KEY1 == 1 && key1_pressed) {  // 前进按键松开
            key1_pressed = 0;  // 重置标记
            if (!key2_pressed) {  // 只有当没有后退命令时才停止
                UART_SendChar('S');
                moving_forward = 0;  // 电机停止
            }
        }

        if (KEY2 == 0 && !key2_pressed) {  // 后退按键按下
            UART_SendChar('B');
            key2_pressed = 1;  // 标记为按下
            moving_forward = 0;  // 停止前进
            moving_backward = 1; // 记录电机正在后退
        } else if (KEY2 == 1 && key2_pressed) {  // 后退按键松开
            key2_pressed = 0;  // 重置标记
            if (!key1_pressed) {  // 只有当没有前进命令时才停止
                UART_SendChar('S');
                moving_backward = 0; // 电机停止
            }
        }
    }
}

void INT0_ISR(void) interrupt 0 {
    if (KEY3 == 0 && !key3_pressed) {  // 左转按键按下
        UART_SendChar('L');  // 启动转向
        key3_pressed = 1;    // 标记为按下
    }
    while (KEY3 == 0);   // 等待按键松开
    key3_pressed = 0;     // 松开时重置标记
    // 如果前进键或后退键还在按下，则继续前进或后退
    if (moving_forward) {
        UART_SendChar('F');  // 继续前进
    } else if (moving_backward) {
        UART_SendChar('B');  // 继续后退
    } else {
        UART_SendChar('S');  // 否则停止电机
    }
}

void INT1_ISR(void) interrupt 2 {
    if (KEY4 == 0 && !key4_pressed) {  // 右转按键按下
        UART_SendChar('R');  // 启动转向
        key4_pressed = 1;    // 标记为按下
    }
    while (KEY4 == 0);   // 等待按键松开
    key4_pressed = 0;     // 松开时重置标记
    // 如果前进键或后退键还在按下，则继续前进或后退
    if (moving_forward) {
        UART_SendChar('F');  // 继续前进
    } else if (moving_backward) {
        UART_SendChar('B');  // 继续后退
    } else {
        UART_SendChar('S');  // 否则停止电机
    }
}

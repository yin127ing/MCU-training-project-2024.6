#include<reg51.h>//51单片机头文件
#include "uart.h"//串口通信
#include"function.c"//主要执行函数

//主函数
void main() {
    while (1) { // 无限循环，或者使用其他条件来控制循环
        if (!CHANGE1) {
            NO_1_main();
        }
        if (!CHANGE2) {
            NO_2_main();
        }
        else {
            DataPort = 0xff;
        }
    }
}


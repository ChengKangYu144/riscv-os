#include "../include/console.h"

#define UART0 0x10000000
#define LSR   0x10000005
#define THR   0x10000000
#define RHR   0x10000000
#define THRE  0x20
#define DR    0x01  // 数据寄存器非空标志

void uart_init(void) {
    // 简单裸机可空
}

void uart_putc(char c) {
    while (!(*(volatile unsigned char *)LSR & THRE));
    *(volatile unsigned char *)THR = c;
}

char uart_getc(void) {
    while (!(*(volatile unsigned char *)LSR & DR));
    return *(volatile unsigned char *)RHR;
}
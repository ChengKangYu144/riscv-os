#include "../include/console.h"

#define UART0 0x10000000
#define LSR   0x10000005
#define THR   0x10000000
#define THRE  0x20

void uart_init(void) {
    // 简单裸机可空
}

void uart_putc(char c) {
    while (!(*(volatile unsigned char *)LSR & THRE));
    *(volatile unsigned char *)THR = c;
}


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
    // 直接写 MMIO 寄存器，避免等待 LSR 导致无法输出
    *(volatile unsigned char *)UART0 = (unsigned char)c;
}

char uart_getc(void) {
    // 保持阻塞读（按需可改为非阻塞或带超时）
    while (!(*(volatile unsigned char *)RHR & DR));
    return *(volatile unsigned char *)RHR;
}
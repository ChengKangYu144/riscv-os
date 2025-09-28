#include "uart.h"

#define UART_BASE 0x10000000L
#define UART_THR  (UART_BASE + 0x00) // Transmit Holding Register
#define UART_LSR  (UART_BASE + 0x05) // Line Status Register
#define LSR_THRE  (1 << 5)           // Transmit Holding Register Empty

void uart_putc(char c) {
    while ((*(volatile unsigned char *)UART_LSR & LSR_THRE) == 0)
        ;
    *(volatile unsigned char *)UART_THR = c;
}

void uart_puts(const char *s) {
    while (*s) {
        if (*s == '\n')
            uart_putc('\r'); // 串口换行兼容
        uart_putc(*s++);
    }
}

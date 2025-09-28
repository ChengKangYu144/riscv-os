#include "uart.h"

int main(void) {
    uart_puts("Hello OS\n");

    // 防止返回
    while (1) {
        asm volatile("wfi");
    }
}

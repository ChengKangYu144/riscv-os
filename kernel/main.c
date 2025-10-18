#include "console.h"

int main(void) {
    uart_init();
    clear_screen();

    printf("Hello OS! Press Ctrl+J or Ctrl+L to clear screen.\n");
    printf("Decimal: %d\n", 123);
    printf("Negative: %d\n", -456);
    printf("Hex: 0x%x\n", 0xABC);
    printf("Char: %c\n", 'X');
    printf("String: %s\n", "Hello");
    printf("Percent: %%\n");
    printf("INT_MAX: %d\n", 2147483647);
    printf("INT_MIN: %d\n", -2147483648);
    printf("NULL string: %s\n", (char*)0);
    printf("Empty string: %s\n", "");

    while (1) {
        char c = uart_getc();  // 阻塞读取字符

        if (c == 0x0A || c == 0x0C) {       // Ctrl+J 的 ASCII
            clear_screen();
            printf("Screen cleared!\n");
        } else {
            console_putc(c);   // 回显其他字符
        }
    }
}

#include "console.h"

int main(void) {
    uart_init();
    clear_screen();

    printf("Hello OS!\n");
    printf("Decimal: %d\n", 123);
    printf("Negative: %d\n", -456);
    printf("Hex: 0x%x\n", 0xABC);
    printf("Char: %c\n", 'X');
    printf("String: %s\n", "Hello");
    printf("Percent: %%\n");

    while (1) asm volatile("wfi");  // 防止程序返回
}

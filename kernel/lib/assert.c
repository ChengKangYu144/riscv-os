#include "assert.h"
#include "console.h"   // 用你自己的 printf 或 panic

void __assert_fail(const char *msg, const char *file, int line)
{
    printf("Assertion failed: %s, file %s, line %d\n", msg, file, line);

    // 让系统停在这里（替代 abort）
    for(;;) {
        asm volatile("wfi");   // 或 while(1);
    }
}

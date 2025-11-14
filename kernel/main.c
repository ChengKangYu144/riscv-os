#include "console.h"
#include "mm/pmm.h"
#include "mm/vmm.h"

extern char etext[];
extern uint64 etext_addr;

void kernel_test(void);

int main()
{
    uart_init();

    /* 初始化物理内存管理并运行测试 */
    pmm_init(etext_addr, PHYSTOP);

    kernel_test();

    while (1);
    return 0;
}
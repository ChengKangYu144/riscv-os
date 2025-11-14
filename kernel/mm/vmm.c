#include "vmm.h"
#include "pmm.h"
#include "../include/riscv.h"
#include <string.h>

extern char etext[];
extern char end[];             // 所有初始化段之后的符号（.bss 结束）
uint64 etext_addr = (uint64)end;
pagetable_t kernel_pagetable = 0;

/* Create kernel page table and map kernel + devices */
void kvminit(void) {
    if (kernel_pagetable) return;
    kernel_pagetable = pagetable_create();
    if (!kernel_pagetable) return;

    /* 映射内核地址区（identity mapping: va == pa）:
       从 KERNBASE 到 PHYSTOP，需 RWX（代码/数据） */
    map_region(kernel_pagetable, KERNBASE, KERNBASE, PHYSTOP - KERNBASE, PTE_R | PTE_W | PTE_X);

    /* 映射 UART0（物理 MMIO），保证开启分页后仍能直接写 UART */
#ifdef UART0
    map_region(kernel_pagetable, UART0, UART0, PGSIZE, PTE_R | PTE_W);
#endif
}

/* activate kernel page table on this hart */
void kvminithart(void) {
    if (!kernel_pagetable) return;

    /* debug mark (可选): 在启用分页前后写 MMIO 便于定位 */
    // *(volatile unsigned char *)UART0 = 'G';

    /* kernel_pagetable 是内核虚拟地址，需要转成物理地址再取 PPN */
    uint64 pa = KVA2PA(kernel_pagetable);
    uint64 ppn = pa >> PGSHIFT;
    /* Sv39 MODE = 8 */
    uint64 satp_val = (8ULL << 60) | ppn;
    write_csr_satp(satp_val);
    sfence_vma();

    /* *(volatile unsigned char *)UART0 = 'H'; */
}

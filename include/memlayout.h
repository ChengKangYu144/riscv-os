#ifndef MEMLAYOUT_H
#define MEMLAYOUT_H

/* Physical memory layout for QEMU 'virt' default:
 * RAM base = 0x8000_0000, default size 128MiB (0x08000000)
 * Adjust PHYSTOP if your guest RAM is different.
 */
#define PHY_START    0x80000000UL
#define PHY_SIZE     0x08000000UL    /* 128 MiB */
#define PHYSTOP      (PHY_START + PHY_SIZE)

#ifndef KERNBASE
#define KERNBASE     PHY_START
#endif

/* UART0 MMIO base (用于在分页启用后映射 UART) */
#ifndef UART0
#define UART0 0x10000000UL
#endif

#define PGSIZE 4096
#define PGSHIFT 12

/* Sv39 uses 3-level page tables */
#define PT_LEVELS 3

/* Helpers to round */
#define PGROUNDUP(a)   ((((uint64)(a)) + PGSIZE - 1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((uint64)(a)) & ~(PGSIZE-1))

/* 物理地址 <-> 内核虚拟地址 映射（当前假定身份映射） */
#ifndef PA2KVA
#define PA2KVA(pa)   ((void *)((uintptr_t)(pa)))
#endif

#ifndef KVA2PA
#define KVA2PA(kva)  ((uint64)((uintptr_t)(kva)))
#endif

#endif

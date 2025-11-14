#ifndef VMM_H
#define VMM_H

#include "../include/types.h"
#include "../include/memlayout.h"
#include "page.h"

extern char etext[];   // 内核代码段结束地址（来自 linker script）
extern uint64 etext_addr;  // 改为 extern 声明，避免在头文件中定义变量

/* pagetable_t: pointer to one page (4KB) containing 512 pte_t entries */
typedef pte_t* pagetable_t;

/* Create an empty page table (allocates one page) */
pagetable_t pagetable_create(void);

/* Free a page table recursively (frees page table pages, not mapped physical pages) */
void pagetable_free(pagetable_t pt);

/* Walk page table: return pointer to pte for va.
 * If alloc==1, allocate intermediate page-table pages as needed using kalloc().
 * Returns NULL on error.
 */
pte_t* walk(pagetable_t pagetable, uint64 va, int alloc);

/* Map region [va, va+size) to physical [pa, pa+size) with permission perm (R/W/X/U bits).
 * Returns 0 on success, -1 on failure.
 */
int mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm);

/* 添加此声明以消除 vmm.c 中的隐式声明警告 */
void map_region(pagetable_t pagetable, uint64 va, uint64 pa, uint64 size, int perm);

/* Unmap pages in [va, va+size). This clears PTEs (does not free mapped physical pages). */
int unmap_pages(pagetable_t pagetable, uint64 va, uint64 size);

/* Top-level helpers for kernel:
 * - kvminit(): create kernel_pagetable and map kernel regions + devices
 * - kvminithart(): write satp and sfence.vma to activate on this hart
 */
extern pagetable_t kernel_pagetable;

void kvminit(void);
void kvminithart(void);

/* Helper: convert pagetable root pointer -> satp value (Sv39) */
#define SATP_MODE_SV39 (8ULL << 60)
static inline uint64 make_satp(pagetable_t pt) {
    uint64 root_pa = (uint64)PA2KVA(pt); /* if PA==KVA this is OK; otherwise replace with KVA2PA */
    return SATP_MODE_SV39 | (root_pa >> 12);
}

#endif

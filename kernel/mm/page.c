#include "vmm.h"
#include "pmm.h"
#include "page.h"
#include "../include/memlayout.h"
#include <string.h>

/* helpers */
#define PTE2PA(pte)   (((* (uint64 *)(pte)) >> 10) << 12)
#define PA2PTE(pa)    (((pa) >> 12) << 10)
#define PTE_FLAGS(pte) ((* (uint64 *)(pte)) & 0x3FF)

pagetable_t pagetable_create(void) {
    void *page = kalloc();
    if (!page) return 0;
    memset(page, 0, PGSIZE);
    return (pagetable_t)page;
}

/* walk pagetable, allocate intermediate page table if alloc != 0 */
pte_t* walk(pagetable_t pagetable, uint64 va, int alloc) {
    for (int level = PT_LEVELS - 1; level > 0; level--) {
        int idx = (va >> (12 + level*9)) & 0x1FF;
        pte_t *pte = &pagetable[idx];
        if ((*pte & PTE_V) == 0) {
            if (!alloc) return 0;
            void *np = kalloc();
            if (!np) return 0;
            memset(np, 0, PGSIZE);
            uint64 pa = KVA2PA(np);
            *pte = PA2PTE(pa) | PTE_V;
        }
        pagetable = (pagetable_t)PA2KVA(PTE2PA(pte));
    }
    int idx = (va >> 12) & 0x1FF;
    return &pagetable[idx];
}

/* map [va, va+size) to [pa, pa+size) with permissions perm (size must be multiple of PGSIZE) */
int mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm) {
    if (size % PGSIZE) return -1;
    for (uint64 offset = 0; offset < size; offset += PGSIZE) {
        pte_t *pte = walk(pagetable, va + offset, 1);
        if (!pte) return -1;
        if (*pte & PTE_V) return -1; /* already mapped */
        uint64 curpa = pa + offset;
        *pte = PA2PTE(curpa) | (perm & 0x3FF) | PTE_V;
    }
    return 0;
}

/* convenience: map region (va==pa allowed) */
void map_region(pagetable_t pagetable, uint64 va, uint64 pa, uint64 size, int perm) {
    /* ignore return; caller may assume success in this simple kernel */
    mappages(pagetable, va, size, pa, perm);
}
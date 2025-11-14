#include "pmm.h"
#include "page.h"
#include "../include/memlayout.h"
#include <string.h>

/* simple free-list allocator (per-page) */

struct run {
    struct run *next;
};

static struct run *freelist = 0;
static uint64 free_start = 0;
static uint64 free_end = 0;

/* pmm_init 恢复为正常实现：不再有调试输出 */
void pmm_init(uint64 start, uint64 end) {
    /* align */
    uint64 a = (start + PGSIZE - 1) & ~(PGSIZE - 1);
    uint64 b = end & ~(PGSIZE - 1);
    free_start = a;
    free_end = b;

    /* build free list */
    freelist = 0;
    for (uint64 p = a; p + PGSIZE <= b; p += PGSIZE) {
        kfree((void*)p);
    }
}

/* free a page (pa must be page aligned physical address) */
void kfree(void *pa) {
    if (!pa) return;
    /* simple debug guard: avoid freeing addresses outside range */
    uint64 p = (uint64)pa;
    if (p < free_start || p + PGSIZE > free_end) {
        /* out of range; ignore or hang - here we ignore */
        return;
    }
    /* optionally clear memory (debug) */
    memset((void*)PA2KVA(p), 1, PGSIZE);

    struct run *r = (struct run*)PA2KVA(p);
    r->next = freelist;
    freelist = r;
}

/* allocate one page, return kernel virtual address (identity PA->KVA) */
void* kalloc(void) {
    if (!freelist) return 0;
    struct run *r = freelist;
    freelist = r->next;
    /* clear allocated page */
    memset((void*)r, 0, PGSIZE);
    return (void*)r;
}

#ifndef PMM_H
#define PMM_H

#include "../include/types.h"

/* Initialize physical memory manager.
 * start: first free physical address (e.g. linker symbol 'end')
 * end:   one-past-last physical address available (e.g. PHYSTOP)
 */
void pmm_init(uint64 start, uint64 end);

/* allocate one physical page (returns physical address or 0) */
void* kalloc(void);

/* free one physical page (pa must be page-aligned) */
void kfree(void *pa);

#endif

#include "console.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "riscv.h"
#include <stdint.h>
#include "assert.h"

// ===========================================================
// Test 1: Physical Memory Allocator
// ===========================================================
void test_physical_memory(void)
{
    printf("[TEST] Physical Memory Allocator...\n");

    void *page1 = kalloc();
    void *page2 = kalloc();

    assert(page1 != 0);
    assert(page2 != 0);
    assert(page1 != page2);

    // check alignment
    assert(((uint64)page1 & 0xFFF) == 0);
    assert(((uint64)page2 & 0xFFF) == 0);

    // write & read test
    *(uint32_t *)page1 = 0x12345678;
    assert(*(uint32_t *)page1 == 0x12345678);

    // free and reallocate
    kfree(page1);
    void *page3 = kalloc();
    assert(page3 != 0);

    // free all
    kfree(page2);
    kfree(page3);

    printf("[ OK ] Physical memory test passed.\n\n");
}


// ===========================================================
// Test 2: Page Table
// ===========================================================
void test_pagetable(void)
{
    printf("[TEST] Page Table...\n");

    pagetable_t pt = pagetable_create();
    assert(pt != 0);

    uint64 va = 0x1000000UL;
    uint64 pa = (uint64)kalloc();

    assert(pa != 0);

    // map VA → PA
    /* mappages(pagetable, va, size_in_bytes, pa, perm) */
    assert(mappages(pt, va, PGSIZE, pa, PTE_R | PTE_W) == 0);

    // lookup PTE
    pte_t *pte = walk(pt, va, 0);
    assert(pte != 0);
    assert(*pte & PTE_V);

    // PA correct?
    assert(PTE2PA(*pte) == pa);

    // permission correct?
    assert(*pte & PTE_R);
    assert(*pte & PTE_W);
    assert(!(*pte & PTE_X));

    printf("[ OK ] Page table test passed.\n\n");
}


// ===========================================================
// Test 3: Virtual Memory Activation
// ===========================================================
void test_virtual_memory(void)
{
    printf("[TEST] Virtual Memory Activation...\n");

    printf("Before enabling paging...\n");

    kvminit();
    kvminithart();   // enable SATP + sfence.vma

    printf("After enabling paging...\n");

    // If kernel can still print, paging works.
    printf("[ OK ] Virtual memory active and functional.\n\n");
}


// ===========================================================
// Test Entry
// ===========================================================
void kernel_test(void)
{
    printf("========== Kernel Tests ==========\n");

    test_physical_memory();
    test_pagetable();
    test_virtual_memory();

    printf("========== All Tests Passed ==========\n");
}

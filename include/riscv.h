#ifndef RISCV_H
#define RISCV_H

#include <stdint.h>

static inline void write_csr_satp(uint64_t val) {
    asm volatile("csrw satp, %0" :: "r"(val) : "memory");
}

static inline void sfence_vma() {
    asm volatile("sfence.vma" ::: "memory");
}

#endif

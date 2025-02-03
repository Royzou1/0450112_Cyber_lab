
// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print 
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions 
// (e.g., rand()).  You may not use system().
#include <stdlib.h>

#include <inttypes.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

CYCLES measure_one_block_access_time(ADDR_PTR addr);

// You Should Not Use clflush in your final submission
// It is only used for debug
void clflush(ADDR_PTR addr);

char *string_to_binary(char *s);
char *binary_to_string(char *data);

int string_to_int(char* s);

int sum = 0;
static inline void mfence() {
    asm volatile("mfence");
}

void warmUp() {
    int tmp1 = 0;
    for (int i = 0 ; i < 10000000 ; i++) {
        tmp1 += rand() % 100;
    }
    sum += tmp1;
}

void flush_cache(int size , int *all_cache) {
    for (int i = 1 ; i < size ; i++)
    {
        all_cache[i] = rand()%25;
    }
    int it = 10 +  rand() % 20;
    int tmp = 0;
    for (int i = 0 ; i < it ; i++) {
        for (int i = 1 ; i < size ; i++)
        {
            mfence();
            sum += all_cache[i];
        }
    }
}


#endif

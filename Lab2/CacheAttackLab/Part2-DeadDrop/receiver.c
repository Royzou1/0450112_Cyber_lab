#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include "util.h"

#define BUFF_SIZE (2 * 12582912)
#define L3_SIZE 12582912
#define THRESH 250
#define SYNC_BYTE 0xAA
#define MESSAGE_COUNT 20
#define BIT_COUNT 8

static inline void mfence() {
    asm volatile("mfence" ::: "memory");
}

void warmUp() {
    volatile int sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += rand() % 100;
    }
}

void flush_cache(int size, int *cache) {
    for (int i = 0; i < size; i++) {
        cache[i] = rand() % 25;
    }
    mfence();
}

void main() {
    int *target_buffer = malloc(sizeof(int));
    if (!target_buffer) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int16_t buffer = 0;
    int hist[256] = {0};
    printf("Receiver listening...\n");

    while (true) {
        warmUp();
        mfence();
        *target_buffer = measure_one_block_access_time((uint64_t)target_buffer);
        printf("Time: %d\n", *target_buffer);
        
        buffer = (buffer << 1) | (*target_buffer > THRESH);
        printf("Received bit: %d\n", buffer & 1);
        
        if ((buffer >> 8) == SYNC_BYTE) {
            hist[buffer & 0xFF]++;
            printf("Received char: %c\n", buffer & 0xFF);
        }

        if (hist[buffer & 0xFF] == 3) break;
        sleep(1);
    }

    printf("Receiver finished, got: %c\n", buffer & 0xFF);
    free(target_buffer);
}
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
    uint64_t *buf = malloc(2 * L3_SIZE * sizeof(uint8_t));
    if (!buf) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    printf("Please type a message: ");
    char msg_char;
    scanf(" %c", &msg_char);
    int16_t msg = (SYNC_BYTE << 8) | msg_char;

    printf("Sender started\n");
    for (int i = 0; i < MESSAGE_COUNT; i++) {
        for (int bit = BIT_COUNT - 1; bit >= 0; bit--) {
            warmUp();
            if (msg & (1 << bit)) {
                flush_cache(L3_SIZE / 2, (int *)buf);
                printf("Sent bit: 1\n");
            } else {
                printf("Sent bit: 0\n");
            }
            sleep(1);
        }
        printf("Message %d sent\n", i + 1);
    }
    free(buf);
    printf("Sender finished\n");
}

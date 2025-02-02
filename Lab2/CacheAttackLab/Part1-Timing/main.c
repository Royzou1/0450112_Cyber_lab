#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TODO: Uncomment the following lines and fill in the correct size
#define L1_SIZE 32768
#define L2_SIZE 262144
#define L3_SIZE 12582912

static inline void mfence() {
    asm volatile("mfence");
}


void warmUp() {
    int tmp1 = 0;
    for (int i = 0 ; i < 10000000 ; i++) {
        tmp1 += rand() % 100;
    }
    printf("%d\n", tmp1);
}

void flush_cache(int size , int *all_cache) {
    for (int i = 1 ; i < size ; i++)
    {
        all_cache[i] = rand()%25;
    }
    int it = 100 +  rand() % 20;
    int tmp = 0;
    for (int i = 0 ; i < it ; i++) {
        for (int i = 1 ; i < size ; i++)
        {
            mfence();
            all_cache[i] += all_cache[i];
        }
    }
}


int main (int ac, char **av) {
    
    
    // create 4 arrays to store the latency numbers
    // the arrays are initialized to 0
    uint64_t dram_latency[SAMPLES] = {0};
    uint64_t l1_latency[SAMPLES] = {0};
    uint64_t l2_latency[SAMPLES] = {0};
    uint64_t l3_latency[SAMPLES] = {0};
    srand(time(0));
    // A temporary variable we can use to load addresses
    // The volatile keyword tells the compiler to not put this variable into a
    // register- it should always try to load from memory/ cache.
    volatile uint64_t tmp;

    // Allocate a buffer of 64 Bytes
    // the size of an unsigned integer (uint64_t) is 8 Bytes
    // Therefore, we request 8 * 8 Bytes
    uint64_t *target_buffer = (uint64_t*)malloc(L3_SIZE*sizeof(uint8_t));

    if (NULL == target_buffer) {
        perror("Unable to malloc");
        return EXIT_FAILURE;
    }

    // [1.2] TODO: Uncomment the following line to allocate a buffer of a size
    // of your chosing. This will help you measure the latencies at L2 and L3.
    uint64_t *eviction_buffer = (uint64_t*)malloc(L3_SIZE*sizeof(uint8_t));

    // Example: Measure L1 access latency, store results in l1_latency array
    warmUp();

    for (int i=0; i<SAMPLES; i++){
        // Step 1: bring the target cache line into L1 by simply accessing the line
        tmp = target_buffer[0];
        flush_cache (L2_SIZE/4 , (int*)eviction_buffer);
        // Step 2: measure the access latency
        l1_latency[i] = measure_one_block_access_time((uint64_t)target_buffer);
    }

    // ======
    // [1.2] TODO: Measure DRAM Latency, store results in dram_latency array
    // ======
    //
    for (int i = 0 ; i < SAMPLES ; i++) {
        int rand = random() % ((2*L3_SIZE) / 8);
        flush_cache(L3_SIZE/4 , (int*)eviction_buffer);
        dram_latency[i] = measure_one_block_access_time((uint64_t)(target_buffer + rand));
    }
    
    // ======
    // [1.2] TODO: Measure L2 Latency, store results in l2_latency array
    // ======
    //
    for (int i = 0; i < SAMPLES ; ++i) {
        int rand = random() % ((2*L3_SIZE) / 8);
        tmp += target_buffer[rand];
        
        l2_latency[i] = measure_one_block_access_time((uint64_t)(target_buffer + rand));
    }

    // ======
    // [1.2] TODO: Measure L3 Latency, store results in l3_latency array
    // ======
    //
    for (int i = 0; i < SAMPLES ; ++i) {
        int rand = random() % ((2*L3_SIZE) / 8);
        tmp += target_buffer[rand];
        flush_cache(L2_SIZE/4 , (int*)eviction_buffer);
        l3_latency[i] = measure_one_block_access_time((uint64_t)(target_buffer + rand));
    }


    // Print the results to the screen
    // [1.5] Change print_results to print_results_for_python so that your code will work
    // with the python plotter software
    print_results(dram_latency, l1_latency, l2_latency, l3_latency);

    free(target_buffer);

    // [1.2] TODO: Uncomment this line once you uncomment the eviction_buffer creation line
    free(eviction_buffer);
    return 0;
}


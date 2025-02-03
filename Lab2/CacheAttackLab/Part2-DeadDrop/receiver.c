
#include"util.h"
#include <unistd.h>
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#define THRESH 250
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


int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	//printf("Please press enter.\n");

	//char text_buf[2];
	//fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	int *target_buffer = (int*)malloc(sizeof(int));

	int16_t buffer = 0;
	char data = 0;
	int hist[256] = {0};

	bool listening = true;
	while (listening) {
		sum += *target_buffer;
		sleep(5);
		warmUp();
		mfence();
		*target_buffer = measure_one_block_access_time((uint64_t)target_buffer);
		printf("Time is: %d\n", *target_buffer);
		if (*target_buffer > THRESH){
			buffer =  (buffer << 1) + 1;
			printf("Receiver got bit = '1'\n");
		}
		else {
			printf("Receiver got bit = '0'\n");
			buffer =  (buffer << 1);
		}
		// Put your covert channel code here


		if ((buffer >> 8) == 0xAA) {
			hist[buffer % 256]++;
			printf("Receiver got: %d.\n" , buffer%256);
		}
		if (hist[buffer % 256] == 3)
			break;
	}

	printf("Receiver finished got: %d.\n" , buffer%256);

	printf("%d" , sum);
	return 0;
}



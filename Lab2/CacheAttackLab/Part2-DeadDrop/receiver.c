
#include"util.h"
#include <unistd.h>
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#define THRESH 250

static inline void mfence() {
    asm volatile("mfence");
}


int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please press enter.\n");

	char text_buf[2];
	//fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	uint64_t *target_buffer = (uint64_t*)malloc(sizeof(uint8_t));
	int16_t buffer = 0;
	char data = 0;
	int hist[256] = {0};
	int sum = 0;
	bool listening = true;
	measure_one_block_access_time((uint64_t)target_buffer);
	while (listening) {
		sum += *target_buffer;
		sleep(5);
		mfence();
		int time = measure_one_block_access_time((uint64_t)target_buffer);
		printf("Time is: %d\n", time);
		if (time > THRESH){
			buffer =  buffer << 1 + 1;
			printf("Receiver got bit = '1'\n");
		}
		else {
			printf("Receiver got bit = '0'\n");
			buffer =  buffer << 1;
		}
		// Put your covert channel code here
		if (buffer >> 8 == 0xAA) {
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



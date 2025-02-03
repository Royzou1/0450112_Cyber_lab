
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#define THRESH 1000



int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	uint64_t *target_buffer = (uint64_t*)malloc((uint8_t));
	int16_t buffer = 0;
	char data = 0;
	hist[256] = {0};

	bool listening = true;
	measure_one_block_access_time((uint64_t)target_buffer);
	while (listening) {
		sleep(1);
		if (measure_one_block_access_time((uint64_t)target_buffer) > THRESH)
			buffer =  buffer << 1 + 1;
		else 
			buffer =  buffer << 1;
		// Put your covert channel code here
		if (buffer >> 8 = 0xAA) {
			hist[buffer % 256]++;
			printf("Receiver got: %d.\n" , buffer%256);
		}
		if (hist[buffer % 256] == 3)
			break;
	}

	printf("Receiver finished got: %d.\n" , buffer%256);

	return 0;
}



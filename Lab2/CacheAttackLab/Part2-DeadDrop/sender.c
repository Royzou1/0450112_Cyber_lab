
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#include <unistd.h>

// TODO: define your own buffer size
#define BUFF_SIZE 2*12582912
//#define BUFF_SIZE [TODO]
#define L1_SIZE 32768
#define L2_SIZE 262144
#define L3_SIZE 12582912


int main(int argc, char **argv)
{
  // Allocate a buffer using huge page
  // See the handout for details about hugepage management
  void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
  
  if (buf == (void*) - 1) {
     perror("mmap() error\n");
     exit(EXIT_FAILURE);
  }
  // The first access to a page triggers overhead associated with
  // page allocation, TLB insertion, etc.
  // Thus, we use a dummy write here to trigger page allocation
  // so later access will not suffer from such overhead.
  //*((char *)buf) = 1; // dummy write to trigger page allocation


  // TODO:
  // Put your covert channel setup code here
  printf("Please type a message.\n");
  char msg1;
  scanf("%c",&msg1);
  int16_t msg = msg1 + 0xAA00; //msg is BC 
  int i = 20;
  int msg_counter = 0;

  bool sending = true;
  //char text_buf[128];
  //fgets(text_buf, sizeof(text_buf), stdin);
  while (sending) {

      // TODO:
      // Put your covert channel code here
      sleep(5);
      warmUp();
      int16_t mask = 1 << i;
      if (msg & mask) {//send bit = 1
        flush_cache(L3_SIZE/2 , (int*)buf);
        printf("sender sent '1'\n");
      }
      else {
        
        printf("sender sent '0'\n");
      }
      if (i == 0) {
        msg_counter++;
        i = 20;
        printf("msg %d sent by the sender.\n" , msg_counter);
      }
      else
        i--;
    if (msg_counter == 20) //send 20 messages
      break;
  }

  printf("Sender finished.\n");
  printf("%d" , sum);
  return 0;
}



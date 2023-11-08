#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
int main() {
int i;
void *shared_memory;
char buff[100];
int shmid;
// Create or get the shared memory segment
shmid = shmget((key_t)2345, 1024, 0666);
printf("Key of shared memory is %d\n", shmid);
// Attach to the shared memory segment
shared_memory = shmat(shmid, NULL, 0); // Process attached to shared memory segment
printf("Process attached at %p\n", shared_memory);
// Read data from shared memory
printf("Data read from shared memory is: %s\n", (char *)shared_memory);
// Detach from shared memory
shmdt(shared_memory);
return 0;
}

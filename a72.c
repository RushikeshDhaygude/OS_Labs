#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
int main() {
int i;
void *shared_memory;
char buff[100];
int shmid;
// Create or get the shared memory segment
shmid = shmget((key_t)2345, 1024, 0666 | IPC_CREAT);
printf("Key of shared memory is %d\n", shmid);
// Attach to the shared memory segment
shared_memory = shmat(shmid, NULL, 0);
printf("Process attached at %p\n", shared_memory);
printf("Enter some data to write to shared memory: ");
fgets(buff, sizeof(buff), stdin); // Get some input from the user
strncpy(shared_memory, buff, 100); // Data written to shared memory
// Detach from shared memory
shmdt(shared_memory);
return 0;
}

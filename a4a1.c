#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#define Buffer_Size 5
int in=0;int out=0;
int buffer[Buffer_Size];
sem_t empty,full;
pthread_mutex_t mutex;

void print_buffer(){
printf("Buffer: [");
for(int i=0;i<Buffer_Size;i++){
printf("%d",buffer[i]);
if(i<Buffer_Size-1)
printf(",");
}
 printf("]\n");
}
void *producer(void *arg){
	int item;
	while(1){
	item = rand() % 100;

        // Wait for an empty slot in the buffer
        sem_wait(&empty);

        // Acquire the mutex to update the buffer
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d Produced: %d\n", in, item);
        in = (in + 1) % Buffer_Size;
        pthread_mutex_unlock(&mutex);

        // Signal that a slot in the buffer is full
        sem_post(&full);

        // Print the current state of the buffer
        print_buffer();

        // Sleep for 2 seconds (simulating work)
        sleep(2);
	}
	pthread_exit(NULL);

}
void *consumer(void *arg){
int item;
	while(1){

	int item;

        // Wait for an empty slot in the buffer
        sem_wait(&full);

        // Acquire the mutex to update the buffer
        pthread_mutex_lock(&mutex);
        item=buffer[out];
        printf("Consumer %d Consumed: %d\n", out, item);
        out = (out + 1) % Buffer_Size;
        pthread_mutex_unlock(&mutex);

        // Signal that a slot in the buffer is full
        sem_post(&empty);

        // Print the current state of the buffer
        print_buffer();

        // Sleep for 2 seconds (simulating work)
        sleep(5);
	}
	pthread_exit(NULL);


}
int main(){
pthread_t producer_thread,consumer_thread;
sem_init(&empty,0,Buffer_Size);
sem_init(&full,0,0);
pthread_mutex_init(&mutex,NULL);

pthread_create(&producer_thread,NULL,producer,NULL);
pthread_create(&consumer_thread,NULL,consumer,NULL);

pthread_join(producer_thread, NULL);
pthread_join(consumer_thread, NULL);

return 0;
}

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, wrt;
int readcount = 0, nwt, nrd;

void *reader_thr(void *id) {
    int reader_id = *((int*)id);

    printf("\nReader %d is trying to read the database.\n", reader_id);

    sem_wait(&mutex);
    readcount++;
    if (readcount == 1)
        sem_wait(&wrt);
    sem_post(&mutex);

    printf("\nReader %d is reading in the database.\n", reader_id);

    sem_wait(&mutex);
    readcount--;
    if (readcount == 0)
        sem_post(&wrt);
    sem_post(&mutex);

    printf("Reader %d has finished reading.\n", reader_id);
    sleep(3);
    pthread_exit(NULL);
}

void *writer_thr(void *id) {
    int writer_id = *((int*)id);

    printf("\nWriter %d is trying to write to the database.\n", writer_id);

    sem_wait(&wrt);
    printf("\nWriter %d is writing to the database.\n", writer_id);
    sleep(3);
    printf("Writer %d has finished writing.\n", writer_id);
    sem_post(&wrt);

    pthread_exit(NULL);
}

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    pthread_t reader[100], writer[100];

    printf("Enter the number of readers: ");
    scanf("%d", &nrd);
    printf("Enter the number of writers: ");
    scanf("%d", &nwt);

    int reader_ids[nrd];
    int writer_ids[nwt];

    for (int i = 0; i < nwt; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writer[i], NULL, writer_thr, (void*)&writer_ids[i]);
    }

    for (int i = 0; i < nrd; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&reader[i], NULL, reader_thr, (void*)&reader_ids[i]);
    }

    for (int i = 0; i < nwt; i++) {
        pthread_join(writer[i], NULL);
    }

    for (int i = 0; i < nrd; i++) {
        pthread_join(reader[i], NULL);
    }

    pthread_exit(NULL);
}


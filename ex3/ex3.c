#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 3

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
sem_t mutex, empty, full;

void *producer(void *arg) {
    int item;
    while (1) {
        item = (int)(100.0*rand()/(RAND_MAX+1.0));
        sem_wait(&empty);
        sem_wait(&mutex);

        printf("Producer %ld: Produced item %d\n", (intptr_t)arg, item);
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);

        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item;
    while (1) {
        sem_wait(&full);
        sem_wait(&mutex);

        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        printf("\tConsumer %ld: Consumed item %d\n", (intptr_t)arg, item);

        sem_post(&mutex);
        sem_post(&empty);

        sleep(2); // 소비 시간을 나타내기 위해 잠시 대기
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producer1, producer2, consumer1, consumer2;
    int p1 = 1, p2 = 2, c1 = 1, c2 = 2;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // 쓰레드 생성
    pthread_create(&producer1, NULL, producer, (void *)(intptr_t)p1);
    pthread_create(&producer2, NULL, producer, (void *)(intptr_t)p2);
    pthread_create(&consumer1, NULL, consumer, (void *)(intptr_t)c1);
    pthread_create(&consumer2, NULL, consumer, (void *)(intptr_t)c2);

    // 쓰레드 대기
    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
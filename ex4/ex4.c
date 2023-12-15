#include <stdio.h>
#include <pthread.h>

#define MAX_CLIENTS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *client_thread(void *arg) {
    long client_id = (long)arg;


    pthread_mutex_lock(&mutex);

    printf("Client %ld: Sending message request\n", client_id);
    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&mutex);


    pthread_exit(NULL);
}

void *server_thread(void *arg) {

    while (1) {
        pthread_mutex_lock(&mutex);

        printf("Server: Waiting for message request\n");
        pthread_cond_wait(&cond, &mutex);

        printf("Server: Broadcasting message to all clients\n");

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t server_tid, client_tids[MAX_CLIENTS];
    long i;

    // 서버 쓰레드 생성
    pthread_create(&server_tid, NULL, server_thread, NULL);

    // 클라이언트 쓰레드들 생성
    for (i = 0; i < MAX_CLIENTS; i++) {
        pthread_create(&client_tids[i], NULL, client_thread, (void *)i);
    }

    // 쓰레드 대기
    pthread_join(server_tid, NULL);
    for (i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(client_tids[i], NULL);
    }

    return 0;
}
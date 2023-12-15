#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 3490

#define MAX_CLIENTS 4
#define BUFFER_SIZE 1024
#define EPOLL_SIZE 128

int clients[MAX_CLIENTS];
fd_set read_fds;
int server_socket;

void handle_new_connection() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1) {
        perror("accept failed");
        return;
    }

    printf("New client connected, socket fd is %d\n", client_socket);
    send(client_socket, "채팅에 오신 것을 환영합니다!\n", strlen("채팅에 오신 것을 환영합니다!\n"), 0);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] == 0) {
            clients[i] = client_socket;
            break;
        }
    }
}

void handle_client_message(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (bytes_received <= 0) {
        printf("Client %d disconnected\n", client_socket);
        close(client_socket);
        FD_CLR(client_socket, &read_fds);

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i] == client_socket) {
                clients[i] = 0;
                break;
            }
        }
    }
        // Broadcast the message to all other clients
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i] > 0 && clients[i] != client_socket) {
                send(clients[i], buffer, bytes_received, 0);
            }
        }
    
}

int main() {
    struct sockaddr_in server_addr;
    struct epoll_event ev, events[MAX_CLIENTS + 1];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket() failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind() failed");
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 6) < 0) {
        perror("listen failed");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on port 3490...\n");

    int epoll_fd = epoll_create(EPOLL_SIZE);
    if (epoll_fd < 0) {
        perror("epoll create failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &ev) < 0) {
        perror("epoll add error");
        close(server_socket);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int event_count = epoll_wait(epoll_fd, events, MAX_CLIENTS + 1, -1);
        if (event_count < 0) {
            perror("epoll_wait() error");
            break;
        }

        for (int i = 0; i < event_count; ++i) {
            if (events[i].data.fd == server_socket) {
                handle_new_connection();
            } else {
                handle_client_message(events[i].data.fd);
            }
        }
    }

    close(server_socket);
    close(epoll_fd);

    return 0;
}
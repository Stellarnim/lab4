#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3490
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket create error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("server connect error");
        close(client_socket);
        exit(1);
    }

    printf("Connected to server.\n");

    while (1) {
        char message[BUFFER_SIZE];

        int bytes_received = recv(client_socket, message, sizeof(message), 0);
        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            close(client_socket);
            break;
        }

        message[bytes_received] = '\0';
        printf("Received: %s", message);

        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);

        send(client_socket, message, strlen(message), 0);
        
        if (strcmp(message, "exit\n") == 0) {
            break;
        }
    }
    
    close(client_socket);

    return 0;
}
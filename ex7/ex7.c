#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_REQUEST_SIZE 2048

void handle_request(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 서버 소켓 생성
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 소켓과 주소를 바인딩
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // 연결 대기
    if (listen(server_socket, 10) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 클라이언트 요청 처리
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // 클라이언트 요청 처리 함수 호출
        handle_request(client_socket);

        // 소켓 닫기
        close(client_socket);
    }

    // 서버 소켓 닫기
    close(server_socket);

    return 0;
}

void handle_request(int client_socket) {
    char request[MAX_REQUEST_SIZE];
    ssize_t request_size = recv(client_socket, request, sizeof(request), 0);

    if (request_size == -1) {
        perror("Error receiving request");
        return;
    }

    // NULL 문자로 문자열 끝 지정
    request[request_size] = '\0';

    printf("Received request:\n%s\n", request);

    // 간단한 GET 요청 처리
    if (strstr(request, "GET") != NULL) {
        char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!\r\n";
        send(client_socket, response, sizeof(response), 0);
    }

    // 간단한 POST 요청 처리
    else if (strstr(request, "POST") != NULL) {
        char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPOST request received!\r\n";
        send(client_socket, response, sizeof(response), 0);
    }

    // CGI 프로그램 실행 (간단한 echo 프로그램)
    else if (strstr(request, "GET /cgi-bin/echo") != NULL) {
        char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

        // echo 프로그램 실행
        FILE *fp = popen("/bin/echo CGI Program", "r");
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            strcat(response, buffer);
        }
        pclose(fp);

        send(client_socket, response, strlen(response), 0);
    }
}
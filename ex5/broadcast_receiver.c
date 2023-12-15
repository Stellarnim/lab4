/* broadcast_receiver.c */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXRECVSTRING 255
int main(int argc, char *argv[])
{
int sock;
struct sockaddr_in bAddr;
unsigned int bPort;
char buf[MAXRECVSTRING + 1];
int bufLen;
if (argc != 2) {
fprintf(stderr, "Usage: %s <Broadcast Port>\n", argv[0]);
exit(1);
}
bPort = atoi(argv[1]);
if ((sock = socket(AF_INET, SOCK_DGRAM,
IPPROTO_UDP)) < 0) {
perror("socket() failed");
exit(1);
}
memset(&bAddr, 0, sizeof(bAddr));
bAddr.sin_family = AF_INET;
bAddr.sin_addr.s_addr = htonl(INADDR_ANY);
bAddr.sin_port = htons(bPort);
if (bind(sock, (struct sockaddr *) &bAddr,
sizeof(bAddr)) < 0) {
perror("bind() failed");
exit(1);
}
if ((bufLen = recvfrom(sock, buf,
MAXRECVSTRING, 0, NULL, 0)) < 0) {
perror("recvfrom() failed");
exit(1);
}
buf[bufLen] = '\0';
printf("Received: %s\n", buf);
close(sock);
}
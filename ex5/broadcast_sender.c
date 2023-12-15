/* broadcast_sender.c */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define bIP "255.255.255.255"

int main(int argc, char *argv[])
{
int sock;
struct sockaddr_in bAddr;
unsigned short bPort; /* 브로드캐스트할 포트 */
char *buf;
int broadcastPermission;
unsigned int bufLen;
if (argc != 3) {
fprintf(stderr, "Usage: %s <포트번호> <보낼문자열>\n", argv[0]);
exit(1);
}
bPort = atoi(argv[1]);
buf = argv[2];
if ((sock = socket(AF_INET, SOCK_DGRAM,
IPPROTO_UDP)) < 0) {
perror("socket() failed");
exit(1);
}
broadcastPermission = 1;
if (setsockopt(sock, SOL_SOCKET,
SO_BROADCAST, (void *) &broadcastPermission,
sizeof(broadcastPermission)) < 0) {
perror("setsockopt() failed");
exit(1);
}
memset(&bAddr, 0, sizeof(bAddr));
bAddr.sin_family = AF_INET;
bAddr.sin_addr.s_addr = inet_addr(bIP);
bAddr.sin_port = htons(bPort);
bufLen = strlen(buf);
if (sendto(sock, buf, bufLen, 0, (struct sockaddr *) &bAddr, sizeof(bAddr)) != bufLen) {
perror("전송된 바이트 수 틀림");
exit(1);
}
return 0;
}
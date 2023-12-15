#include <stdio.h> /* printf() and fprintf() */
#include <sys/socket.h> /* socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h> /* sockaddr_in, inet_addr() */
#include <stdlib.h> /* atoi() and exit() */
#include <string.h> /* memset() */
#include <unistd.h> /* close() */

#define MAXRECVSTRING 255

int main(int argc, char *argv[])
{
int sock; /* Socket */
struct sockaddr_in mAddr; /* Multicast Address */
char *mIP; /* IP Multicast Address */
unsigned int mPort; /* Port */
char buf[MAXRECVSTRING + 1]; /* Buffer for received string */
int bufLen; /* Length of received string */
struct ip_mreq mReq; /* Multicast address join structure */
if (argc != 3) {
fprintf(stderr, "Usage: %s <멀티캐스트 IP 주소> <멀티캐스트 포트>\n", argv[0]);
exit(1);
}
mIP = argv[1];
mPort = atoi(argv[2]);
/* 소켓 생성 */
if ((sock = socket(PF_INET, SOCK_DGRAM,
IPPROTO_UDP)) < 0) {
perror("소켓 생성 실패");
exit(1);
}
/* bind에 사용할 주소 지정 */
memset(&mAddr, 0, sizeof(mAddr));
mAddr.sin_family = AF_INET;
mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
mAddr.sin_port = htons(mPort);
/* bind */
if (bind(sock, (struct sockaddr *) &mAddr, sizeof(mAddr)) < 0) {
perror("bind() 실패");
exit(1);
}
/* 멀티캐스트 수신 구조체 ip_mreq 설정 */
mReq.imr_multiaddr.s_addr = inet_addr(mIP);
mReq.imr_interface.s_addr = htonl(INADDR_ANY);

/* 멀티캐스트 그룹 가입 소켓 옵션 설정 */
if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &mReq, sizeof(mReq)) < 0) {
perror("소켓 옵션 설정 실패");
exit(1);
}
/* 멀티캐스트 패킷 수신 */
memset(buf, 0, MAXRECVSTRING);
if ((bufLen = recvfrom(sock, buf, MAXRECVSTRING, 0, NULL, 0)) < 0) {
perror("수신 실패");
exit(1);
}
printf("Received: %s\n", buf); /* Print the received string */
close(sock);
exit(0);
}

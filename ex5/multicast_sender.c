#include <stdio.h> /* fprintf() */
#include <sys/socket.h> /* socket(), connect(), send(), and recv() */
#include <arpa/inet.h> /* sockaddr_in,inet_addr() */
#include <stdlib.h> /* atoi() and exit() */
#include <string.h> /* memset() */
#include <unistd.h> /* sleep() */
int main(int argc, char *argv[])
{
int sock; /* Socket */
struct sockaddr_in mAddr; /* Multicast address */
char *mIP; /* IP Multicast address */
unsigned short mPort; /* Server port */
char *buf; /* String to multicast */
unsigned char mTTL; /* TTL of multicast packets 
*/
unsigned int bufLen; /* Length of string to 
multicast */
if ((argc < 4) || (argc > 5)) { /* Test for
correct number of parameters */
fprintf(stderr, "Usage: %s <멀티캐스트 IP 주소 > < 포 트 번 호 > < 문자열 > <TTL>\n", argv[0]);
exit(1);
}
mIP = argv[1];
mPort = atoi(argv[2]);
buf = argv[3];
mTTL = atoi(argv[4]);
/* 소켓 생성 */
if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
perror("소켓 생성 실패");
exit(1);
}
/* 멀티캐스트 TTL 설정 */
if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &mTTL, sizeof(mTTL)) < 0) {
perror("소켓 옵션 설정 실패");
exit(1);
}
/* 멀티캐스트 주소와 멀티캐스트 포트 설정 */
memset(&mAddr, 0, sizeof(mAddr));
mAddr.sin_family = AF_INET;
mAddr.sin_addr.s_addr = inet_addr(mIP);
mAddr.sin_port = htons(mPort);
bufLen = strlen(buf);
if (sendto(sock, buf, bufLen, 0, (struct sockaddr *) &mAddr, sizeof(mAddr)) != bufLen) {
perror("sendto() 전송 실패");
exit(1);
}
}
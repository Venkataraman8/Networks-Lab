#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#define SERV_PORT 49312
#define MAXLINE 1024
char *END_FLAG = "================END";
int main(int argc, char **argv)
{
int sockfd, n, fd;
struct sockaddr_in servaddr;
char buf[MAXLINE];
char *target, *path;
bzero(&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(SERV_PORT);
inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
sockfd = socket(AF_INET, SOCK_DGRAM, 0);
fd = open("client.txt", O_RDONLY);
while ((n = read(fd, buf, MAXLINE)) > 0) {
sendto(sockfd, buf, n, 0, (struct sockaddr *) &servaddr,
sizeof(servaddr));
}
sendto(sockfd, END_FLAG, strlen(END_FLAG), 0, (struct sockaddr *)
&servaddr, sizeof(servaddr));
printf("File send to the server\n");
return 0;
}
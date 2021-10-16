#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/ioctl.h>
#include<netdb.h>
#define PORT 8080
#define MAXLINE 2048
int main()
{
	int sockfd,connfd,len;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr,cliaddr;
	bzero(&servaddr,sizeof(servaddr));
	bzero(&cliaddr,sizeof(cliaddr));
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(PORT);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	listen(sockfd,2);
	len=sizeof(cliaddr);
	connfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);
	read(connfd,buffer,MAXLINE);
	printf("%s",buffer);
	FILE *fp;
	fp=fopen("text2.txt","w");
	fprintf(fp,"%s",buffer);
	printf("File received successfully\n");
	close(sockfd);
}

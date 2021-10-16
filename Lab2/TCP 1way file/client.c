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
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(PORT);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	bzero(buffer,sizeof(buffer));
	FILE *f;
	f=fopen("text1.txt","r");
	for(int i=0;i<MAXLINE;i++)
	{
		buffer[i]=fgetc(f);
		if(buffer[i]==EOF)
			break;
	}
	write(sockfd,buffer,sizeof(buffer));
	printf("%s",buffer);
	printf("File sent to the server\n");
	close(sockfd);
}

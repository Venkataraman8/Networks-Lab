#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 
typedef enum{ DATA,ACK }   MSGKIND;
struct MESSAGE
{
    MSGKIND type;
    int seq;
    unsigned int len;
    char msg[100];
    int parity;
};

int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
        
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 


    while(1)
    {
                        //send(sock,(void*)Acknowledge, sizeof(struct MESSAGE), 0);
                    struct MESSAGE* Message = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));

                    if(recv(sock, Message, sizeof(struct MESSAGE), 0) > 0)
                    {
                        printf("Seq:%d Message recvd: %s\n",Message->seq,Message->msg);
                    }
           
    }
 
             
    close(sock);
    return 0;
} 
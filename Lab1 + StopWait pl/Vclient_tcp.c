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
    // Declare the file pointer 
    FILE *filePointer ; 
      
    // Get the data to be written in file 
    char dataToBeWritten[100] ;
  
    filePointer = fopen("write.txt", "w") ; 
      

    if ( filePointer == NULL ) 
    { 
        printf( "file failed to open." ) ; 
    } 
    else
    { 
            int s=1;
            char null[1]={'N'};
            int i=0;
            int count=0;
            while(i<5)
            {
                while(1)
                {
                    count++;
                    struct MESSAGE* Message = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                    struct MESSAGE* Acknowledge = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                    Acknowledge->type = ACK;
                    Acknowledge->len = 0;
                    strcpy(Acknowledge->msg, null);
                    Acknowledge->seq=s;
                    // if(count!=2)
                        send(sock,(void*)Acknowledge, sizeof(struct MESSAGE), 0);
                    if(recv(sock, Message, sizeof(struct MESSAGE), 0) > 0)
                    {
                        if(Message->type == DATA && Message->seq + s == 1)
                        {
                            int sum=0;
                            for(int j=0;j<strlen(Message->msg);j++)
                            {
                                sum+=Message->msg[j];
                            }
                            if(Message->parity!=sum%2)
                            {
                                printf("Error on seq:%d  lineno:%d",s,i);
                                continue;
                            }
                            if(s==0) s=1;
                            else s=0;
                            strcpy(dataToBeWritten, Message->msg);
                            fputs(dataToBeWritten, filePointer) ;    
                            break;
                        }
                    }
                }
            i++;
            }
 
            
        // Closing the file using fclose() 
        fclose(filePointer) ; 
          
    } 
    close(sock);
    return 0;
} 
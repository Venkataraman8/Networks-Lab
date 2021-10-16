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
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    serv_addr.sin_port = htons( PORT );
    
    int len, n;
    len = sizeof(serv_addr);

    // Declare the file pointer 
    FILE *filePointer ; 
      
    // Get the data to be written in file 
    char dataToBeWritten[100] ;
  
    // Open the existing file GfgTest.c using fopen() 
    // in write mode using "w" attribute 
    filePointer = fopen("write.txt", "w") ; 
      
    // Check if this filePointer is null 
    // which maybe if the file does not exist 
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
            while(1)
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

                    // if(count!=3)
                        sendto(sock, (void*)Acknowledge, sizeof(struct MESSAGE), 0, ( struct sockaddr *) &serv_addr, len);
                    // else
                    // {
                    //     sleep(3);
                    //     sendto(sock, (void*)Acknowledge, sizeof(struct MESSAGE), 0, ( struct sockaddr *) &serv_addr, len);
                    // }
                    
                    
                    if(recvfrom(sock, (void *)Message, sizeof(struct MESSAGE), 0, (  struct sockaddr *) &serv_addr, &len)> 0)
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
                                printf("Error in seq %d lineno: %d",s,count);
                                continue;
                            }
                            printf("%d",Message->parity);
                            if(s==0) s=1;
                            else s=0;
                            strcpy(dataToBeWritten, Message->msg);
                            fputs(dataToBeWritten, filePointer) ;    
                            break;
                        }
                    }
                }
            }
 
            
        // Closing the file using fclose() 
        fclose(filePointer) ; 
          
    } 
    close(sock);
    return 0;
} 
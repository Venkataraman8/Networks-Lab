#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
typedef enum{ DATA,ACK }   MSGKIND;
struct timeval timeout;

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
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
        
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    if (setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }
    // Declare the file pointer 
    FILE *filePointer ; 
    // Declare the variable for the data to be read from file 
    char dataToBeRead[100]; 
  
    filePointer = fopen("read.txt", "r") ; 

    if ( filePointer == NULL ) 
    { 
        perror( "file failed to open." ) ; 
    } 
    else
    { 
           
          

        int flag=1, s=0;
        int count=0;
        while( fgets ( dataToBeRead, 100, filePointer ) != NULL ) 
        { 
            while(1)
            {
                count++;
                struct MESSAGE* Message = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                struct MESSAGE* Acknowledge = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                Message->type = DATA;
                Message->len = strlen(dataToBeRead);
                strcpy(Message->msg, dataToBeRead);
                Message->seq = s;
                int sum=0;
                for(int j=0;j<strlen(dataToBeRead);j++)
                {
                    sum+=dataToBeRead[j];
                }
                    Message->parity =  sum%2;

                send(new_socket,(void*)Message, sizeof(struct MESSAGE), 0);
                printf("MSG: %d-%s\n",s,dataToBeRead);
                if(recv(new_socket, Acknowledge, sizeof(struct MESSAGE), 0) > 0)
                {
                    printf("ACK: %d\n",Acknowledge->seq);
                    if(Acknowledge->type == ACK && Acknowledge->seq == s)
                    {
                        sleep(1);
                        // if(flag==0) flag=1;
                        // else flag=0;
                        break;
                    }
                }
                

            }
            if(s==0) s=1;
            else s=0;

         } 
          
        // Closing the file using fclose() 
        fclose(filePointer) ; 
          
    } 
    close(new_socket);
    return 0; 
} 
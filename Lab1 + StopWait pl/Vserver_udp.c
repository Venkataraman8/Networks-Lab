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
    int server_fd; 
    struct sockaddr_in servaddr, cliaddr;  
    char buffer[1024] = {0}; 
        
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr)); 
    if (setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&servaddr,  
                                 sizeof(servaddr))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    int len, n;
    len = sizeof(cliaddr);

    // Declare the file pointer 
    FILE *filePointer ; 
    // Declare the variable for the data to be read from file 
    char dataToBeRead[100]; 
  
    // Open the existing file GfgTest.c using fopen() 
    // in read mode using "r" attribute 
    filePointer = fopen("read.txt", "r") ; 
      
    // Check if this filePointer is null 
    // which maybe if the file does not exist 
    if ( filePointer == NULL ) 
    { 
        perror( "file failed to open." ) ; 
    } 
    else
    { 
           
          
        // Read the dataToBeRead from the file 
        // using fgets() method 
        int flag=1, s=0;
        int count=0;
        recvfrom(server_fd, (void *)buffer, sizeof(struct MESSAGE), 0, ( struct sockaddr *) &cliaddr, &len);
        
        while( fgets ( dataToBeRead, 100, filePointer ) != NULL ) 
        { 
            
            while(count<10)
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
                if(count%3!=0)
                    Message->parity =  sum%2;
                else
                    Message->parity = 3;  

                sendto(server_fd, (void*)Message, sizeof(struct MESSAGE), 0, ( struct sockaddr *) &cliaddr, len); 
                printf("MSG: %d-%s\n",s,dataToBeRead);
                if( recvfrom(server_fd, (void *)Acknowledge, sizeof(struct MESSAGE), 0, ( struct sockaddr *) &cliaddr, &len)> 0)
                {
                    printf("ACK: %d\n",Acknowledge->seq);
                    if(Acknowledge->type == ACK && Acknowledge->seq == s)
                    {
                        sleep(1);
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
    close(server_fd);
    return 0; 
} 
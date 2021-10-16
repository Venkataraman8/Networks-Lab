//PUT SOCKET.ACCEPT MULTIPLE INSIDE WHILE LOOP TO ALLOW DYNAMIC JOINS, LEAVING
//EDIT CURRENT CODE


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
    // int parity;
};

int main(int argc, char const *argv[]) 
{ 
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int server_fd, new_socket[5], valread; 
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
    if (listen(server_fd, 5) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    for(int j=0;j<5;j++)
    {
        if ((new_socket[j] = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
        if (setsockopt(new_socket[j], SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) 
        { 
            perror("setsockopt"); 
            exit(EXIT_FAILURE); 
        }
    }
    printf("All 5 client sockets accepted!\n");
           
    int clients[5];
    int n;
    int count=0;
    while(1)
    {
        count++;
        struct MESSAGE* Message = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
        Message->type = DATA;
        char news[100]="KKR beat CSK by 10 runs";
        printf("Msg%d: %s\n",count,news);
        Message->len = strlen(news);
        strcpy(Message->msg, news);
        Message->seq = count;
        scanf("%d",&n);
        if(n==0)
        {
            printf("Closing server...\n");
            break;
        }
        else if(n==5)
        {
            printf("Broadcasting...\n");
            sleep(1);
            for(int i=0;i<n;i++)
            {
                send(new_socket[i],(void*)Message, sizeof(struct MESSAGE), 0);
            }
        }
        else
        {
            for(int i=0;i<n;i++)
            {
                scanf("%d",&clients[i]);
            }
            printf("Multicasting to: ");
            for(int i=0;i<n;i++)
                printf("%d ",clients[i]);
            printf("\n");
            sleep(1);

            for(int i=0;i<n;i++)
            {
                send(new_socket[clients[i]],(void*)Message, sizeof(struct MESSAGE), 0);
            }
        }
    } 
    for(int i=0;i<5;i++)    
        close(new_socket[i]);
    return 0; 
} 
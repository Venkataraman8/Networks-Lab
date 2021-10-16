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
    int msg;
};

int main(int argc, char *argv[]) 
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


        int serv_win = 3;
        int flag=1, s=0;
        int count=0;
        int i=0;
        int max=10;
        int num=0;

        if(argc>2)
        exit(0);
        printf(argv[1]);
        if(strcmp(argv[1],"1")==0)
        {
        //Go back N
        printf("Go back n\n");
        int count=0;
        while(count<max)
        {

            int right= s+serv_win;
            for(int count=s;count<right;count++)
            {
                //Send all messages in window
                num++;
                struct MESSAGE* Message = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                struct MESSAGE* Acknowledge = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                Message->type = DATA;
                Message->len = 1;
                Message->msg=count;
                Message->seq = count;
                // if(num!=2)
                {
                    send(new_socket,(void*)Message, sizeof(struct MESSAGE), 0);
                    printf("MSG: %d\n",count);
                }
                if(recv(new_socket, Acknowledge, sizeof(struct MESSAGE), 0) > 0)
                {
                    printf("ACK: %d\n",Acknowledge->seq);
                    if(Acknowledge->type == ACK && Acknowledge->seq == s)
                    {
                        sleep(1);
                        //move winddow left limit
                        s++;
                        if(count>max)
                        break;
                    }
                }  

            }
        }
        }
        else if(strcmp(argv[1],"2")==0)
        //Selective Repeat
        {
            printf("Selective Repeat\n");
            int buffer[serv_win];
            memset(buffer,0,sizeof(buffer));
            int num=0;
            int right= s+serv_win;
            while(count<max)
            {
            
            for(int count=s;count<right;count++)
            {
                num++;
                if(count>max)
                break;

                //Send message only if not acknowledged
                if( count>=s && buffer[count-s]==0)
                {
                struct MESSAGE* Message = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                Message->type = DATA;
                Message->len = 1;
                Message->msg=count;
                Message->seq = count;
                    // if(num!=2)
                    {
                    send(new_socket,(void*)Message, sizeof(struct MESSAGE), 0);
                    printf("MSG: %d s=%d\n",count,s);
                    }
                }

                //Receive ACK
                struct MESSAGE* Acknowledge = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                if(recv(new_socket, Acknowledge, sizeof(struct MESSAGE), 0) > 0)
                {
                    printf("ACK: %d\n",Acknowledge->seq);
                    if(Acknowledge->type == ACK && Acknowledge->seq >=s && Acknowledge->seq < s+serv_win && buffer[Acknowledge->seq-s]==0)
                    {
                        sleep(1);
                        //Window element acknowledged
                        buffer[Acknowledge->seq-s]=1;
                    }
                } 

                int flag=1;
                for(int p=0;p<serv_win;p++)
                {
                    if(buffer[p]==0)
                    {
                        flag=-1;
                        break;
                    }
                }
                if(flag==1)
                {
                    s+=serv_win;
                    right+=serv_win;
                    memset(buffer,0,sizeof(buffer));
                }
            }
            }
        }   
           
    close(new_socket);
    return 0; 
} 
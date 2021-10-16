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
    int msg;
};

int main(int argc, char *argv[]) 
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
    
            int s=0;
            char null[1]={'N'};
            int i=0;
            int count=0;
            int max=10;
            int recv_win=3;
    if(argc>2)
        exit(0);
    printf(argv[1]);
    if(strcmp(argv[1],"1")==0)
    //Go Back n
    {
        printf("Goback n\n");
        int num=0;
        int temp;
            while(s<max)
            {
                num++;
                struct MESSAGE* Message = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                if(recv(sock, Message, sizeof(struct MESSAGE), 0) > 0)
                    {
                        if(Message->type == DATA && Message->seq == s)
                        {
                            printf("Recvd: %d Acc\n",Message->seq);
                            s++;
                            temp=s;
                        }
                        else
                        {
                            printf("Recvd: %d Disc expecting %d\n",Message->seq,s);
                            if(Message->seq<s)
                            {
                                temp=s;    
                                s=Message->seq;
                            }
                        }
                        
                    }
                struct MESSAGE* Acknowledge = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));
                Acknowledge->type = ACK;
                Acknowledge->len = 0;
                Acknowledge->msg=-1;
                Acknowledge->seq=s-1;
                
                send(sock,(void*)Acknowledge, sizeof(struct MESSAGE), 0);

                if(temp!=s)
                s=temp;
            }
    }
    else if(strcmp(argv[1],"2")==0)
    {
    // Selective repeat
    printf("Selective Repeat\n");
        int ind=0;
        int temp;
        count=0;
        int buffer[recv_win];
        memset(buffer,0,sizeof(buffer));
        //for(int count=s;count<s+recv_win;s++)
        int left=0;
        int right=recv_win-1;
        int num=0;
        while(left<max)
        {
                num++;
                struct MESSAGE* Message = (struct MESSAGE*) malloc(sizeof(struct MESSAGE)); 
                if(recv(sock, Message, sizeof(struct MESSAGE), 0) > 0)
                    {
                        if(Message->type == DATA && Message->seq >= left && Message->seq <= right && buffer[Message->seq-left]==0)
                        {
                            printf("Recvd: %d Acc L:%d R:%d\n",Message->seq,left,right);
                            buffer[Message->seq-left]=1;
                            ind=Message->seq;
                            temp=ind;
                        }
                        else
                        {
                            printf("Recvd: %d Disc L:%d R:%d\n",Message->seq,left,right);
                            if(Message->seq<=left)
                            {
                                temp=ind;
                                ind=Message->seq;
                            }
                        }
                    }

                    struct MESSAGE* Acknowledge = (struct MESSAGE*) malloc(sizeof(struct MESSAGE));               
                    Acknowledge->type = ACK;
                    Acknowledge->len = 0;
                    Acknowledge->msg=-1;
                    Acknowledge->seq=ind;
                        send(sock,(void*)Acknowledge, sizeof(struct MESSAGE), 0);
                    
                    if(ind!=temp)
                        ind=temp;
                
                int flag=1;
                for(int p=left;p<=right;p++)
                {
                    if(buffer[p-left]==0)
                    {
                        flag=-1;
                        //printf("%d",p);
                        break;
                    }
                }
                if(flag==1)
                {
                    left+=recv_win;
                    right+=recv_win;
                    memset(buffer,0,sizeof(buffer));
                }
                
            
        }
    }       
            
  
    close(sock);
    return 0;
} 
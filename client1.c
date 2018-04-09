#include <stdio.h>
#include <sys/types.h>//system type definitions
#include <sys/socket.h>//network system functions
#include <netinet/in.h>// protocol & struct definition 
#include <string.h>
#include <unistd.h>

int main()
{
    int i, sock; 
    char x_cordinate[80], y_cordinate[80]; 
    char still_playing[80]; 
    struct sockaddr_in  addr_send; 
    unsigned short  server_port = 60000;  
    

    //creating a socket 
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
    {
        printf("Socket() failed\n"); 
    }

   

    //make local address structure 
    memset(&addr_send, 0, sizeof(addr_send));  //clear structure 
    addr_send.sin_family = PF_INET; //family address
    addr_send.sin_port = htons(server_port);
 
    //connect to server 
    i = connect(sock, &addr_send, sizeof(addr_send)); 
    if (i < 0)
        printf("connect() failed\n"); 
    do 
    {
        printf("\nEnter x-position: ");
        scanf("%s",x_cordinate); 
        printf("\nEnter y-position: ");
        scanf("%s",y_cordinate); 
        printf("\nContinue Playing (y/n) ?  "); 
        scanf("%s",still_playing); 

        send(sock,still_playing,strlen(still_playing), 0);
        send(sock,x_cordinate,strlen(x_cordinate), 0);
        send(sock,y_cordinate,strlen(y_cordinate), 0);

    }while(strcmp("n",still_playing)!= 0); 

   //close commmunication 
    i = close (sock); 
    if (i < 0 )
        printf("close() failed\n"); 
    return 0; 
}
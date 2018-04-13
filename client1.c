#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>//system type definitions
#include <sys/socket.h>//network system functions
#include <netinet/in.h>// protocol & struct definition
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP	"127.0.0.1"

struct msg
{
  char message[10];
};
int main()
{
    int i, sock;

    struct sockaddr_in  addr_send;
    unsigned short  server_port = 60000;
    struct mmsghdr send_msg;
    //char x_cordinate[1],  y_cordinate[1], letter[1], still_playing[1];
    struct msg message[4];
    int bytes_sent;
    int len;
    //creating a socket
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
    {
        printf("Socket() failed\n");
    }
    //make local address structure
    memset(&addr_send, 0, sizeof(addr_send));  //clear structure
    addr_send.sin_family = AF_INET; //family address
    addr_send.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr_send.sin_port = htons(server_port);

    //connect to server
    //i = connect(sock, (struct sockaddr *)&addr_send, sizeof(addr_send));
    if (i < 0)
        printf("connect() failed\n");

        printf("\nEnter still playing: ");
        scanf("%s",(message[3].message));

    while(strcmp(message[3].message,"n")!=0)
    {
        printf("\nEnter x_cordinate: ");
        scanf("%s",(message[0].message));

        printf("\nEnter y_cordinate: ");
        scanf("%s",(message[1].message));

        printf("\nEnter letter: ");
        scanf("%s",(message[2].message));
        // send bytes
        for(int i=0; i<4; i++)
        {
          char buf[10];
          strcpy(buf, (message[i].message) );
          len = strlen(message[i].message);
          printf("%s\n",buf);
          bytes_sent=sendto(sock, buf, len, 0,(struct sockaddr *) &addr_send, sizeof(addr_send));
        }
        //check if they want to continue playing
        printf("\nEnter still playing: ");
        scanf("%s",(message[3].message));
    }
   //close commmunication
    i = close (sock);
    if (i < 0 )
        printf("close() failed\n");
    return 0;
}

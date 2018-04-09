#include <stdio.h>
#include <sys/types.h>//system type definitions
#include <sys/socket.h>//network system functions
#include <netinet/in.h>// protocol & struct definition
#include <string.h>
#include <unistd.h>

int main()
{
    int i, sock;
    char x_cordinate, y_cordinate, letter;
    char still_playing;
    struct sockaddr_in  addr_send;
    unsigned short  server_port = 60001;


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
    i = connect(sock, (struct sockaddr *)&addr_send, sizeof(addr_send));
    if (i < 0)
        printf("connect() failed\n");
    do
    {
        printf("\nEnter x-position: ");
        x_cordinate = getchar();
        getchar();
        printf("\nEnter y-position: ");
        y_cordinate = getchar();
        getchar();
        printf("\nEnter letter: ");
        letter = getchar();
        getchar();
        printf("\nContinue Playing (y/n) ?  ");
        still_playing = getchar();
        getchar();

        send(sock,still_playing,sizeof(char ), 0);
        send(sock,x_cordinate,sizeof(char), 0);
        send(sock,y_cordinate,sizeof(char), 0);
        send(sock,letter,sizeof(char), 0);

    }while(still_playing!='n');

   //close commmunication
    i = close (sock);
    if (i < 0 )
        printf("close() failed\n");
    return 0;
}

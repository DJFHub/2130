#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>//system type definitions
#include <sys/socket.h>//network system functions
#include <netinet/in.h>// protocol & struct definition
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
int canPlay = 0 ; //intialized to false
#define SERVER_IP	"127.0.0.1"
#define MAX_LEN 4

struct msg
{
  char message[10];
};

void getPlayerMove();
void quitGame();
struct msg message[4];
int sock;
int i;

struct sockaddr_in  addr_send;
unsigned short  server_port = 60000;
struct mmsghdr send_msg;
//char x_cordinate[1],  y_cordinate[1], letter[1], still_playing[1];

int bytes_sent;
int len,incoming_len,addr_len;
char buf[10];

int main()
{
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

        //printf("\nEnter still playing: ");
        //scanf("%s",(message[3].message));

//-------Game establishing player connections ------------
    // this one response  should be sent yo the server
    printf("\nWant to play (y/n)? ");
    scanf("%s",(message[3].message));

    if (strcmp(message[3].message,"n")!=0)
    {
      addr_len = sizeof(addr_send);
      len = strlen(message[3].message);
      //tell the server you want to play
      sendto(sock,message[3].message,len, 0,(struct sockaddr *) &addr_send, addr_len);
    }
    else
      exit(0);

    printf("\nwaiting on opponent to play..\n");

    for(;;)//loop forever
    {
        /*-------comment-----------
         each loop should check if the player
         canplay or is still waiting on the opponent
         this message should be sent by the server
        */

        if (canPlay != 0)
        {

          getPlayerMove();
          // send bytes
          for(int i=0; i<4; i++)
          {
            char buffer[10];
            strcpy(buffer, (message[i].message) );
            len = strlen(message[i].message);
            printf("%s\n",buffer);
            addr_len = sizeof(addr_send);
            bytes_sent=sendto(sock, buffer, len, 0,(struct sockaddr *) &addr_send, addr_len);
            if (bytes_sent < 0 )
                printf("\noops server did not recieve the %d th message\n",bytes_sent);

          }
          //if ( strcmp(message[3].message,"n")==0)
              //  exit(0);
          /* ------------------comment----------------------
          check if they want to continue
          going to change to the condition to
          if the user typed quit stop the program but for now
          lets use y/n
          */
            addr_len = sizeof(addr_send);
            recvfrom(sock,buf, sizeof(char),MSG_WAITALL,(struct sockaddr *)&addr_send, &addr_len);
            canPlay = atoi(buf);
            printf("\nclient2 recieved %d\n",canPlay);
            printf("\nwaiting on opponent to play..\n");
            //note it gets the signal to suspend the player but
        }
        //continue to check if the player is still suspended
        addr_len = sizeof(struct sockaddr_in);
        while ( recvfrom(sock,buf, sizeof(char),MSG_WAITALL,(struct sockaddr *)&addr_send, &addr_len) != 1);
        //it enbables it here again to play why?
        canPlay = atoi(buf);
        //printf("\nClient 2 recieved %d\n",canPlay);
        if (canPlay == 2)
        {
          printf("\nPlayer 1 quit the game.\n");
          exit(0);
        }

    }
/*
    strcpy (message[0].message,"0");
    strcpy (message[1].message,"0");
    strcpy (message[2].message,"@");
    strcpy (message[3].message,"n");

    for (int i = 0 ; i < 4 ; i++)
    {
      addr_len = sizeof(addr_send);
      bytes_sent=sendto(sock, message[i].message, len, 0,(struct sockaddr *) &addr_send, addr_len);
    }
   //close commmunication
    i = close (sock);
    if (i < 0 )
        printf("close() failed\n");*/
    return 0;
}

void getPlayerMove()
{
  char play[MAX_LEN];
  int coordinates;
  int x_cord,y_cord;
  char yes[] = "y";
  char no[] = "n";
  printf("\nEnter a letter from the english alphabet\nor 'quit' to end game: ");
  scanf("%s",play);

  if(strcmp(play,"quit")== 0)
    {
      //need to let server know that
      //the player quit
      memset(message[3].message,'\0',sizeof(no));
      sprintf (message[3].message,"%s",no);
      quitGame();
    }
    else
    {
      printf("\nEnter xy coordinates: ");
      scanf("%d",&coordinates);

      memset(message[3].message,'\0',sizeof(yes));
      sprintf (message[3].message,"%s",yes);
      //printf("\n%s\n",play);
      memset(message[2].message,'\0',sizeof(play));
      sprintf (message[2].message,"%s",play);
      //sprintf(message.message[3],"%c",tmp);
      x_cord = coordinates / 10;
      y_cord = coordinates % 10;

      memset(message[0].message,'\0',sizeof(x_cord));
      memset(message[1].message,'\0',sizeof(y_cord));

      sprintf(message[0].message,"%d",x_cord);
      sprintf(message[1].message,"%d",y_cord);
    }
}
void quitGame()
{
  strcpy (message[0].message,"0");
  strcpy (message[1].message,"0");
  strcpy (message[2].message,"@");
  strcpy (message[3].message,"n");

  for (int i = 0 ; i < 4 ; i++)
  {
    addr_len = sizeof(addr_send);
    bytes_sent=sendto(sock, message[i].message, len, 0,(struct sockaddr *) &addr_send, addr_len);
  }
 //close commmunication

  if ( close (sock) < 0 )
      printf("close() failed\n");

  exit(0);
}

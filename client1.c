#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>//system type definitions
#include <sys/socket.h>//network system functions
#include <netinet/in.h>// protocol & struct definition
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
int canPlay = 1 ; //intialized to true
#define SERVER_IP	"127.0.0.1"
#define MAX_LEN 4
int sock;
struct msg
{
  char message[10];
};
void getPlayerMove();

struct msg message[4];
void quitGame();

int i;

struct sockaddr_in  addr_send;
unsigned short  server_port = 60000;
struct mmsghdr send_msg;
//char x_cordinate[1],  y_cordinate[1], letter[1], still_playing[1];

int bytes_sent;
int len,incoming_len, addr_len;
char buf[10];
char waiting[10];

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

    // this one response  should be sent yo the server
    printf("\nWant to play (y/n)? ");
    scanf("%s",(message[3].message));

    if (strcmp(message[3].message,"n")!=0)
    {
      //need to set the size before the fucntion call else the server
      //will recieve 0.0.0.0 or 127.0.0.1
      addr_len = sizeof(addr_send);
      len = strlen(message[3].message);
      sendto(sock,message[3].message,len, 0,(struct sockaddr *) &addr_send, addr_len);
    }
    else
      exit(0);

      //need to wait on next player
      strcpy(waiting,"0");
      printf("\nwaiting on opponent..\n");
      //printf("\nEntered while and waiting %s\n",waiting);
      len = strlen(waiting);
      //wait on response from server that player 2 connected
      while ( recvfrom(sock,waiting,len,0,(struct sockaddr *)&addr_send,&incoming_len) != 1 );
      //printf("\nsomething? %s\n",waiting);

    //printf("\n%s\n","Ready to play...");
    for(;;)//loop forever
    {
        /*-------comment-----------
         each loop should check if the player
         canplay or is still waiting on the opponent
         this message should be sent by the server
        */

        if (canPlay!=0)
        {

          getPlayerMove();

          // send bytes
          for(int i=0; i<4; i++)
          {
            char buffer[10];
            strcpy(buffer, (message[i].message) );
            len = strlen(message[i].message);
            //printf("%s\n",buffer);
            addr_len = sizeof(addr_send);
            bytes_sent=sendto(sock, buffer, len, 0,(struct sockaddr *) &addr_send, addr_len);
          }
          /* ------------------comment----------------------
          check if they want to continue
          going to change to the condition to
          if the user typed quit stop the program but for now
          lets use y/n
          */

          // suspend player
            addr_len = sizeof(addr_send);
            while ( recvfrom(sock,buf, sizeof(char),MSG_WAITALL,(struct sockaddr *)&addr_send,&addr_len) != 1);
            canPlay = atoi(buf);

            printf("\nwaiting on opponent to play..\n");
        }

        //continue to check if the player is still suspended
        addr_len = sizeof(struct sockaddr_in);
        while( recvfrom(sock,buf, sizeof(char),MSG_WAITALL,(struct sockaddr *)&addr_send, &addr_len) != 1);
        canPlay = atoi(buf);
        //printf("\n%d\n",canPlay);
        if (canPlay == 2)
        {
          printf("\nPlayer 2 quit the game.\n");
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
    }*/
   //close commmunication
  //  i = close (sock);
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
      memset(message[3].message,'\0',sizeof(no));
      sprintf (message[3].message,"%s",no);

      //need to let server know that
      //the player quit
      quitGame();

    }
    else
    {
      printf("\nEnter xy coordinates: ");
      scanf("%d",&coordinates);
      //yes the player is still playing cause they didnt QUIT
      memset(message[3].message,'\0',sizeof(yes));
      sprintf (message[3].message,"%s", yes);
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
      //printf("\n%s %s %s %s\n",message[0].message,message[1].message,message[2].message,message[3].message);
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
  if ( close (sock) < 0 )
      printf("close() failed\n");

  exit(0);

}

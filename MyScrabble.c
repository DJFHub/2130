#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>//network system functions
#include <netinet/in.h>// protocol & struct definition
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

//notes going to change and use the UDP cause the TCP is not working

#define BUF_SIZE	1

#define LISTEN_PORT	60000

void getNewBoard();
void drawBoard();
void makePlay(int x, int y, char* c);
int SCRABBLE_LETTER_VALUES(char letterValue);
void udpServer();
void startBoard();
// returns a character from the alphabet
//used as a helper function with startBoard
char *getAsciiVal(int value);

int isOnBoard(int x , int y);

struct mesg
{
  char *message[4];
};
/*
* Services the server should provide:
*   Draw the board which would include placing new moves
*   on the board
*   manage the playing, giving each client a turn
*   or allowing them to quit
*
*/
int k,j;
#define NUM_RANGE 8
//global declaration structure grid
char * grid[NUM_RANGE][NUM_RANGE];
int running = 1 ;
char* letter;

int main()
{

//calling the functions
    udpServer();

    //making two plays
    //makePlay(3,7,"a");
    //makePlay(4,5,"b");

    //redrawing the board with plays shown
    //drawBoard();

    return 0;
}

void getNewBoard(){
        //creates a brand new blank board. Returns a pointer to the array
        for (j = 0; j < NUM_RANGE; j++){
            for (k = 0; k < NUM_RANGE; k++)
            {
                grid[k][j]="   ";
            }//Endfor
        }//Endfor
        return;
    }//EndFunction getNewBoard

void drawBoard(){
    //This function prints out the board that was passed. Returns void
    char * const NLINE = "    1    2    3    4    5    6    7    8";
    char * const HLINE = "  +----+----+----+----+----+----+----+----+";
    char * const VLINE = "  |    |    |    |    |    |    |    |    |";

    printf("%s\n",NLINE);
    printf("%s\n",HLINE);
    for (j = 0; j < NUM_RANGE; j++)
    {
        printf("%s\n",VLINE);
        printf("%d ",j+1);
        for (k = 0; k < NUM_RANGE; k++)
        {  if(strcmp(grid[k][j],"   ")==0){
                printf("| %s",grid[k][j]);
            }else{
                printf("| %s  ",grid[k][j]);
            }
        }
        printf("%s","|");
        printf("\n");
        printf("%s\n",VLINE);
        printf("%s\n",HLINE);
    }
    return;
}//EndFunction drawBoard


void makePlay(int x, int y, char* c){
   //Arrays are zero indexed but our grid starts at index 1
   grid[x-1][y-1]=c;
   return;
}

int SCRABBLE_LETTER_VALUES(char letterValue)
{
    int value;
    switch(letterValue)
    {
        case 'A':
        value = 1;
        break;
        case 'B':
        value = 3;
        break;
        case 'C':
        value = 3;
        break;
        case 'D':
        value = 2;
        break;
        case 'E':
        value = 1;
        break;
        case 'F':
        value = 4;
        break;
        case 'G':
        value = 2;
        break;
        case 'H':
        value = 4;
        break;
        case 'I':
        value = 1;
        break;
        case 'J':
        value = 8;
        break;
        case 'K':
        value = 5;
        break;
        case 'L':
        value = 1;
        break;
        case 'M':
        value = 3;
        break;
        case 'N':
        value = 1;
        break;
        case 'O':
        value = 1;
        break;
        case 'P':
        value = 3;
        break;
        case 'Q':
        value = 10;
        break;
        case 'R':
        value = 1;
        break;
        case 'S':
        value = 1;
        break;
        case 'T':
        value = 1;
        break;
        case 'U':
        value = 1;
        break;
        case 'V':
        value = 4;
        break;
        case 'W':
        value = 4;
        break;
        case 'X':
        value = 8;
        break;
        case 'Y':
        value = 4;
        break;
        case 'Z':
        value = 10;
        break;
    }
    return value;
}
// UDP Server Functions
void udpServer()
{
  int			sock_recv;
  struct sockaddr_in	my_addr;
  int			i;
  fd_set	readfds,active_fd_set,read_fd_set;
  //struct timeval		timeout={0,0};
  int	incoming_len;
  struct sockaddr_in	remote_addr, client1_addr , client2_addr;
  int			recv_msg_size;
  char			buf[BUF_SIZE];
  int			select_ret;
  struct mesg messages;
  getNewBoard();
  //randomly generate letters on board
  startBoard();
  //Displaying empty board
  printf("\n%s\n\n","Printing an empty board....");
  drawBoard();

  sock_recv=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock_recv < 0){
      printf("socket() failed\n");
      exit(0);
  }
  memset(&my_addr, 0, sizeof (my_addr));	/* zero out structure */
  my_addr.sin_family = AF_INET;	/* address family */
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* current machine IP */
  my_addr.sin_port = htons((unsigned short)LISTEN_PORT);
      /* bind socket to the local address */
  i=bind(sock_recv, (struct sockaddr *) &my_addr, sizeof (my_addr));
  if (i < 0){
      printf("bind() failed\n");
      exit(0);
  }
     FD_ZERO(&readfds);		/* zero out socket set */
     FD_SET(sock_recv,&readfds);	/* add socket to listen to */

      /* listen ... */
  while (running)
  {
      read_fd_set = active_fd_set;
      select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);

      if (select_ret > 0)
      {


          for(int i = 0 ; i < 4; i++)
          {
                incoming_len=sizeof(remote_addr);
                recv_msg_size=recvfrom(sock_recv,buf,sizeof(char),MSG_WAITALL,(struct sockaddr *)&remote_addr,&incoming_len);

                if (recv_msg_size > 0)
                {
                  printf("\nbuf val: %s ",buf);
                  memset(buf,'\0',strlen(buf)-1);
                  //buf[recv_msg_size]='\0';
                  messages.message[i] = strdup(buf);
                  //strup() is used instead of strcpy, returns a pointer
                  // to the string, used to avoid memory leaks
                  //printf("\n%s %d ",messages.message[i],i);
                }
          }

            if (strcmp(messages.message[3],"n")!=0)
            {

                // make play x y letter
                int x  = atoi(messages.message[0]);
                int y  = atoi(messages.message[1]);

                //char letter = messages[2].message[0];
                //printf("\n%d %d %s",x,y,messages.message[2]);
                /*------------------comment------------------------
                  Should check if the x and y coordinates to valid before
                  making the play
                */
                if (isOnBoard(x,y))
                {
                  makePlay(x,y,messages.message[2]);
                  printf("\n%s\n\n","RePrinting board after plays....");
                  //redrawing the board with plays shown
                  drawBoard();
                  //should send a message to the client letting them
                  //know the play was valid
                }
                /*--------------------comment-------------------------
                  else  send some message to back to the client letting them
                  know that the play was invalid

                */
              }
              else
                running = 0; //no longer running

        }
  }

  close(sock_recv);
}
void startBoard()
{
  //This function uses the board to randomly position 10 letters on the
  //boardt
  time_t t;
  int x,y;//coordinates
  int ascii; //ascii value
  char *c ;
  int maxAscii = 122;
  int minAscii = 97;
  //the board range
  int minPos = 1;
  int maxPos = 8;
  for (int i = 0 ; i < 10 ; i++)
  {
    //maximumvalue + rand() / (RAND_MAX / (minimumValue - maximumvalue + 1) + 1)
    x = maxPos+rand()/(RAND_MAX / (minPos - maxPos + 1) + 1);
    y = maxPos+rand()/(RAND_MAX / (minPos - maxPos + 1) + 1);

    ascii = maxAscii + rand()/(RAND_MAX/ (minAscii- maxAscii + 1)+1);
    c = getAsciiVal(ascii);
    makePlay(x,y,c);
  }

}
char *getAsciiVal(int value)
{
  char* c ;

  switch(value)
  {
    case 97:
    c = "a";
    break;
    case 98:
    c = "b";
    break;
    case 99:
    c = "c";
    break;
    case 100:
    c = "d";
    break;
    case 101:
    c = "e";
    break;
    case 102:
    c = "f";
    break;
    case 103:
    c = "g";
    break;
    case 104:
    c = "h";
    break;
    case 105:
    c = "i";
    break;
    case 106:
    c = "j";
    break;
    case 107:
    c = "k";
    break;
    case 108:
    c = "l";
    break;
    case 109:
    c = "n";
    break;
    case 110:
    c = "m";
    break;
    case 111:
    c = "o";
    break;
    case 112:
    c = "p";
    break;
    case 113:
    c = "q";
    break;
    case 114:
    c = "r";
    break;
    case 115:
    c = "s";
    break;
    case 116:
    c = "t";
    break;
    case 117:
    c = "u";
    break;
    case 118:
    c = "v";
    break;
    case 119:
    c = "w";
    break;
    case 120:
    c = "x";
    break;
    case 121:
    c = "y";
    break;
    case 122:
    c = "z";
    break;
  }

return c;
}
int isOnBoard(int x , int y)
{
  int booleanValue = 1;// 0 is false and 1 is true

  if ( (x < 1 || x > 8) || ( y < 1 || y > 8 ) )
  {
    booleanValue  = 0;
  }
  return booleanValue;
}

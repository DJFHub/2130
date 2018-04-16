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
#include <errno.h>
#include <arpa/inet.h>

//notes going to change and use the UDP cause the TCP is not working

#define BUF_SIZE	1
#define LISTEN_PORT	60000

int			sock_recv;
struct  sockaddr_in	my_addr;
fd_set	readfds,active_fd_set,read_fd_set;
//struct timeval		timeout={0,0};
int	    incoming_len;
int			recv_msg_size;
char		buf[BUF_SIZE];
int			select_ret;
struct  mesg messages;
pid_t   parentProcess = -1;

int isPlayerOne = 1;//true
int isPlayerTwo = 0;//false

int PlayerOneScore = 0;
int PlayerTwoScore = 0;

void getNewBoard();
void drawBoard();
void makePlay(int x, int y, char* c);
int SCRABBLE_LETTER_VALUES(char letterValue);
void udpServer();
void startBoard();
// returns a character from the alphabet
//used as a helper function with startBoard
void runningServer(struct sockaddr_in *client,struct sockaddr_in *opponent );

char *getAsciiVal(int value);

int isOnBoard(int x , int y);

#define WORDCOUNT 83667
int foundWords = 0 ;
char* isWord(int x , int y);
void makeDictionary();
void dictionaryLookUp(char * wordOnBoard );
void upperCase(char *wordOnBoard);
void reverseWordCheck(char *wordOnBoard);
int calculateScore(char *word);

struct Dictionary
{

    char word[40];

} dictionary[WORDCOUNT],wordsFound[100];

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
  //parentProcess = getpid();
//calling the functions
    makeDictionary();
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
  int			i, client_addrlen;
  struct sockaddr_in	client, client1_addr , client2_addr;
  int addr_len;
//  int recvMessageSize;
  getNewBoard();
  //randomly generate letters on board
  startBoard();
  //Displaying empty board
  printf("\n%s\n\n","Printing an empty board....");
  drawBoard();

  sock_recv = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (sock_recv < 0){
      printf("\n%s\n","socket() failed");
      exit(0);
  }
  memset(&my_addr, 0, sizeof (my_addr));	/* zero out structure */
  my_addr.sin_family = AF_INET;	/* address family */
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* current machine IP */
  my_addr.sin_port = htons((unsigned short)LISTEN_PORT);

  addr_len = sizeof(my_addr);
      /* bind socket to the local address */
  i=bind(sock_recv, (struct sockaddr *) &my_addr, addr_len);

  if (i < 0){
      printf("\n%s\n","bind() failed");
      exit(0);
  }
     FD_ZERO(&readfds);		/* zero out socket set */
     FD_SET(sock_recv,&readfds);	/* add socket to listen to */
     read_fd_set = active_fd_set;
     //select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);
      /* listen ... */

    printf("\n%s\n","Waiting on players..");
    //need to be intialized prior or you will get garbage
    incoming_len = sizeof(struct sockaddr_in);
    if ( recvfrom(sock_recv,buf,sizeof(char),MSG_WAITALL,(struct sockaddr *)&client,&incoming_len) < 0)
        printf("\n recvFrom() failed to recieve client1");

        printf("Handling client1 %s\n", inet_ntoa(client.sin_addr));
        memset(&client1_addr, 0, sizeof(client1_addr));  //clear structure
        client1_addr.sin_family = AF_INET; //family address
        client1_addr.sin_addr.s_addr = client.sin_addr.s_addr;
        client1_addr.sin_port = client.sin_port;

    //did player one want to play?
    if (strcmp("y",buf)==0)
        printf("\n%s\n","Player 1 is ready..");
    else
      {
        printf("\nPlayer1 disconnected. Player 2 wins");
        exit(0);
      }
    incoming_len = sizeof(struct sockaddr_in);
    if (recvfrom(sock_recv,buf,sizeof(char),MSG_WAITALL,(struct sockaddr *)&client2_addr,&incoming_len) < 0 )
        printf("\n recvFrom() failed to recieve client2");

     printf("\nsock: %d\n",sock_recv);

    if (strcmp("y",buf)==0)
    {

        printf("\n%s\n","Player 2 is ready..");
        strcpy(buf,"1");

        incoming_len = strlen(buf);
        client_addrlen = sizeof(client1_addr);

        printf("Handling client1 %s\n", inet_ntoa(client1_addr.sin_addr));
        printf("Handling client2 %s\n", inet_ntoa(client2_addr.sin_addr));

        errno = 0 ;
        while ( sendto(sock_recv, buf, incoming_len,0,(struct sockaddr *)&client1_addr, client_addrlen) != 1);
          //  printf("\n%s\n\n",strerror(errno));
    }
    else
    {
          printf("\nPlayer 2 disconnected. Player 1 wins");
          exit(0);
    }
  /*------------------------------------comment----------------------------------------
    Description of the intended commmunication process between clients and server
    1.Player one is always allowed to play first
    2.player 2 should be in suspended state when player one is playing

    note 1 & 2 are the intial state of the p1 and p2
    so we see that canPlay is false for in client2.c and true in client1.c

  */
//fork();
//if (parentProcess == getpid())

  runningServer(&client1_addr , &client2_addr);


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
  srand((unsigned) time(&t));

  for (int i = 0 ; i < 10 ; i++)
  {
    //maximumvalue + rand() / (RAND_MAX / (minimumValue - maximumvalue + 1) + 1)
    x = maxPos+rand()/(RAND_MAX / (minPos - maxPos + 1) + 1);
    y = maxPos+rand()/(RAND_MAX / (minPos - maxPos + 1) + 1);
    ascii = maxAscii + rand()/(RAND_MAX/ (minAscii- maxAscii + 1)+1);

    if (isOnBoard(x,y)!=1)
    {
      --i;
      continue;
    }
    else if (ascii > maxAscii || ascii < minAscii)
    {
      --i;
      continue;
    }
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

void runningServer(struct sockaddr_in *player1,struct sockaddr_in *player2 )
{
  int addr_len;


  while (running)
  {

      select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);

      if (select_ret > 0)
      {

          for(int i = 0 ; i < 4; i++)
          {
                incoming_len=sizeof(struct sockaddr_in);
                if (isPlayerOne==1)
                recv_msg_size=recvfrom(sock_recv,buf,sizeof(char),MSG_WAITALL,(struct sockaddr *)player1,&incoming_len);
                else if (isPlayerTwo == 1)
                  recv_msg_size=recvfrom(sock_recv,buf,sizeof(char),MSG_WAITALL,(struct sockaddr *)player2,&incoming_len);



                //printf("\nsize: %d ",recv_msg_size);
                if (recv_msg_size > 0)
                {
                  //printf("\nbuf val: %s ",buf);
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


                  //isPlayerOne playing ?
                  if (isPlayerOne == 1)
                  {
                    isWord(x,y);

                    isPlayerOne = 0;
                    strcpy(buf,"0");
                    addr_len = sizeof((*player1));
                    //suspended the player account until the next player has played
                    sendto(sock_recv, buf, strlen(buf)+1,0,(struct sockaddr *)player1,addr_len);

                    addr_len = sizeof((*player2));

                    //need to enable the next player to play
                    strcpy(buf,"1");
                    sendto(sock_recv, buf, strlen(buf)+1,0,(struct sockaddr *)player2, addr_len);
                    isPlayerTwo = 1;
                  }
                  //isPlayerTwo playing ?
                  else if(isPlayerTwo == 1)
                  {
                    isWord(x,y);
                    isPlayerTwo = 0;
                    strcpy(buf,"0");
                    addr_len = sizeof((*player2));
                    //susupend player 2
                    sendto(sock_recv, buf, strlen(buf)+1,0,(struct sockaddr *)player2, addr_len);

                    //allow player1 to play again
                    isPlayerOne = 1;
                    strcpy(buf,"1");
                    addr_len = sizeof((*player1));
                    //enable
                    sendto(sock_recv, buf, strlen(buf)+1,0,(struct sockaddr *)player1,addr_len);
                  }

                }
                /*--------------------comment-------------------------
                  else  send some message to back to the client letting them
                  know that the play was invalid

                */
              }
              else
              {
                running = 0; //no longer running
                //who wanted to quit ?
                if (isPlayerOne == 1)
                {
                  //tell player 2 that the game is over
                  //ok so when a 2 is sent the player will
                  // that player 1 quit
                  printf("\nPlayer 1 quit the game\n");
                  strcpy(buf,"2");
                  addr_len = sizeof((*player2));
                  //susupend player 2
                  sendto(sock_recv, buf, strlen(buf)+1,0,(struct sockaddr *)player2, addr_len);


                }
                else if (isPlayerTwo == 1)
                {
                  printf("\nPlayer 2 quit the game\n");
                  strcpy(buf,"2");
                  addr_len = sizeof((*player1));
                  //enable
                  sendto(sock_recv, buf, strlen(buf)+1,0,(struct sockaddr *)player1,addr_len);
                }
              }


        }
  }
  close(sock_recv);
}

char* isWord(int x , int y)
{
       x = x - 1;
       y = y - 1;
      char newWord[8] = "";
      //intialize new word
      memset(newWord,'\0',sizeof(newWord));
      //traverse left , empty grids have "   " as there value
      for (int i = x; i >= 0 ; i-- )
      {
         if (grid[i][y]!="   ")
         {
           strcat(newWord,grid[i][y]);
         }
      }
      upperCase(newWord);
      dictionaryLookUp(newWord);
      reverseWordCheck(&newWord);
      //printf("\nWord: %s\n",newWord);
      strcpy(newWord,"");




      //traverse right
      for (int i = x; i < 8 ; i++ )
      {
         if (grid[i][y]!="   ")
         {
           strcat(newWord,grid[i][y]);
         }
      }
      upperCase(&newWord);
      reverseWordCheck(&newWord);
      //printf("\nWord: %s\n",newWord);
      dictionaryLookUp(&newWord);
      strcpy(newWord,"");

      //traverse up
      for (int i = y; i >= 0 ; i-- )
      {
         if (grid[x][i]!="   ")
         {
           strcat(newWord,grid[x][i]);
         }
      }
      upperCase(&newWord);
      reverseWordCheck(&newWord);
      //printf("\nWord: %s\n",newWord);
      dictionaryLookUp(&newWord);
      strcpy(newWord,"");
      //traverse down
      for (int i = y; i < 8 ; i++ )
      {
         if (grid[x][i]!="   ")
         {
           strcat(newWord,grid[x][i]);
         }
      }
      upperCase(&newWord);
      reverseWordCheck(&newWord);
      //printf("\nWord: %s\n",newWord);
      dictionaryLookUp(&newWord);
      strcpy(newWord,"");

      return newWord;
}

void makeDictionary()
{
  FILE *file_pointer ; // points to the file
  char *buffer ; //will contain the address of the first character
  size_t wordSize  = 40; // the amount of space allocated for each word (40 characters/bytes)
  char input[40];

  // allocate space for the word in the dictionary
  buffer = (char *)malloc(wordSize * sizeof(char));

  file_pointer = fopen("words.txt","rb");

  //read up what the getline function does
  for( int i  = 0 ;  getline(&buffer,&wordSize,file_pointer) != -1; i++)
  {
      // using (strlen(buffer)-1) because getline retains \n in the string
      strncpy(dictionary[i].word,buffer, (strlen(buffer)-1));

  }
  fclose(file_pointer); //linux file is closed

}
void dictionaryLookUp(char * wordOnBoard )
{
  for (int i = 0 ; i < WORDCOUNT; i++)
  {
    if ( strcmp(dictionary[i].word, wordOnBoard) == 0)
        {
          strcpy(wordsFound[foundWords].word,wordOnBoard);
          foundWords++;
          if (isPlayerOne == 1)
          {
            PlayerOneScore = calculateScore(wordOnBoard);
            printf("\nPlayer One Score Updated: %d\n",PlayerOneScore);
          }
          else if (isPlayerTwo==1)
          {
            PlayerTwoScore = calculateScore(wordOnBoard);
            printf("\nPlayer Two Score Updated: %d\n",PlayerTwoScore);
          }
        }
  }
}
void reverseWordCheck(char *wordOnBoard)
{
   char word[40];
   memset(word,'\0',sizeof(word));
   for (int i = (strlen(wordOnBoard)-1), g = 0 ; i >= 0 ; i--,g++)
   {
     word[g] = wordOnBoard[i];
   }
   dictionaryLookUp(word);
}
void upperCase(char *wordOnBoard)
{
  for (int i = 0; i < strlen(wordOnBoard); i++)
  {
    wordOnBoard[i] = toupper(wordOnBoard[i]);
  }
}

int calculateScore(char *word)
{
  char *ptr;
  ptr = word;
  int score = 0;
  int i =0;

  //Iterates through word and adds the value of each character to score
  for(i=0; word[i]!= '\0';i++)
  {
   score += SCRABBLE_LETTER_VALUES(word[i]);
   ptr++;
  }
  //printf("Word values %d points!\n",score);

  return score;
}

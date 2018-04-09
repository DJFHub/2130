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

#define BUF_SIZE	2

void getNewBoard();
void drawBoard();
void makePlay(int x, int y, char* c);
int SCRABBLE_LETTER_VALUES(char letterValue);
void tcpServer();
int createTCPServerSocket(unsigned short port);
void  handleTCPClient(int client_socket);
int acceptTCPConnection(int server_socket);

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

int main()
{

//calling the functions
    tcpServer();

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
// TCP Server Functions
void tcpServer()
{
    int *serverSock; //aka the descriptor for the sock
    int maxDesc;  //max socket descriptor value
    fd_set sockSet; // add a descriptor to the vector table
    int const noPort = 2;
    int c1_port = 60000, c2_port = 60001;

    getNewBoard();
    //Displaying empty board
    printf("\n%s\n\n","Printing an empty board....");
    drawBoard();

    //allocate memory for the number sockets for the incoming connections
    serverSock = (int *) malloc( noPort * sizeof(int));

    // creating two server sockets so that the clients can connect
    // to one each
    serverSock[0] = createTCPServerSocket(c1_port);
    maxDesc = serverSock[0];
    serverSock[1] = createTCPServerSocket(c2_port);
    if (maxDesc < serverSock[1])
        maxDesc = serverSock[1];

    while(running)
    {
        FD_ZERO(&sockSet);
        FD_SET(serverSock[0], &sockSet);//descriptor to the vector
        FD_SET(serverSock[1], &sockSet);

        if (select(maxDesc+1,&sockSet,NULL,NULL,NULL) < 0)
            printf("\nselect() in tcpServer failed\n");
        else
        {
            //FD_ISSET test whether the fd is apart of the set
            for (int i ;  i < 2 ; i++)
            {
                if (FD_ISSET(serverSock[i], &sockSet))
                {
                    handleTCPClient(acceptTCPConnection(serverSock[i]));
                    if (running)
                    {
                        printf("\n%s\n\n","RePrinting board after plays....");
                        drawBoard();
                    }
                }
            }

        }

    }
    close(serverSock[0]);
    close(serverSock[1]);

    free(serverSock);

    exit(0);

}
int createTCPServerSocket(unsigned short port)
{
    int sock;
    struct sockaddr_in serv_addr; //hold the local addresss

    //create the socket for the incoming connections
    if ( (sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
        printf("\nsocket() failed\n");

    memset(&serv_addr, 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //*
    serv_addr.sin_port = htons(port);

    //bind the local addresss
    if (bind(sock, (struct sockaddr *)&serv_addr,sizeof(serv_addr)))
        printf("\nbind() in createTCPServerSocket failed\n");

    if ( listen(sock, 5) < 0)
        printf("\nlisten() in createTCPServerSocket failed\n");

    return sock;
}

void  handleTCPClient(int client_socket)
{
    char  x_cordinate, y_cordinate
    , letter, playing;
    int  recvMsgSize;
    struct sockaddr_in	remote_addr;
    int len  = sizeof(remote_addr);
    //recieve message from client
     recvMsgSize = recvfrom(client_socket,x_cordinate,sizeof(char ),MSG_WAITALL,(struct sockaddr *)&remote_addr,&len);
     if (recvMsgSize > 0)
        //x_cordinate[recvMsgSize] = '\0';

     recvMsgSize=recvfrom(client_socket,y_cordinate,sizeof(char ),MSG_WAITALL,(struct sockaddr *)&remote_addr,&len);
     if (recvMsgSize > 0)
        //y_cordinate[recvMsgSize] = '\0';
     recvMsgSize=recvfrom(client_socket,letter,sizeof(char ),MSG_WAITALL,(struct sockaddr *)&remote_addr,&len);
     if (recvMsgSize > 0)
        //letter[recvMsgSize] = '\0';
     recvMsgSize=recvfrom(client_socket,playing,sizeof(char ),MSG_WAITALL,(struct sockaddr *)&remote_addr,&len);
     if (recvMsgSize > 0)
      //playing[recvMsgSize] = '\0';

      printf("%c %c %c %c",x_cordinate,y_cordinate,letter,playing);
    //we need to make the play
    if (playing !='n')
     makePlay( (x_cordinate - '0'), (y_cordinate - '0'),letter);
    else
        running = 0 ;

     close(client_socket);

}

int acceptTCPConnection(int server_socket)
{

    int client_socket;
    struct sockaddr_in clnt_addr ;
    unsigned int len;

    //size of I/O parameter
    len = sizeof(clnt_addr);

    if ((client_socket = accept(server_socket,(struct sockaddr *)&clnt_addr,&len))< 0)
        printf("\naccept() in acceptTCPConnection failed\n");

    //now the client socket is connected
    //printf("Handling client %s\n", inet_ntoa(clnt_addr.sin_addr)) ;
    return client_socket;
}

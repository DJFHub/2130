#include<stdlib.h>
#include<string.h>
#include<stdio.h>

void endGame()
{ 
  char play;
  
  printf("Are you sure you want to end this game,Y/n?");
  scanf("%s\n",&play);
  //gets(play);
  
  
  switch(play)
  {
    case 'Y' :
               calculateScore();
               printf("Player  : Final Score\n");
               printf("  1     :      %d\n",score);
               printf("  2     :      %d\n",score);
               
               if(p1 > p2)
                  printf("Player 1 Won!\n");
                  
               if(p2 > p1)
                  printf("Player 2 Won!\n");
                  
               else
                 printf("Player 1 & Player 2 are Tied!\n");   
               break;  
                
    case 'n' :
               getPlayerMove();
               break;
               
    default  :
               printf("Invalid Symbol!\n");            
  }
  printf("Game Over !\n");
}

int main()
{
  endGame();
}

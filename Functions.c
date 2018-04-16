#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define MAX_LEN 4

/*Comments with Test, are simply there for the sole purpose of testing.*/
/*and should be removed once we put the final project together*/
/*¬Jem*/
struct msg
{
  char message[10];
};

/*void getPlayerMove()
{
  char play[MAX_LEN];
  int coordinates;
  int x_cord,y_cord;
  struct msg message[4];
  char tmp;
  printf("\nEnter a letter from the english alphabet\nor 'quit' to end game: ");
  scanf("%s",play);

  printf("\nEnter xy coordinates: ");
  scanf("%d",&coordinates);

  if(strcmp(play,"quit")== 0)
    {
      //need to let server know that
      //the player quit
    }
    else
    {
      printf("\n%s\n",play);
      memset(message[3].message,'\0',sizeof(play));
      sprintf (message[3].message,"%s",play);
      //sprintf(message.message[3],"%c",tmp);
      x_cord = coordinates / 10;
      y_cord = coordinates % 10;

      memset(message[0].message,'\0',sizeof(x_cord));
      memset(message[1].message,'\0',sizeof(y_cord));

      sprintf(message[0].message,"%d",x_cord);
      sprintf(message[1].message,"%d",y_cord);
    }
}*/

void getPlayerMove()
{
  char play[MAX_LEN];
  char letter;
  int coordinates;
  int new_cord,x_cord,y_cord;

  //Prompt User for letter & Coordinates or Quit to end Game
  printf("Enter your move in the format(letter,xcordycord) or 'quit' to end game\n");
  scanf(" %[^,\n],%d",play,&coordinates);
  //printf("%s\n",play);

  //Test1:Prints user Input...checks if the correct value is being read.
  //printf("%s,%d\n",play,coordinates);

  //Checks if user enters QUIT
  if(strcmp(play,"quit")== 0)
    printf("End Game\n");  //Test2: Checks if the if statement works
    //endGame();           //Will call end game function once it has been implemented.

  else
  {
    new_cord = coordinates;
  //printf("%d\n",new_cord); //Test3:Checks if the correct value was passed to new_cord.
    letter = *play;
    x_cord = new_cord / 10;
    y_cord = new_cord % 10;
    printf("%c(x,y):(%d,%d)\n",letter,x_cord,y_cord); //Test4:Checks if the correct value was passed to new_cord.

  }
return;
}

int main()
{
  getPlayerMove();

}

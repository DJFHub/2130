#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define MAX_LEN 4

/*Comments with Test, are simply there for the sole purpose of testing.*/ 
/*and should be removed once we put the final project together*/
/*¬Jem*/
void sepPlayerMove()
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
int getPlayerMove(char let,int x,int y)
{  
	char letter;
	int x_cord,y_cord;
	
	sepPlayerMove();
	
	let = letter;
	x   = x_cord;
	y   = y_cord;
	 
	
}  
int main()
{  
  getPlayerMove();
}

#include <stdio.h>
#include <string.h>
void getNewBoard();
void drawBoard();
void startBoard();
char* isWord(int x , int y);
void makeDictionary();
int k,j;
#define NUM_RANGE 8
#define WORDCOUNT 83667
//global declaration structure grid
char * grid[NUM_RANGE][NUM_RANGE];
int foundWords = 0 ;
void dictionaryLookUp(char * wordOnBoard );
void upperCase(char *wordOnBoard);
void reverseWordCheck(char *wordOnBoard);

struct Dictionary
{

    char word[40];

} dictionary[WORDCOUNT],wordsFound[100];

int main(){
    makeDictionary();
    getNewBoard();
    //Displaying empty board
    printf("\n%s\n\n","Printing an empty board....");
    drawBoard();
    //making two plays
    //makePlay(1,3,"d");
    makePlay(3,2,"o");
    makePlay(3,3,"o");
    makePlay(1,4,"d");
    makePlay(1,5,"r");
    makePlay(1,6,"a");
    makePlay(1,7,"i");
    makePlay(1,8,"n");

    makePlay(3,1,"d");
    makePlay(2,4,"o");
    makePlay(2,5,"a");
    makePlay(3,4,"g");
    makePlay(3,5,"i");
    makePlay(4,5,"n");

    printf("\n%s\n\n","RePrinting board after plays....");
    //redrawing the board with plays shown
    drawBoard();

    isWord(3,4);

    return 0;
}
void getNewBoard()
  {
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
void makePlay(int x, int y, char* c)
{
//Arrays are zero indexed but our grid starts at index 1
    grid[x-1][y-1]=c;
       return;
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
      printf("\nWord: %s\n",newWord);
      strcpy(newWord,"");

      dictionaryLookUp(newWord);



      //traverse right
      for (int i = x; i < 8 ; i++ )
      {
         if (grid[i][y]!="   ")
         {
           strcat(newWord,grid[i][y]);
         }
      }
      upperCase(&newWord);
      printf("\nWord: %s\n",newWord);
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
      printf("\nWord: %s\n",newWord);
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
      printf("\nWord: %s\n",newWord);
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

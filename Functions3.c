#include<stdlib.h>
#include<string.h>
#include<stdio.h>


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

int calculateScore(char *word)
{
  char *ptr;
  ptr = word;
  int score = 0;
  int total_score = 0;
  int i =0;

  //Iterates through word and adds the value of each character to score
  for(i=0; word[i]!= '\0';i++)
  {
   score += SCRABBLE_LETTER_VALUES(word[i]);
   ptr++;
  }
<<<<<<< HEAD
  //printf("Word values %d points!\n",score);

  return score;
}

=======
   printf("Word values %d points!\n",score); 
   total_score += score;
  // printf("FinalScore: %d\n",total_score);
  return 0;
 }	
>>>>>>> e05afcff3c1fdb9e2a18982ac19879e118a3db31
int calculateTotalScore(char *word)
{

}
int main()
{
  char ch[] = "AMERICAN";   //Test1: random string to test if code works
  calculateScore(ch);
}

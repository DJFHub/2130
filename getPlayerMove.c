//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int getCoordinates(int xy);
//int getCoordinates(int x,int y);
int main()
{
  char play;
  //int x,y ;
  int xy;
 
  printf("Player,Enter a letter:\n");
  scanf("%s",&play); 
  
  switch(play)
  {
    case 'A':
    case 'a':
              //getCoordinates(x,y);
              getCoordinates(xy);
              break;
    case 'B':
    case 'b':
              printf("02\n");
              break;
    case 'C':
    case 'c':
              printf("03\n");
              break;
    case 'D':
    case 'd':
              printf("04\n");
              break;
    case 'E':
    case 'e':
              printf("05\n");
              break;
    case 'F':
    case 'f':
              printf("06\n");
              break;
    case 'G':
    case 'g':
              printf("07\n");
              break;
    case 'H':
    case 'h':
              printf("08\n");
              break;
    case 'I':
    case 'i':
              printf("09\n");
              break;
    case 'J':
    case 'j':
              printf("10\n");
              break;
    case 'K':
    case 'k':
              printf("11\n");
              break;
    case 'L':
    case 'l':
              printf("12\n");
              break;
    case 'M':
    case 'm':
              printf("13\n");
              break;
    case 'N':
    case 'n':
              printf("14\n");
              break;
    case 'O':
    case 'o':
              printf("15\n");
              break;
    case 'P':
    case 'p':
              printf("16\n");
              break;
    case 'Q':
    case 'q':
              printf("17\n");
              break;
    case 'R':
    case 'r':
              printf("18\n");
              break;
    case 'S':
    case 's':
              printf("19\n");
              break;
    case 'T':
    case 't':
              printf("20\n");
              break;
    case 'U':
    case 'u':
              printf("21\n");
              break;
    case 'V':
    case 'v':
              printf("22\n");
              break;
    case 'W':
    case 'w':
              printf("23\n");
              break;
    case 'X':
    case 'x':
              printf("24\n");
              break;
    case 'Y':
    case 'y':
              printf("25\n");
              break;
    case 'Z':
    case 'z':
              printf("26\n");
              break;

   default  :
              printf("Invalid symbol!\n" );

  }
   
   return 0;
}

//Get coordinates
int getCoordinates(int xy)
//int getCoordinates(int x,int y)
{
  int x_cord,y_cord=0,xy_cord;

  printf("Enter letter's coordinates:\n");
  //scanf("%d,%d\n",&x,&y);
  scanf("%d\n",&xy);
  x_cord = xy % 10;
  xy = xy /10;
  
  y_cord = xy % 10;
  xy = xy /10;
  
  printf("%d,%d\n",x_cord,y_cord);
  
 /* x_cord = x;
  y_cord = y;

  printf("%d\n",x_cord);
  printf("%d\n",y_cord);
  return 0; 
  
  len=floor(log10(abs(xy))) + 1;
  x_cord = xy / pow(10, len / 2);
  y_cord = xy - x_cord * pow(10, len / 2);
  printf("x_cord=%d",x_cord);
  printf("\ny_cord= %d",y_cord);
  */
  return 0;
}



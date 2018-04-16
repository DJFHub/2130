#include <stdio.h>
#include <string.h>

int main()
{
	FILE *dpt;
	char dictionary[83667];
	int k =0;
	char word[30];
	
	dpt = fopen("words.txt", "r");
	while (fgets(dictionary,83668,dpt) != NULL)
	{
		//printf("%s\n",dictionary);
	}
	fclose(dpt);
	
	printf("achieved\n");
	
	printf("enter word to check\n");
	scanf("%s", &word);
	
	for(k =0; k < sizeof(dictionary); k++)
	{
		if (word == dictionary[k])
		{
			printf("word found\n");
		}
		else
		{
			printf("word not found\n");
		}
		
		
		k++;
	}
	
}
		

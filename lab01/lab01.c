#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab01.h"

// Function 1: Remove whitespace
void removewhitespace(char *s)
{
	int i =strlen(s) - 1;
	int j;
	while(i >= 0)
	{
		if(s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
		{
			j = i;
			for(; j < strlen(s); j++)
			{
				s[j] = s[j + 1];
			}
		}
		i--;
	}
}

// Function 2: Convert C to Pascal
char * c2pascal(char *s)
{
	int len = strlen(s);
	char *newstr = malloc((len + 1) * sizeof(char));
	newstr[0] = len;
	int i = 0;
	for(;i < len; i++)
	{
		newstr[i+1] = s[i];
	}
	return newstr;
}

// Function 3: Convert Pascal to C
char * pascal2c(char *s)
{
	int len = (int) s[0];
	char *newstr = malloc((len + 1) * sizeof(char));
	int i = 0;
	for(;i < len; i++)
	{
		newstr[i] = s[i + 1];
	}
	return newstr;
}

// Function 4: 
char ** tokenify(char *s)
{
	char **result = malloc((strlen(s)) * sizeof(char *));
	char *sep = " \n\t";
	char *word, *brkt;
	int i = 0;
	int j;
	for(word = strtok_r(s, sep, &brkt); word; word = strtok_r(NULL, sep, &brkt))
	{
		result[i] = malloc((strlen(word) + 1) * sizeof(char));
		j = 0;
		for(;j < strlen(word) + 1;j++)
		{
			result[i][j] = word[j];
		}
		i++;
	}
	result[i] = malloc(sizeof(char));
	result[i][0] = '/0';
	return result;
}

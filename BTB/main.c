// Brian Pecha - April 9, 2016

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


FILE *input;
char curAddr[] = "000000", prevAddr[] = "000000";
int  cur, prev;
//int sizes[3] = {256, 512, 1024};
char *table = "0123456789abcdef";


typedef struct btb
{
	int curPC;
	int predPC;
	int prediction;
	int bool;
} BTB;

int sm(int n, int taken)
{
	switch (n)
	{
	case 0: if (taken)
	{
		return 1;
	}
			else
			{
				return 0;
			}
	case 1: if (taken)
	{
		return 0;
	}
			else
			{
				return 2;
			}
	case 2: if (taken)
	{
		return 1;
	}
			else
			{
				return 3;
			}
	case 3: if (taken)
	{
		return 2;
	}
			else
			{
				return 3;
			}
	default: return 0;
	}
}

int myexp(int n)
{
	int i, result = 1;

	for (i = 0; i < n; i++)
	{
		result = result * 16;
	}

	return result;
}

void base16to10(char *s, int *n)
{
	int i, j;

	for (j = 0; j < 6; j++)
	{//printf("j=%d ",j);
		for (i = 0; i < 16; i++)
		{
			//printf("%d ", i);
			if (s[j] == table[i])
			{
				*n = *n + i * myexp(5 - j);
				break;
			}
		}
	}
}

void convert()
{
	cur = prev = 0;

	base16to10(curAddr, &cur);
	base16to10(prevAddr, &prev);
}

int readNew()
{
	if (!feof(input))
	{
		if (curAddr[0])
		{
			if (prevAddr[0])
				strcpy(prevAddr, curAddr);
			else
				strcat(prevAddr, curAddr);
			fscanf(input, " %6[^\n]", curAddr);
		}
		else // read first item
		{
			fscanf(input, " %6[^\n]", curAddr);
		}
		return 1;
	}
	else
	{
		return 0; // false - read did not work because file is empty or has
		//         been fully read
	}
}


int main(int argc, char *argv[])
{
	int i = 0, taken = 0;
	int branchCount = 0;
	int sizeBTB = 0;
	char *filename;
	printf("Please enter the source file: ");
	scanf("%s", filename);
	printf("Please enter the size of BTB wanted: ");
	scanf("%d", &sizeBTB);
	input = fopen(filename, "r");

	//while(readNew())
	while (!feof)
	{
		readNew();
		convert(); // ASCII to integer
		//printf("curAddr  = %s    prevAddr = %s\n", curAddr, prevAddr);
		//printf("cur      = %d   prev     = %d\n", cur, prev);

		if (cur != prev + 4 && prev) {
			//printf(" branch: %s  =>  %d\n", prevAddr, prev);
			taken = 1;
			branchCount++;
		}
		/*else
		{
		taken = 0;
		}*/



		//btb stuff
	}




	fclose(input);

	return 0;
}
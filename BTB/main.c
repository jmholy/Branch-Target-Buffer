// Brian Pecha - April 9, 2016

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


FILE *input;
char curAddr[] = "000000", prevAddr[] = "000000";
int  cur, prev;
int hit = 0, miss = 0;
double hitrate = 0.0;
//int sizes[3] = {256, 512, 1024};
char *table = "0123456789abcdef";


typedef struct btb
{
	int curPC;
	int predPC;
	int prediction;
	int busy;
} BTB;

void init(BTB btbname[], int size)
{
	int k = 0;
	for (k = 0; k < size; k++)
	{
		btbname[k].curPC = 0;
		btbname[k].predPC = 0;
		btbname[k].prediction = 0;
		btbname[k].busy = 0;
	}
}

int sm(int n, int taken)
{
	switch (n)
	{
	case 0: if (taken)
	{
		return 0;
	}
			else
			{
				return 1;
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

//need an add to BTB in available spot function
void addToBTB(BTB b[], int size, int curPC, int predPC, int pred)
{
	int i = 0;

	for (i = 0; i < size; i++)
	{
		if (b[i].busy == 0)
		{
			b[i].curPC = curPC;
			b[i].predPC = predPC;
			b[i].prediction = pred;
			b[i].busy = 1;
			return;
		}
	}
}


//need to search BTB for integer address prediction
// return the predicted PC if branch is found in BTB
int searchBTB(BTB b[], int size, int curPC)
{
	int i;

	for (i = 0; i < size; i++)
	{
		if (b[i].busy == 1)
		{
			if (curPC == b[i].curPC)
			{
				return i;
			}
		}
	}
	return -1;
}

int main(int argc, char *argv[])
{ 
		int i = 0, taken = 0;
		int branchCount = 0;
		int sizeBTB = 0;
		int k = 0;
		int position = 0;
		char *filename = "\0";
		BTB btbreal[1024];
		//printf("Please enter the source file: ");
		//scanf("%s", filename);
		while (sizeBTB != 256 && sizeBTB != 512 && sizeBTB != 1024)
		{
			printf("Please enter the size of BTB wanted (256, 512, 1024): ");
			scanf("%d", &sizeBTB);
		}
		input = fopen("trace.txt", "r");
		init(btbreal, sizeBTB);
		//while(readNew())
		while (!feof(input))
		{
			readNew();
			convert(); // ASCII to integer
			//printf("curAddr  = %s    prevAddr = %s\n", curAddr, prevAddr);
			//printf("cur      = %d   prev     = %d\n", cur, prev);
			position = searchBTB(btbreal, sizeBTB, prev);
			if (position >= 0)
			{
				if (cur != prev + 4 && prev) {
					//printf(" branch: %s  =>  %d\n", prevAddr, prev);
					taken = 1;
					if (btbreal[position].prediction == 2 || btbreal[position].prediction == 3)
					{
						miss++;
						btbreal[position].prediction = sm(btbreal[position].prediction, taken);
					}
					else
					{
						hit++;
						btbreal[position].prediction = sm(btbreal[position].prediction, taken);
					}
				}
				else
				{
					taken = 0;
					if (btbreal[position].prediction == 0 || btbreal[position].prediction == 1)
					{
						miss++;
						btbreal[position].prediction = sm(btbreal[position].prediction, taken);
					}
					else
					{
						hit++;
						btbreal[position].prediction = sm(btbreal[position].prediction, taken);
					}
				}
			}
			else
			{
				if (cur != prev + 4 && prev)
				{
					miss++;
					branchCount++;
					if (btbreal[sizeBTB-1].busy == 0)
					{
						addToBTB(btbreal, sizeBTB, prev, cur, 0);
					}
				}
			}
		}
		hitrate = (double)(hit / (double)(miss + hit));
		printf("Hits: %d  Misses: %d  Hit Rate: %lf\n", hit, miss, hitrate);
		fclose(input);
	return 0;
}
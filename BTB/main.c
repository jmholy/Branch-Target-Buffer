/*EE334
Mark Holy
Brian Pecha
Jordan Little*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
	FILE *input = NULL;
	int sizeBTB = 0;
	char *filename;
	char *curPC[1024], *desPC[1024], *pred[1024], *busy[1024];
	char *address;
	int curAddress = 0, prevAddress = 0;
	int rightPred = 0, wrongPred = 0, hitrate = 0;
	printf("Please enter the source file: ");
	scanf("%s", filename);
	printf("Please enter the size of BTB wanted: ");
	scanf("%d", &sizeBTB);
	input = fopen(filename, "r");
	while (!feof)
	{
		fscanf(input, "%s", address);
		/*Brian's conversion from Hex to Dec*/
		if (curAddress == (prevAddress + 4)) // Not Branch
		{ 
			
		}
		else //Branch
		{

		}
	}
	return 0;
}
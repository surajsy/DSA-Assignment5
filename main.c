#include <stdio.h>
#include <stdlib.h>
#include "spheap.h"

int main()
{
	int *num;
	float *frac;

	num = (int*)spheap(sizeof(int));
	printf("\n");
	*num = 25;
	frac = (float*)spheap(sizeof(float));
	printf("\n");
	*frac = 0.234;

	if(num!=NULL) printf("%d\n", *num);
	if(frac!=NULL) printf("%f\n", *frac);

	freeSpheap(num);
	printf("\n");
	freeSpheap(frac);
	printf("\n");
	return 0;
}
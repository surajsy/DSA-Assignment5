#include <stdio.h>
#include <stdlib.h>
#include "spheap.h"

int main()
{
	int *num, *num2;
	float *frac, *frac2;

	num = (int*)spheap(10*sizeof(int));
	printf("\n");
//	*num = 25;
	frac = (float*)spheap(5*sizeof(float));
	printf("\n");
///	*frac = 0.234;


//	if(num!=NULL) printf("%d\n", *num);
//	if(frac!=NULL) printf("%f\n", *frac);
	freeSpheap(frac);
	printf("\n");
	freeSpheap(num);
	printf("\n");
	return 0;
}
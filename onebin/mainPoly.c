#include<stdio.h>
#include "largeintpoly.h"

void main(){

	Poly *poly1, *poly2, *q, *r;
	int digit,i;
	char c;
	
	poly1 = NULL;
	poly2 = NULL;
	
	printf("Polynomial divison: Input two polynomials\n");
	readPoly(&poly1);
	printf("A=");
	printPoly(poly1);


	readPoly(&poly2);
	printf("B=");
	printPoly(poly2);
	
	printf("Performing A/B:\n");
	r = divPoly(poly1,poly2,&q);
	
	printf("Quotient:");
	printPoly(q);
	printf("Remainder:");
	printPoly(r);
	
	delPoly(&r);
	delPoly(&q);
	delPoly(&poly1);
	delPoly(&poly2);
}
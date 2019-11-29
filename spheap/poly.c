#include<stdio.h>
#include "largeintpoly.h"

void printPoly(Poly* header){
	Poly *temp = header;
	
	if(temp == NULL){
		printf("\n");
		return;
	}
	
	while(temp->next!=NULL){
		printf("(%d,%.2f),",temp->deg,temp->coeff);
		temp = temp->next;
	}
	printf("(%d,%.2f)\n",temp->deg,temp->coeff);
	return;
}

void readPoly(Poly** header){
	int deg,n,tempDeg;
	float coeff;
	
	tempDeg = INF;
	
	do{
		n=scanf("(%d,%f)",&deg,&coeff);
		if(n==0){
			printf("error reading input\n");
			exit(0);
		}

		if(deg<=tempDeg){ 
			prependPoly(header,deg,coeff);
			tempDeg = deg-1;
		}
		
		else{
			printf("skipped coeff of deg %d. Not in order\n",deg);
		}
	}while(getchar()!='\n');
	
	reversePoly(header);
}

void prependPoly(Poly** header, int deg, float coeff){
	Poly *temp;
	temp = createPoly();
	temp->coeff = coeff;
	temp->deg = deg;
	
	if(*header == NULL){
		*header = temp;
	}
	else{
		temp->next = *header;
		(*header)->prev = temp;
		*header = temp;
	}
//	printf("prepending\n");
}

void appendPoly(Poly** header, int deg, float coeff){
	Poly* temp = *header;
	if(*header == NULL){
		*header = createPoly();
		(*header)->deg = deg;
		(*header)->coeff = coeff;
	}
	else {
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = createPoly();
		temp->next->prev = temp;
		temp->next->deg = deg;
		temp->next->coeff = coeff;
	}
}

Poly* divPoly(Poly* dividend, Poly* divisor,Poly** quotient){
	Poly *rem, *div, *prev;
	int index;
	float tempCoeff,ratio;
	
	*quotient = NULL;
	prev = NULL;
	rem = copyPoly(dividend);

	while(rem!=NULL && rem->deg >= divisor->deg){
		div = divisor;
//		printPoly(rem);
		index = rem->deg - div->deg;
		
		ratio = rem->coeff / div->coeff;
		appendPoly(quotient,index,ratio);
		delPolyNode(&rem);
		div = div->next;
		
		while(div!=NULL){
			if(rem!=NULL && rem->deg - div->deg == index){
				tempCoeff = rem->coeff - ratio*div->coeff;
//				printf("%f\n",tempCoeff);
				if(abs2(tempCoeff)<=ZERO) delPolyNode(&rem);
				else rem->coeff = tempCoeff;
//				printPoly(rem);
				div = div->next;
			}
			
			else if(rem!=NULL && rem->deg - div->deg > index){
				prev = rem;
				rem = rem->next;
//				printPoly(rem);
			}
			
			else if(rem!=NULL && rem->deg - div->deg < index){
				insertPolyNode(&rem,div->deg + index,-ratio*div->coeff);
//				printPoly(rem);
				div = div->next;
			}
			
			else if(rem == NULL && prev!=NULL){
//				printf("yolo\n");
				appendPoly(&prev,div->deg + index,-ratio*div->coeff);
				rem = prev->next;
//				printPoly(rem);
				div = div->next;
			}
		}
		
		while(rem!=NULL && rem->prev!=NULL){
			rem = rem->prev;
		}
	}
	
	if(rem == NULL){
		rem = createPoly();
		rem->deg = 0;
		rem->coeff = 0;
	}
	return rem;
}

Poly* copyPoly(Poly* expr){
	Poly *temp, *exprOut;
	
	exprOut = NULL;
	temp = expr;
	while(temp!=NULL){
//		printf("%d %0.2f\n",temp->deg,temp->coeff);
		appendPoly(&exprOut,temp->deg,temp->coeff);
		temp = temp->next;
	}
	
	return exprOut;
}

float abs2(float num){
	if (num<0) return -num;
	else return num;
}


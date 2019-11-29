#include<stdio.h>
#include "largeintpoly.h"
#include "onebin.h"

Node* createNode(){
	Node* node;
	node = (Node*)oneBin(sizeof(Node));
	node->digit = 0;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

Poly* createPoly(){
	Poly* node;
	node = (Poly*)oneBin(sizeof(Poly));
	node->coeff = 0;
	node->deg = 0;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

void clearList(Node** header){
	Node* temp;
	while((*header)!=NULL){
		temp = *header;
		(*header) = (*header)->next;
		freeOneBin(temp);
	}
}

void reverseList(Node** header){
	Node *temp, *current;
	temp = NULL;
	current = *header;
	
	while(current!=NULL){
		temp = current->prev;
		current->prev = current->next;
		current->next = temp;
		current = current->prev;
	}
	
	if(temp!=NULL)
		*header = temp->prev;
}

void reversePoly(Poly** header){
	Poly *temp, *current;
	temp = NULL;
	current = *header;
	
	while(current!=NULL){
		temp = current->prev;
		current->prev = current->next;
		current->next = temp;
		current = current->prev;
	}
	
	if(temp!=NULL)
		*header = temp->prev;
}

void delPolyNode(Poly** header){
	Poly *temp = *header;
	
	if(*header == NULL) return;
	*header = temp->next;
	if (temp->next!=NULL) temp->next->prev = temp->prev;
	if(temp->prev!=NULL) temp->prev->next = temp->next;
	freeOneBin(temp);
}

void delPoly(Poly** header){
	while(*header!=NULL){
//		printf("%d %0.2f\n",(*header)->deg,(*header)->coeff);
		delPolyNode(header);
	}
}

void insertPolyNode(Poly** header, int deg, float coeff){
	Poly *temp;
	
	if(*header == NULL || (*header)->prev == NULL) prependPoly(header,deg,coeff);
	else{
		temp = createPoly();
		temp->deg = deg;
		temp->coeff = coeff;
		
		temp->next = *header;
		temp->prev = (*header)->prev;
		
		(*header)->prev->next = temp;
		(*header)->prev = temp;
	}
}
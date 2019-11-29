#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "onebin.h"

#define ALLOC(X) (X*)malloc(sizeof(X))
#define SIZE sizeof(Poly)
#define FREE 1

struct poly
{
	float coeff;
	int deg;
	struct poly* next;
	struct poly* prev;
};

typedef struct poly Poly;

struct asl{
	void *memory;
	int tag;
	size_t size;
	struct asl *next;
	struct asl *prev;
};

typedef struct asl ASL;

char memory[MEMMAX];
ASL* freeList;
int init;

void initialize(){
	freeList = ALLOC(ASL);
	freeList->memory = (void*)memory;
	freeList->size = MEMMAX;
	freeList->tag = FREE;
	freeList->next = NULL;
	freeList->prev = NULL;
}

ASL* createAslNode(size_t size){
	ASL* new;
	new = ALLOC(ASL);
	new->memory = NULL;
	new->tag = FREE;
	new->size = size;
	new->next = NULL;
	new->prev = NULL;
}

ASL* split(ASL *slot,size_t size){
	ASL *new1,*new2,*temp;
	int factor;
	size_t s=SIZE;

	if(slot->size < size){
		printf("size less than size\n");
		return NULL;
	}

	factor = size/SIZE;
	if(size > factor*SIZE)
		factor = factor + 1;

	new1 = createAslNode(slot->size - factor*SIZE);
	new2 = createAslNode(factor*SIZE);
	new1->memory = slot->memory + factor*SIZE;
	new2->memory = slot->memory;
	new1->next = new2;
	new2->next = slot->next;
	new2->prev = new1;
	new1->prev = slot->prev;

	if(slot->prev != NULL)
		slot->prev->next = new1;
	else
		freeList = new1;

	if(slot->next != NULL)
		slot->next->prev = new2;

	free(slot);
	return new2;
}

void* oneBin(size_t bytes){
	ASL *current,*prev;

	if(!init){
		initialize();
		init = 1;
	}

	current = freeList;
	while((current->size < bytes || current->tag != FREE) && current != NULL){
		current = current->next;
	}

	if(current != NULL)
	current = split(current,bytes);
	
	if(current != NULL){
		current->tag = !FREE;
//		printf("%p\n", current->memory);
		return current->memory;
	}

	else{
		printf("request cannot be fulfilled\n");
		return NULL;
	} 
}

void merge(ASL *node){
	ASL *new;
	if(node == NULL)
		return;

	if(node->size == MEMMAX){
		node->tag = FREE;
		return;
	}

	if(node->prev!=NULL && node->prev->tag == FREE){
		new = createAslNode(node->size + node->prev->size);
		new->memory = node->prev->memory;
		new->next = node->next;
		new->prev = node->prev->prev;

		if(node->next != NULL) 
			node->next->prev = new;

		if(node->prev->prev != NULL) 
			node->prev->prev->next = new;
		else freeList = new;

		free(node->prev);
		free(node);
		merge(new);
	}
	else if(node->next!=NULL && node->next->tag == FREE){
		new = createAslNode(node->size + node->next->size);
		new->memory = node->memory;
		new->next = node->next->next;
		new->prev = node->prev;

		if(node->prev != NULL)
			node->prev->next = new;
		else
			freeList = new;

		if(node->next->next != NULL)
			node->next->next->prev = new;

		free(node->next);
		free(node);
		merge(new);
	}
	else
		node->tag = FREE;

}

void freeOneBin(void *block){
	ASL *current = freeList;
	ASL *temp; 
	if(block == NULL)
		return;

	while(current != NULL && current->memory != block){
		current = current->next;
	}

	if(current == NULL || current->tag == FREE){
		printf("invalid address or memory already freed\n");
		return;
	}

	merge(current);
}



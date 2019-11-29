#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include "spheap.h"

#define KMAX 28

#define FREE 1

#define EVEN 0

#define TYPE1 1
#define TYPE2 2
#define TYPE3 3

#define ALLOC(X) (X*)malloc(sizeof(X))
#define POW2K(X) (1 << (X))

struct asl{
	void *memory;
	size_t size;
	int KVAL;
	int TAG;
	int TYPE;
	struct asl *next;
	struct asl *prev;
};

typedef struct asl ASL;

struct parentType{
	void *address;
	size_t size;
	int TYPE;
	struct parentType *next;
	struct parentType *prev;
};

typedef struct parentType PT;

const size_t memmax = MEMMAX;
unsigned long int allocMem = 0;
char memory[MEMMAX];
ASL *freeList = NULL;
PT *typeList = NULL;
int init;
unsigned long int intFrag,splits,recomb;

void initialize(){
	freeList = ALLOC(ASL);
	freeList->memory = (void*)memory;
	freeList->size = MEMMAX;
	freeList->KVAL = KMAX;
	freeList->TAG = FREE;
	freeList->TYPE = TYPE3;
	freeList->next = NULL;
	freeList->prev = NULL;
}

ASL* createAslNode(int k,int type,size_t size){
	ASL* new;
	new = ALLOC(ASL);
	new->memory = NULL;
	new->size = size;
	new->KVAL = k;
	new->TYPE = type;
	new->TAG = FREE;
	new->next = NULL;
	new->prev = NULL;
	return new;
}

void printASL(){
	ASL *temp = freeList;
	while(temp!=NULL){
		printf("%4d %16ld %16p %d\n", temp->TYPE,temp->size,temp->memory,temp->TAG);
		temp = temp->next;
	}
}

PT* createPTnode(void* address,int type,size_t size){
	PT* new;
	new = ALLOC(PT);
	new->address = address;
	new->TYPE = type;
	new->size = size;
	new->next = NULL;
	new->prev = NULL;
	return new;
}

int getType(void *address,size_t size){
	PT *temp;
	int type;
	temp = typeList;
	while(temp!=NULL){
		if(temp->address == address && temp->size == size){
			type = temp->TYPE;
			break;
		}
		temp = temp->next;
	}

	if(temp == NULL){
		printf("error getting type: %p %ld\n",address,size);
		exit(0);
	}
	return type;
}

void deleteType(void *address,size_t size){
	PT* temp;
	temp = typeList;
	while(temp!=NULL){
		if(temp->address==address && temp->size == size)
			break;
		temp = temp->next;
	}

	if(temp == NULL)
		return;

	if(temp->next != NULL) temp->next->prev = temp->prev;
	if(temp->prev != NULL) temp->prev->next = temp->next;

	if(temp->prev == NULL) typeList = temp->next;

	free(temp);
}

void printTypeList(){
	PT *temp = typeList;
	while(temp!=NULL){
		printf("%16d %16p %16ld\n",temp->TYPE,temp->address, temp->size);
		temp = temp->next;
	}
}

ASL* split(ASL *slot, size_t size){
	ASL *new1, *new2;
	PT *typePtr;
	int K = slot->KVAL;
	int type = slot->TYPE;

	if(slot->size == size)
		return slot;

	if(slot->size == 2 && size == 1)
		return slot;

	splits++;

	if((type == TYPE3) && (slot->size % POW2K(K+2)) == 3*POW2K(K)){
		new1 = createAslNode(K+1,TYPE1,POW2K(K+1));
		new2 = createAslNode(K,TYPE2,POW2K(K));
		new1->memory = slot->memory;
		new2->memory = slot->memory + POW2K(K+1);
	}
	else{
		new1 = createAslNode(K-2 , TYPE3 , 3*POW2K(K-2));
		new2 = createAslNode(K-2 , TYPE3 , POW2K(K-2));
		new1->memory = slot->memory;
		new2->memory = slot->memory + 3*POW2K(K-2);
	}

	typePtr = createPTnode(slot->memory,slot->TYPE,slot->size);
	typePtr->next = typeList;
	if(typeList != NULL) typeList->prev = typePtr;
	typeList = typePtr;

	new1->next = new2;
	new2->prev = new1;
	new2->next = slot->next;
	new1->prev = slot->prev;

	if(slot->next != NULL) 
		slot->next->prev = new2;
	
	if(slot->prev != NULL) 
		slot->prev->next = new1;
	else	
		freeList = new1;		
	
	free(slot);
	if(new1->size == size)
		return new1;
	else if(new2->size == size)
		return new2;
	else if(new2->size > size)
		split(new2,size);
	else
		split(new1,size);

}

void* spheap(size_t bytes){
	ASL *current,*prev;
	void *block;
	double percentage;
	size_t temp;
	size_t pow3,pow2;
	int K=0;

	if(!init){
		initialize();
		init = 1;
//		printf("memory initialized\n");
	}

	temp = bytes;
	while(temp!=0){
		temp = temp >> 1;
		K++;
	}

	pow2 = POW2K(K);
	pow3 = 3*POW2K(K-2);

	if(K == 1)
		temp = 1;
	else if(pow3 > bytes)
		temp = pow3;
	else
		temp = pow2;

	current = freeList;
	while(current != NULL && (current->size < temp || current->TAG != FREE)){
		current = current->next;
	}

	if (current != NULL)
	current = split(current,temp);

	if(current != NULL){
		current->TAG = !FREE;
		//printf("memory allocated size: %ld\n",temp);
		intFrag = intFrag + current->size - bytes;
		allocMem = allocMem + current->size;
		return current->memory;
	}

	else{
		printf("request cannot be fulfilled\n");
		percentage = 100*(double)allocMem/memmax;
		printf("requested = %ld bytes; percentage of allocated memory = %lf\n", bytes,percentage);
		return NULL;
	}
}

int parity(unsigned long int num){
	int p=0;
	while(num!=0){
		p = !p;
		num = num & (num -1);
	}
	return p;
}

void merge(ASL *node){
	void *buddy;
	ASL *new; 
	unsigned long int mod,size;
	int K;
	int type;
	ASL* temp;
	int p;
	unsigned long int min,min1;

	if(node->KVAL == KMAX){
		node->TAG = FREE;
		return;
	}

	mod = ((unsigned long int)node->memory - (unsigned long int)memory)% POW2K(node->KVAL + 2);
	switch(node->TYPE){
		case TYPE1:
			buddy = node->memory + POW2K(node->KVAL);
			break;
		case TYPE2:
			buddy = node->memory - POW2K(node->KVAL + 1);
			break;
		case TYPE3:
			if(mod == 0){
				buddy = node->memory + 3*POW2K(node->KVAL);
			}
			else if(mod == 3*POW2K(node->KVAL)){
				buddy = node->memory - 3*POW2K(node->KVAL);
			}
			else
				printf("something wrong with mod\n");
			break;
		default:
			printf("something wrong with the type\n");
	}

	if(node->TYPE == TYPE3){
		K = node->KVAL + 2;
		size = POW2K(K);
	}

	else if(node->TYPE == TYPE2){
		K = node->KVAL;
		size = 3*POW2K(K);
	}
	else if(node->TYPE == TYPE1){
		K = node->KVAL - 1;
		size = 3*POW2K(K);
	}

//	printTypeList();
	if(buddy < node->memory)
		type = getType(buddy,size);
	else
		type = getType(node->memory,size);

	if(node->next != NULL && node->next->memory == buddy){
		if(node->next->TAG == FREE && (node->size + node->next->size == size)){
			//Find K!!!

			new = createAslNode(K,type,size);

			new->memory = node->memory;
			if(node->prev!=NULL){
				node->prev->next = new;
				new->prev = node->prev;
			}
			else
				freeList = new;

			if(node->next->next!=NULL){
				node->next->next->prev = new;
				new->next = node->next->next;
			}

			deleteType(node->memory,size);

			free(node->next);
			free(node);

			recomb++;
			merge(new);
		}
		else{
			node->TAG = FREE;
			return;
		}
	}
	else if(node->prev != NULL && node->prev->memory == buddy){
		if(node->prev->TAG == FREE && (node->prev->size + node->size == size)){
			//Find K!!!

			new = createAslNode(K,type,size);

			new->memory = node->prev->memory;
			if(node->next!=NULL){
				node->next->prev = new;
				new->next = node->next;
			}

			if(node->prev->prev !=NULL){
				node->prev->prev->next = new;
				new->prev = node->prev->prev;
			}
			else
				freeList = new;

			deleteType(node->prev->memory,size);

			free(node->prev);
			free(node);

			recomb++;
			merge(new);
		}
		else{
			node->TAG = FREE;
			return;
		}
	}
	else{
		node->TAG = FREE;
		return;
	}

}

void freeSpheap(void *block){
	ASL *current = freeList;
	ASL *temp; 
	if(block == NULL)
		return;

	while(current != NULL && current->memory != block){
		current = current->next;
	}

	if(current == NULL || current->TAG == FREE){
		printf("memory already freed\n");
		return;
	}
	
	allocMem = allocMem - current->size;
	merge(current);

}

unsigned long int totIntFrag(){
	if(intFrag<0)
		printf("overflow in intFrag\n");
	return intFrag;
}

unsigned long int Nsplits(){
	if(splits<0)
		printf("overflow in splits\n");
	return splits;
}

unsigned long int budRecomb(){
	if(recomb<0)
		printf("overflow in recomb\n");
	return recomb;
}
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "spheap.h"

char memory[MEMMAX];
ASL *freeList;
int init;

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

ASL* split(ASL *slot, size_t size){
	ASL *new1, *new2;
	int K = slot->KVAL;
	int type = slot->TYPE;

//	printf("%d ", K);
	if(slot->size < size)
		return NULL;

	if(size % (slot->size/2) != size)
		return slot;

	if((type == TYPE3) && (slot->size % POW2K(K+2)) == 3*POW2K(K)){
		new1 = createAslNode(K+1,TYPE1,POW2K(K+1));
		new2 = createAslNode(K,TYPE2,POW2K(K));
		new1->memory = slot->memory;
		new2->memory = slot->memory + POW2K(K+1);
//		printf("if part\n");
	}
	else{
		new1 = createAslNode(K-2 , TYPE3 , 3*POW2K(K-2));
		new2 = createAslNode(K-2 , TYPE3 , POW2K(K-2));
		new1->memory = slot->memory;
		new2->memory = slot->memory + 3*POW2K(K-2);
//		printf("else part\n");
	}

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
	if(new2->KVAL > new1->KVAL)
		return split(new1,size);
	else if (new2->size < size)
		return split(new1,size);
	else
		return split(new2,size);

}

void* spheap(size_t bytes){
	ASL *current,*prev;
	ASL *temp;
	void *block;

	if(!init){
		initialize();
		init = 1;
		printf("memory initialized\n");
	}

	current = freeList;
	while(current->size < bytes || current->TAG != FREE){
		current = current->next;
	}

	current = split(current,bytes);
	printf("\n");
	temp = freeList;

	while(temp!=NULL){
		printf("%d ", temp->TYPE);
		temp = temp->next;
	}
	printf("\n");

	if(current != NULL){
		current->TAG = !FREE;
		printf("memory allocated\n");
		printf("current->KVAL = %d, current->TYPE = %d, current->size = %ld\n", current->KVAL,current->TYPE,current->size);
		return current->memory;
	}

	else{
		printf("request cannot be fullfilled\n");
		return NULL;
	}
}

void merge(ASL *node){
	void* buddy;
	ASL* new; 
	int mod;
	int K;
	int type;
	ASL* temp;

	if(node->KVAL == KMAX){
		node->TAG = FREE;
		return;
	}

	if(node->KVAL == 26 && node->prev != NULL && node->prev->KVAL == 27)
		node->TYPE = TYPE2;

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
				printf("if\n");
			}
			else if(mod == 3*POW2K(node->KVAL)){
				buddy = node->memory - 3*POW2K(node->KVAL);
			}
			else printf("something wrong with mod value or the type\n");
	}

	temp = freeList;
	while(temp!=NULL){
		printf("%d ", temp->KVAL);
		temp = temp->next;
	}
	printf("\n");

	if(node->next != NULL && node->next->memory == buddy){
		printf("if part\n");
		if(node->next->TAG == FREE){
			//Find K!!!
			if(node->TYPE == TYPE3)
				K = node->KVAL + 2;
			else if(node->TYPE == TYPE1)
				K = node->KVAL - 1;

			new = createAslNode(K,TYPE3,POW2K(K));
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

			free(node->next);
			free(node);

			merge(new);
		}
		else{
			node->TAG = FREE;
			return;
		}
	}
	else if(node->prev != NULL && node->prev->memory == buddy){
//		printf("else part\n");
		if(node->prev->TAG == FREE){
			//Find K!!!
			if(node->TYPE == TYPE3)
				K = node->KVAL + 2;
			else if(node->TYPE == TYPE2)
				K = node->KVAL;

			new = createAslNode(K,TYPE3,POW2K(K));
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

			free(node->prev);
			free(node);

			merge(new);
		}
		else{
			node->TAG = FREE;
			return;
		}
	}
//	else printf("buddy address not calculated properly\n");

}

void freeSpheap(void *block){
	ASL *current = freeList;
	ASL *temp; 
	if(block == NULL)
		return;

	while(current->memory != block){
		current = current->next;
	}

	if(current->prev !=NULL) printf("%d<-", current->prev->KVAL);
	else printf("NULL<-");

	printf("%d",current->KVAL);

	if(current->next != NULL) printf("->%d\n", current->next->KVAL);
	else printf("->NULL\n");

	merge(current);
	temp = freeList;
	while(temp!=NULL){
		printf("%d ", temp->KVAL);
		temp = temp->next;
	}
	printf("\n");
}
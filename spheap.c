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

	if(slot->size < size)
		return NULL;

	if(size % (slot->size/2) != size){
		printf("%ld %ld\n", size,slot->size);
		return slot;
	}

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

	if(current != NULL){
		current->TAG = !FREE;
		printf("memory allocated\n");
		printf("KVAL = %d TYPE = %d\n", current->KVAL, current->TYPE);
		return current->memory;
	}

	else{
		printf("request cannot be fullfilled\n");
		return NULL;
	}
}

void merge(ASL *node){
	void *buddy,*pBud1,*pBud2,*pBud3;
	ASL *new,*parent1,*parent2,*parent3; 
	int mod;
	int K;
	int type;
	ASL* temp;
	int count1,count2,count3;

	if(node->KVAL == KMAX){
		node->TAG = FREE;
		return;
	}

/*	if(node->KVAL == 26 && node->prev != NULL && node->prev->KVAL == 27)
		node->TYPE = TYPE2;*/

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
			else printf("something wrong with mod value or the type\n");
	}

	temp = freeList;
	while(temp!=NULL){
		printf("%d ", temp->KVAL);
		temp = temp->next;
	}
	printf("\n");

	if(node->TYPE == TYPE3){
		K = node->KVAL + 2;
		pBud1 = buddy + POW2K(K);
		pBud2 = buddy - POW2K(K+1);
		pBud3 = buddy - 3*POW2K(K);

		parent1 = node->next;
		count1 = 0;
		while(parent1!=NULL && parent1->memory!= pBud1){
			parent1 = parent1->next;
			count1++;
		}

		parent2 = node->prev;
		count2 = 0;
		while(parent2!=NULL && parent2->memory!= pBud2){
			parent2 = parent2->prev;
			count2++;
		}

		parent3 = node->prev;
		count3 = 0;
		while(parent3!=NULL && parent3->memory!= pBud3){
			parent3 = parent3->prev;
			count3++;
		}

		if(parent1!=NULL && count1 != 0){
			type = TYPE1;
			printf("type = %d count1 = %d\n", type,count1);
		}
		else if(parent2!=NULL && count2 != 0){
			type = TYPE2;
			printf("type = %d count2 = %d\n", type,count2);
		}
		else if(parent3!=NULL && count3 != 0){
			type = TYPE3;
			printf("type = %d count3 = %d\n", type,count3);
		}
		else type = TYPE3;

	}
	else if(node->TYPE == TYPE2){
		K = node->KVAL;
		type = TYPE3;
	}
	else if(node->TYPE == TYPE1){
		K = node->KVAL - 1;
		type = TYPE3;
	}


	if(node->next != NULL && node->next->memory == buddy){
		if(node->next->TAG == FREE){
			//Find K!!!

			new = createAslNode(K,type,POW2K(K));
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
		if(node->prev->TAG == FREE){
			//Find K!!!

			new = createAslNode(K,type,POW2K(K));
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
	
	printf("KVAL = %d TYPE = %d\n", current->KVAL, current->TYPE);	
	merge(current);
	temp = freeList;
	while(temp!=NULL){
		printf("%d ", temp->KVAL);
		temp = temp->next;
	}
	printf("\n");
}
#include <stdlib.h>
#include <math.h>
#include "sim.h"

List* createList(){
	List* list;
	list = ALLOC(List);
	list->timeNode = NULL;
	return list;
}

TS* createNode(int time,void* memory){
	TS* ts;
	ts = ALLOC(TS);
	ts->time = time;
	ts->memory = memory;
	ts->next = NULL;
	ts->prev = NULL;
}

void insert(int time, void* memory, List* list){
	TS *ts;
	ts = createNode(time,memory);
	if(list->timeNode == NULL){
		list->timeNode = ts;
		return;		
	}

	ts->next = list->timeNode;
	list->timeNode->prev = ts;
	list->timeNode = ts;
}

void delete(TS *ts,List *list){

	if(ts == NULL)
		return;

	if(ts->next!=NULL) ts->next->prev = ts->prev;
	if(ts->prev!=NULL) ts->prev->next = ts->next;

	if(ts->prev == NULL) list->timeNode = ts->next;

	free(ts);
}

size_t uReq(){
	int range = MAX - MIN + 1;

	int chunkSize = (RAND_MAX)/range;
	int endOfLastChunk = chunkSize*range;

	int r = rand();

	while(r >= endOfLastChunk){
		r = rand();
	}
	return (size_t)(MIN + r/chunkSize);
}

size_t eReq(){
	double e,u;
	u = (double)rand()/RAND_MAX;
	e = -MEAN*log(1 - u);

	e = MIN + e;
	if(e > MAX)
		e = MAX;
	return (size_t)e;
}

int randTime(){
	int range = TMAX + 1;

	int chunkSize = (RAND_MAX)/range;
	int endOfLastChunk = chunkSize*range;

	int r = rand();

	while(r >= endOfLastChunk){
		r = rand();
	}
	return (r/chunkSize);	
}
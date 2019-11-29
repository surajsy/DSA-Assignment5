#include <stdio.h>
#include "spheap.h"
#include "sim.h"

int main()
{
	int time,T;
	size_t size,totSize;
	void *memory;
//	void *memory[10];
	TS* ts, *temp;
	List *list;
	int nDealloc;
	double fracIntFrag;
	list = createList();
	int i;
	const int tmax = 100000;
	nDealloc = 0;
	totSize = 0;
	time = 0;

	while(time < tmax){
		ts = list->timeNode;
		while(ts != NULL){
			if(ts->time == time){
				freeSpheap(ts->memory);
				temp = ts->next;
				delete(ts,list);
				ts = temp;
				nDealloc++;
			}
			else
				ts = ts->next;
		}

		size = uReq();
		T = randTime();

		memory = spheap(size);

		totSize += size;

		if(time % 1000 == 0){
			fracIntFrag = (double)totIntFrag()*100/totSize;
			printf("time = %d\n", time);
			printf("nDAll\tIntFrag\t\tNsplits\tRecomb\n");
			printf("%4d\t %lf\t %ld\t %ld\n", nDealloc, fracIntFrag,Nsplits(),budRecomb());
		}

		if(memory == NULL) break;

		insert(time + T,memory,list);
		time++;
	}

	fracIntFrag = (double)totIntFrag()*100/totSize;
	printf("time = %d\n", time);
	printf("nDAll\tIntFrag\t\tNsplits\tRecomb\n");
	printf("%4d\t %lf\t %ld\t %ld\n", nDealloc, fracIntFrag,Nsplits(),budRecomb());
	
	ts = list->timeNode;
	while(ts != NULL){
		freeSpheap(ts->memory);
		temp = ts->next;
		delete(ts,list);
		ts = temp;
	}

	return 0;
}
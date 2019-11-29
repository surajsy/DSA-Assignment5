#ifndef SPHEAP_H
#define SPHEAP_H

#ifndef MEMMAX
#define MEMMAX 256*1024*1024
#endif

unsigned long int totIntFrag();

unsigned long int Nsplits();

unsigned long int budRecomb();

void* spheap(size_t bytes);

void freeSpheap(void *block);

#endif
#ifndef ONEBIN_H
#define ONEBIN_H

#ifndef MEMMAX
#define MEMMAX 256*1024*1024
#endif

void *oneBin(size_t bytes);

void freeOneBin(void *block);

#endif
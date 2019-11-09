#define MEMMAX 256*1024*1024
#define KMAX 28

#define FREE 1
#define TYPE0 00
#define TYPE1 01
#define TYPE2 10
#define TYPE3 11

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

void* spheap(size_t bytes);

void freeSpheap(void *block);
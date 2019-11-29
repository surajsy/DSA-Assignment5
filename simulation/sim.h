#define MEAN 100000
#define MAX 200000
#define MIN 10000

#define TMAX 10000

#define ALLOC(X) (X*)malloc(sizeof(X))

struct timeStamp
{
	unsigned int time;
	void* memory;
	struct timeStamp *next;
	struct timeStamp *prev;
};

typedef struct timeStamp TS;

struct list
{
	TS* timeNode;
};

typedef struct list List;

size_t uReq();

size_t eReq();

int randTime();

List* createList();

void insert(int time, void* memory, List* list);

void delete(TS *ts,List *list);


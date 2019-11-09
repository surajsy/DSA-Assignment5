mem.out: main.o spheap.o
	gcc -g main.o spheap.o -o mem.out

main.o: main.c spheap.h
	gcc -g -c main.c

spheap.o: spheap.c spheap.h
	gcc -g -c spheap.c
spHeapPoly.out: mainPoly.o spheap.o poly.o list.o
	gcc -g mainPoly.o spheap.o poly.o list.o -o spHeapPoly.out

mainPoly.o: mainPoly.c largeintpoly.h 
	gcc -g -c mainPoly.c

poly.o: poly.c list.c largeintpoly.h
	gcc -g -c poly.c list.c

list.o: list.c largeintpoly.h spheap.h
	gcc -g -c list.c

spheap.o: spheap.c spheap.h
	gcc -g -c spheap.c
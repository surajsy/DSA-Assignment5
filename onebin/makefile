oneBinPoly.out: mainPoly.o poly.o list.o onebin.o
	gcc -g mainPoly.o poly.o list.o onebin.o -o oneBinPoly.out

mainPoly.o: mainPoly.c largeintpoly.h
	gcc -g -c mainPoly.c

poly.o: poly.c list.c largeintpoly.h
	gcc -g -c poly.c list.c

list.o: list.c largeintpoly.h onebin.h
	gcc -g -c list.c

onebin.o: onebin.c onebin.h
	gcc -g -c onebin.c
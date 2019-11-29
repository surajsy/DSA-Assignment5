usim.out: mainUniSim.o spheap.o sim.o
	gcc -g mainUniSim.o spheap.o sim.o -o usim.out -lm

mainUniSim.o: mainUniSim.c spheap.h
	gcc -g -c mainUniSim.c

spheap.o: spheap.c spheap.h
	gcc -g -c spheap.c 

sim.o: sim.c sim.h
	gcc -g -c sim.c -lm
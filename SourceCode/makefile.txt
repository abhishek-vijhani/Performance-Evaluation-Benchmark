all: cpu memory disk net clean

cpu: cpu.c 
	gcc -mavx2 -pthread -o cpu cpu.c
	./cpu 1
	./cpu 2
	./cpu 3
	./cpu 4

memory: memory.c 
	gcc  -pthread -o mem memory.c
	./mem

disk: disk.c
	gcc  -pthread -o disk disk.c
	./disk

net: net.cpp
	g++ -std=c++0x -pthread -o net net.cpp -Wl,--no-as-needed
	./net 49294 1
	./net 49394 2
	./net 49494 4
	./net 49594 8

clean:
	rm -rf *.o cpu mem disk net 


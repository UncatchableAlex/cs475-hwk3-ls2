all: main.c ls2.h ls2.o
	gcc -Wall -g -o ls2 main.c ls2.o

ls2.o: ls2.h ls2.c
	gcc -Wall -g -c ls2.c
	
clean:
	rm -f ls2 *.o

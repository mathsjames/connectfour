CC=gcc
CFLAGS=-I.

make: connectfour.o backendutilities.o frontendutilities.o abagent.o
	$(CC) -o connectfour connectfour.o backendutilities.o frontendutilities.o abagent.o -I.

clean:
	-rm *.o

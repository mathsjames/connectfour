CC=gcc
CFLAGS=-g -I.

make: connectfour.o backendutilities.o frontendutilities.o abagent.o mcagent.o gamestructure.o
	$(CC) -o connectfour connectfour.o backendutilities.o frontendutilities.o abagent.o mcagent.o gamestructure.o -g -lm -I.

clean:
	-rm *.o

all: serverFork.c
	gcc -g -Wall -o server serverFork.c

clean: 
	$(RM) server
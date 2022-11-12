CC = gcc
CFLAGS  = -g -Wall -std=c99 -pedantic

default: q1 q2

q1:  q1.o
	 $(CC) $(CFLAGS) q1.o -o q1

q1.o:  q1.c 
	   $(CC) $(CFLAGS) -c q1.c

q2:  q2.o
	 $(CC) $(CFLAGS) q2.o -o q2

q2.o:  q2.c 
	   $(CC) $(CFLAGS) -c q2.c
clean: 
	$(RM) *.o
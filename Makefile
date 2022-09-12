CC=cc
CFLAGS= -O3 -DNDEBUG -I/usr/local/include/igraph -I/usr/local/include
#CFLAGS=-Wall -g -I/usr/local/include/igraph -I/usr/local/include
LDFLAGS=-L/usr/local/lib

all: taucritical

taucritical: taucritical.c graph6_utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) taucritical.c graph6_utils.o -ligraph -o taucritical 

graph6_utils.o: graph6_utils.c
	$(CC) -c $(CFLAGS) graph6_utils.c -o graph6_utils.o

clean:
	rm -rf taucritical graph6_utils.o

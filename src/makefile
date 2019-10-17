# makefile
# Huan Chen, 2/21/2017

CC = gcc
FLAGS = -g -Wall #-DDEBUG

#compiler flags for GTK
GTKINC = `pkg-config --cflags gtk+-2.0`
GTKLIBS = `pkg-config --libs gtk+-2.0`

# test modules
TESTS = u m c # (utils, map, config)
T = u	# check memory leaks: make check T=m  # it will run ./m in valgrind

# to test utils.c, run ./u
# to test map.c, run ./m
# to run the all tests in one file, run ./c

PROJECT = c # configure map in one file: config.c

all: u m p Client Server taxi_client 


# test map.c
m: map_test.o utils.o map.o
	$(CC) $(FLAGS) $^ -o $@

# test utils.c
u: utils_test.o utils.o
	$(CC) $(FLAGS) $^ -o $@

TestRoute: Route.o config.o ServerSide.o
	gcc -Wall -ansi Route.o config.o ServerSide.o -o r -lm
	./r

p: profit.c config.h
	gcc -Wall -ansi profit.c -o p

Client: Client.c
	gcc Client.c -Wall -ansi -g -o Client

Route: Route.o
	gcc Route.c -Wall -ansi -c -o Route.o

Server.o: Server.c
	gcc Server.c -Wall -ansi $(GTKINC) -c -o Server.o

testServer.o: testServer.c
	gcc testServer.c -Wall -ansi $(GTKINC) -c -o testServer.o

taxi_client.o: taxi_client.c  
	gcc taxi_client.c -Wall -ansi $(GTKINC) -c -o taxi_client.o

Server: Server.o config.o ServerSide.o
	gcc Server.o config.o ServerSide.o -Wall -ansi $(GTKLIBS) -o Server


taxi_client: taxi_client.o config.o ServerSide.o
	gcc taxi_client.o config.o ServerSide.o -Wall -ansi $(GTKLIBS) -o taxi_client

test: testServer.o config.o ServerSide.o
	gcc testServer.o config.o ServerSide.o -Wall -ansi $(GTKLIBS) -o test 
	./test 3005 

ServerSide.o: ServerSide.c ServerSide.h config.h 
	gcc ServerSide.c -Wall -ansi -c -o ServerSide.o -lm


config.o: config.c config.h
	gcc -Wall -ansi -c config.c  -o config.o

# check memory leaks
check:
	valgrind --leak-check=full ./$(T) 

tar:
	tar czf ../Conf_map.tar.gz *.c *.h makefile *.map

clean:
	rm -f *.o $(PROJECT) $(TESTS) 
	rm -rf Client Server taxi_client p


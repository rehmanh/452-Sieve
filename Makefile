CPP=g++
CFLAGS=-std=c++11 -c -g

all: Client Server

Client: Client.o Sieve.o
	$(CPP) Client.o Sieve.o -o Client

Server: Server.o Sieve.o
	$(CPP) Server.o Sieve.o -o Server

Server.o: Server.cpp
	$(CPP) $(CFLAGS) Server.cpp

Client.o: Client.cpp
	$(CPP) $(CFLAGS) Client.cpp

Sieve.o: Sieve.cpp
	$(CPP) ${CFLAGS} Sieve.cpp

clean:
	rm -rf *.o Server Client

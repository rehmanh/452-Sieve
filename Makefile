CPP=g++
CFLAGS=-std=c++11 -c -g

run client: Client
	./Client

run server: Server
	./Server

Server: Server.cpp
	$(CPP) $(CFLAGS) Server.cpp -o Server

Client: Client.cpp
	$(CPP) $(CFLAGS) Client.cpp -o Client

clean:
	rm Client ; rm Server

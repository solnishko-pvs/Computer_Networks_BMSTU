all: clean server client

server: server.cpp _server.h server.h
	g++ -o server.out server.cpp _server.h server.h

client: client.cpp _client.h client.h
	g++ -o client.out client.cpp _client.h client.h

clean:
	rm -rf *.out

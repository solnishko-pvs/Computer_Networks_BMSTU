all: clean client server

client: client.c header.h 
	gcc -std=c11 -o client.out client.c -D_DEFAULT_SOURCE

server: server.c header.h 
	gcc -std=c11 -o server.out server.c

clean: 
	rm -rf *.out

CC = gcc
CFLAGS =

all: directories client

directories:
	mkdir -p achievements saved_games

client: client.o server.o
	$(CC) $(CFLAGS) -o client client.o server.o

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f client client.o server.o
	rm -rf achievements saved_games

.PHONY: clean directories

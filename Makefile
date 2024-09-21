CC = gcc
CFLAGS = -Wall

all: directories client server

directories:
	mkdir -p achievements saved_games

client: client.c
	$(CC) $(CFLAGS) -o client client.c

server: server.c
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f client server
	rm -rf achievements saved_games

.PHONY: clean directories

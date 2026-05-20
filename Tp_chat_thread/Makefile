CC=gcc
CFLAGS=-Wall -pthread

all: serveur client

serveur:
	$(CC) serveur_thread.c -o chatd $(CFLAGS)

client:
	$(CC) client_thread.c -o chatc $(CFLAGS)

clean:
	rm -f chatd chatc
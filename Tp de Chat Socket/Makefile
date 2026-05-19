CC=gcc
CFLAGS=-Wall

all: serveur client

serveur:
	$(CC) serveur.c -o chatd $(CFLAGS)

client:
	$(CC) client.c -o chatc $(CFLAGS)

clean:
	rm -f chatd chatc
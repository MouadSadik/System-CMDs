CC     = gcc
CFLAGS = -Wall -Wextra

all: ls mkdir rm

ls: ls.c
	$(CC) $(CFLAGS) -o ls ls.c

mkdir: mkdir.c
	$(CC) $(CFLAGS) -o mkdir mkdir.c

rm: rm.c
	$(CC) $(CFLAGS) -o rm rm.c

clean:
	rm -f ls mkdir rm
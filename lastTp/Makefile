CC     = gcc
CFLAGS = -Wall -Wextra

all: lancerApp gestionApps

lancerApp: lancerApp.c
	$(CC) $(CFLAGS) -o lancerApp lancerApp.c

gestionApps: gestionApps.c
	$(CC) $(CFLAGS) -o gestionApps gestionApps.c

clean:
	rm -f lancerApp gestionApps
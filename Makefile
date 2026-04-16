CC     = gcc
CFLAGS = -Wall -Wextra

all: attribut cdroit supprimer cgroup cowner

attribut: attribut.c
	$(CC) $(CFLAGS) -o attribut attribut.c

cdroit: cdroit.c
	$(CC) $(CFLAGS) -o cdroit cdroit.c

supprimer: supprimer.c
	$(CC) $(CFLAGS) -o supprimer supprimer.c

cgroup: cgroup.c
	$(CC) $(CFLAGS) -o cgroup cgroup.c

cowner: cowner.c
	$(CC) $(CFLAGS) -o cowner cowner.c

clean:
	rm -f attribut cdroit supprimer cgroup cowner

/*
 * ls [repertoire]
 * Liste le contenu d'un répertoire
 * Utilise : open(), getdents64(), write(), close()
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <dirent.h>

#define BUF_SIZE 4096

struct linux_dirent64 {
    unsigned long long d_ino;
    long long          d_off;
    unsigned short     d_reclen;
    unsigned char      d_type;
    char               d_name[];
};

int main(int argc, char *argv[]) {
    const char *path = (argc >= 2) ? argv[1] : ".";

    /* 1. Ouvrir le répertoire */
    int fd = open(path, O_RDONLY | O_DIRECTORY);
    if (fd < 0) {
        perror("ls: open");
        return 1;
    }

    char buf[BUF_SIZE];

    /* 2. Lire les entrées avec getdents64 */
    for (;;) {
        long n = syscall(SYS_getdents64, fd, buf, BUF_SIZE);
        if (n < 0) { perror("ls: getdents64"); close(fd); return 1; }
        if (n == 0) break;

        long pos = 0;
        while (pos < n) {
            struct linux_dirent64 *d = (struct linux_dirent64 *)(buf + pos);

            if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
                char type_char = '-';
                if      (d->d_type == DT_DIR) type_char = 'd';
                else if (d->d_type == DT_LNK) type_char = 'l';

                char line[512];
                int len = snprintf(line, sizeof(line), "[%c] %s\n",
                                   type_char, d->d_name);

                /* 3. Écriture vers stdout */
                write(STDOUT_FILENO, line, len);
            }
            pos += d->d_reclen;
        }
    }

    /* 4. Fermer le descripteur */
    close(fd);
    return 0;
}
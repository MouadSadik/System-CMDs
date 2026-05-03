/*
 * mkdir <repertoire>
 * Crée un nouveau répertoire
 * Utilise : mkdir(path, mode)
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define DIR_PERM 0755

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <repertoire>\n", argv[0]);
        return 1;
    }

    /* Créer le répertoire */
    if (mkdir(argv[1], DIR_PERM) == -1) {
        perror("mkdir");
        return 1;
    }

    printf("Répertoire '%s' créé (permissions: %04o)\n", argv[1], DIR_PERM);
    return 0;
}
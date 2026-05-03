/*
 * rm <fichier|repertoire>
 * Supprime un fichier régulier ou un répertoire vide
 * Utilise : stat(), unlink(), rmdir()
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier|repertoire>\n", argv[0]);
        return 1;
    }

    const char *chemin = argv[1];
    struct stat st;

    /* 1. Inspecter le type de l'entrée */
    if (stat(chemin, &st) == -1) {
        perror("rm: stat");
        return 1;
    }

    if (S_ISDIR(st.st_mode)) {
        /* 2a. Répertoire → rmdir (doit être vide) */
        if (rmdir(chemin) == -1) {
            perror("rm: rmdir");
            fprintf(stderr, "Conseil: le répertoire doit être vide\n");
            return 1;
        }
        printf("Répertoire '%s' supprimé.\n", chemin);
    } else {
        /* 2b. Fichier / lien symbolique → unlink */
        if (unlink(chemin) == -1) {
            perror("rm: unlink");
            return 1;
        }
        printf("Fichier '%s' supprimé.\n", chemin);
    }

    return 0;
}
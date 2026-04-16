#include <stdio.h>
#include <unistd.h>

/*
 * supprimer <nom_fichier>
 * Supprime un fichier en utilisant unlink(char* path)
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        return 1;
    }

    if (unlink(argv[1]) == -1) {
        perror("unlink");
        return 1;
    }

    printf("Fichier '%s' supprimé avec succès.\n", argv[1]);
    return 0;
}

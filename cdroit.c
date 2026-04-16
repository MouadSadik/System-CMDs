#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

/*
 * cdroit <fichier> <U|G|O> <droits_octal>
 *   U = User (propriétaire)
 *   G = Group
 *   O = Other (L = Other aussi accepté)
 *   E = Enlever tous les droits (mode 0)
 *
 * Exemples :
 *   cdroit test U 7     → rwx pour le propriétaire
 *   cdroit test G E     → aucun droit pour le groupe
 *   cdroit test L 5     → r-x pour others
 */

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <fichier> <U|G|O|L> <droit(0-7)|E>\n", argv[0]);
        fprintf(stderr, "  U = propriétaire, G = groupe, O/L = autres, E = enlever\n");
        return 1;
    }

    const char *fichier = argv[1];
    char  type   = argv[2][0];  // U, G, O, L
    char *droits = argv[3];

    // Récupérer les droits actuels
    struct stat st;
    if (stat(fichier, &st) == -1) {
        perror("stat");
        return 1;
    }

    mode_t mode = st.st_mode & 0777;  // garder uniquement les bits rwxrwxrwx

    // Valeur du nouveau droit
    int val;
    if (strcmp(droits, "E") == 0) {
        val = 0;
    } else {
        val = atoi(droits);
        if (val < 0 || val > 7) {
            fprintf(stderr, "Droit invalide : %s (attendu 0-7 ou E)\n", droits);
            return 1;
        }
    }

    // Appliquer selon le type d'utilisateur
    switch (type) {
        case 'U': case 'u':
            mode = (mode & ~S_IRWXU) | (val << 6);
            break;
        case 'G': case 'g':
            mode = (mode & ~S_IRWXG) | (val << 3);
            break;
        case 'O': case 'o':
        case 'L': case 'l':
            mode = (mode & ~S_IRWXO) | val;
            break;
        default:
            fprintf(stderr, "Type inconnu : %c (U=user, G=group, O/L=other)\n", type);
            return 1;
    }

    if (chmod(fichier, mode) == -1) {
        perror("chmod");
        return 1;
    }

    printf("Droits mis à jour pour '%s' : %o\n", fichier, mode);
    return 0;
}

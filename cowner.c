#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>

/*
 * cowner <fichier> <nouveau_proprietaire>
 * Change le propriétaire d'un fichier (nécessite sudo ou être root)
 * Utilise : chown(path, uid, gid) avec getpwnam()
 */

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <fichier> <nouveau_proprietaire>\n", argv[0]);
        return 1;
    }

    const char *fichier = argv[1];
    const char *user    = argv[2];

    // Récupérer le UID de l'utilisateur (getpwnam)
    struct passwd *pw = getpwnam(user);
    if (!pw) {
        fprintf(stderr, "cowner: utilisateur '%s' introuvable\n", user);
        return 1;
    }

    // Récupérer le GID actuel du fichier (pour ne pas le changer)
    struct stat st;
    if (stat(fichier, &st) == -1) {
        perror("stat");
        return 1;
    }

    // Changer uniquement le propriétaire (gid inchangé)
    if (chown(fichier, pw->pw_uid, st.st_gid) == -1) {
        perror("chown (cowner)");
        fprintf(stderr, "Conseil: relancez avec sudo si permission refusée\n");
        return 1;
    }

    printf("Propriétaire de '%s' changé en : %s (uid=%d)\n",
           fichier, user, pw->pw_uid);
    return 0;
}

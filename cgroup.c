#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <grp.h>

/*
 * cgroup <fichier> <nouveau_groupe>
 * Change le groupe d'un fichier (nécessite sudo ou être root)
 * Utilise : chown(path, uid, gid) avec getgrnam()
 */

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <fichier> <nouveau_groupe>\n", argv[0]);
        return 1;
    }

    const char *fichier = argv[1];
    const char *groupe  = argv[2];

    // Récupérer le GID du groupe
    struct group *gr = getgrnam(groupe);
    if (!gr) {
        fprintf(stderr, "cgroup: groupe '%s' introuvable\n", groupe);
        return 1;
    }

    // Récupérer le UID actuel du fichier (pour ne pas le changer)
    struct stat st;
    if (stat(fichier, &st) == -1) {
        perror("stat");
        return 1;
    }

    // Changer uniquement le groupe (uid inchangé)
    if (chown(fichier, st.st_uid, gr->gr_gid) == -1) {
        perror("chown (cgroup)");
        fprintf(stderr, "Conseil: relancez avec sudo si permission refusée\n");
        return 1;
    }

    printf("Groupe de '%s' changé en : %s (gid=%d)\n",
           fichier, groupe, gr->gr_gid);
    return 0;
}

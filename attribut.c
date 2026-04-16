#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>

void afficher_type(mode_t mode) {
    if (S_ISREG(mode))  printf("Type       : Fichier régulier\n");
    else if (S_ISDIR(mode))  printf("Type       : Répertoire\n");
    else if (S_ISLNK(mode))  printf("Type       : Lien symbolique\n");
    else if (S_ISCHR(mode))  printf("Type       : Fichier spécial caractère\n");
    else if (S_ISBLK(mode))  printf("Type       : Fichier spécial bloc\n");
    else if (S_ISFIFO(mode)) printf("Type       : FIFO (pipe nommé)\n");
    else if (S_ISSOCK(mode)) printf("Type       : Socket\n");
    else                     printf("Type       : Inconnu\n");
}

void afficher_droits(mode_t mode) {
    printf("Droits     : %c%c%c %c%c%c %c%c%c (%o)\n",
        (mode & S_IRUSR) ? 'r' : '-',
        (mode & S_IWUSR) ? 'w' : '-',
        (mode & S_IXUSR) ? 'x' : '-',
        (mode & S_IRGRP) ? 'r' : '-',
        (mode & S_IWGRP) ? 'w' : '-',
        (mode & S_IXGRP) ? 'x' : '-',
        (mode & S_IROTH) ? 'r' : '-',
        (mode & S_IWOTH) ? 'w' : '-',
        (mode & S_IXOTH) ? 'x' : '-',
        mode & 0777);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        return 1;
    }

    struct stat st;
    if (stat(argv[1], &st) == -1) {
        perror("stat");
        return 1;
    }

    printf("=== Attributs de : %s ===\n", argv[1]);

    // Type
    afficher_type(st.st_mode);

    // Droits
    afficher_droits(st.st_mode);

    // Taille
    printf("Taille     : %ld octets\n", (long)st.st_size);

    // Nombre de liens
    printf("Nb liens   : %ld\n", (long)st.st_nlink);

    // Propriétaire
    struct passwd *pw = getpwuid(st.st_uid);
    if (pw)
        printf("Propriétre : %s (uid=%d)\n", pw->pw_name, st.st_uid);
    else
        printf("Propriétre : uid=%d\n", st.st_uid);

    // Date de dernière modification
    char buf[64];
    struct tm *tm_info = localtime(&st.st_mtime);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Modifié le : %s\n", buf);

    // Date de création (ctime = dernier changement d'état sous Linux)
    tm_info = localtime(&st.st_ctime);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Changé le  : %s\n", buf);

    return 0;
}

/*
 *   Crée N processus fils pour CHAQUE programme listé.
 *   Exemple : ./lancerApp 3 ls gedit
 *     → 3 copies de "ls" + 3 copies de "gedit" = 6 fils au total.
 *
 * Compilation : gcc -Wall -o lancerApp lancerApp.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    /* ── Vérification des arguments ─────────────────────────────────── */
    if (argc < 3) {
        fprintf(stderr,
                "Usage : %s N prog1 [prog2 ...]\n"
                "  N     : nombre de copies à créer pour chaque programme\n"
                "  prog* : noms des programmes à lancer\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Conversion de N */
    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "Erreur : N doit être un entier strictement positif.\n");
        exit(EXIT_FAILURE);
    }

    int nb_progs = argc - 2;          /* nombre de programmes */
    char **progs = &argv[2];          /* tableau des noms de programmes */

    printf("[parent PID=%d] Lancement de %d copie(s) pour %d programme(s).\n\n",
           getpid(), N, nb_progs);

    /* ── Création des processus ──────────────────────────────────────── */
    /*
     * Pour chaque programme on crée N fils.
     * On stocke tous les PIDs pour pouvoir attendre proprement.
     */
    int total   = N * nb_progs;
    pid_t *pids = malloc(total * sizeof(pid_t));
    if (!pids) { perror("malloc"); exit(EXIT_FAILURE); }

    int idx = 0;
    for (int p = 0; p < nb_progs; p++) {
        for (int i = 0; i < N; i++) {
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork");
                /* On continue malgré l'erreur pour ne pas bloquer */

            } else if (pid == 0) {
                /* ── Fils : remplace son image par le programme cible ── */
                printf("[fils  PID=%d] Exécution de '%s' (copie %d/%d)\n",
                       getpid(), progs[p], i + 1, N);
                fflush(stdout);

                execlp(progs[p], progs[p], (char *)NULL);

                /* Si execlp échoue */
                perror(progs[p]);
                exit(EXIT_FAILURE);

            } else {
                /* ── Parent : enregistre le PID du fils ─────────────── */
                printf("[parent] Fils créé : PID=%d pour '%s' (copie %d/%d)\n",
                       pid, progs[p], i + 1, N);
                pids[idx++] = pid;
            }
        }
    }

    /* ── Attente de tous les fils ────────────────────────────────────── */
    printf("\n[parent] Attente de la fin des %d fils...\n", idx);
    for (int i = 0; i < idx; i++) {
        int status;
        pid_t ended = waitpid(pids[i], &status, 0);
        if (ended == -1) {
            perror("waitpid");
        } else if (WIFEXITED(status)) {
            printf("[parent] Fils PID=%d terminé (code=%d)\n",
                   ended, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[parent] Fils PID=%d tué par signal %d\n",
                   ended, WTERMSIG(status));
        }
    }

    printf("\n[parent] Tous les fils ont terminé. Fin du programme.\n");
    free(pids);
    return EXIT_SUCCESS;
}
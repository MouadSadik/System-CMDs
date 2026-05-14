/*
 * gestionApps.c — Programmation Système
 *
 * Lit une liste d'applications (depuis un fichier ou stdin),
 * construit une liste chaînée, lance chaque application,
 * attend la fin de toutes, puis détruit la liste.
 *
 * Usage :
 *   ./gestionApps apps.txt         ← lecture depuis un fichier
 *   ./gestionApps                  ← lecture depuis stdin (Ctrl+D pour terminer)
 *
 * Format du fichier / stdin : un nom d'application par ligne.
 *   ls
 *   date
 *   pwd
 *
 * Compilation : gcc -Wall -o gestionApps gestionApps.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* ═══════════════════════════════════════════════════════════════════════
 *  Structure de la liste chaînée
 * ═══════════════════════════════════════════════════════════════════════ */
typedef struct Noeud {
    char          *nom;      /* nom du programme */
    pid_t          pid;      /* PID du fils (0 avant le fork) */
    struct Noeud  *suivant;
} Noeud;

/* ── Créer un nouveau nœud ──────────────────────────────────────────── */
Noeud *creer_noeud(const char *nom)
{
    Noeud *n = malloc(sizeof(Noeud));
    if (!n) { perror("malloc noeud"); exit(EXIT_FAILURE); }

    n->nom     = strdup(nom);
    if (!n->nom) { perror("strdup"); exit(EXIT_FAILURE); }
    n->pid     = 0;
    n->suivant = NULL;
    return n;
}

/* ── Ajouter en queue ───────────────────────────────────────────────── */
void ajouter(Noeud **tete, const char *nom)
{
    Noeud *nouveau = creer_noeud(nom);
    if (*tete == NULL) {
        *tete = nouveau;
        return;
    }
    Noeud *courant = *tete;
    while (courant->suivant) courant = courant->suivant;
    courant->suivant = nouveau;
}

/* ── Afficher la liste ──────────────────────────────────────────────── */
void afficher_liste(const Noeud *tete)
{
    int i = 1;
    printf("┌─────────────────────────────────────────┐\n");
    printf("│        Liste des applications           │\n");
    printf("├────┬────────────────────┬───────────────┤\n");
    printf("│ N° │ Programme          │ PID           │\n");
    printf("├────┼────────────────────┼───────────────┤\n");
    for (const Noeud *n = tete; n; n = n->suivant, i++) {
        printf("│ %-2d │ %-18s │ %-13d │\n",
               i, n->nom, (int)n->pid);
    }
    printf("└────┴────────────────────┴───────────────┘\n");
}

/* ── Lancer toutes les applications ────────────────────────────────── */
void lancer_applications(Noeud *tete)
{
    printf("\n[parent PID=%d] Lancement des applications...\n", getpid());

    for (Noeud *n = tete; n; n = n->suivant) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");

        } else if (pid == 0) {
            /* ── Fils ── */
            printf("[fils  PID=%d] Exécution de '%s'\n", getpid(), n->nom);
            fflush(stdout);
            execlp(n->nom, n->nom, (char *)NULL);
            perror(n->nom);          /* si execlp échoue */
            exit(EXIT_FAILURE);

        } else {
            /* ── Parent ── */
            n->pid = pid;
            printf("[parent] '%s' lancé avec PID=%d\n", n->nom, pid);
        }
    }
}

/* ── Attendre la fin de tous les fils ──────────────────────────────── */
void attendre_applications(const Noeud *tete)
{
    printf("\n[parent] Attente de la fin de tous les processus...\n");

    for (const Noeud *n = tete; n; n = n->suivant) {
        if (n->pid <= 0) continue;

        int status;
        pid_t ended = waitpid(n->pid, &status, 0);

        if (ended == -1) {
            perror("waitpid");
        } else if (WIFEXITED(status)) {
            printf("[parent] '%s' (PID=%d) terminé normalement (code=%d)\n",
                   n->nom, ended, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[parent] '%s' (PID=%d) tué par signal %d\n",
                   n->nom, ended, WTERMSIG(status));
        }
    }
}

/* ── Détruire la liste chaînée (libération mémoire) ────────────────── */
void detruire_liste(Noeud **tete)
{
    printf("\n[mémoire] Destruction de la liste chaînée...\n");
    Noeud *courant = *tete;
    while (courant) {
        Noeud *suivant = courant->suivant;
        printf("[mémoire] Libération du nœud '%s'\n", courant->nom);
        free(courant->nom);
        free(courant);
        courant = suivant;
    }
    *tete = NULL;
    printf("[mémoire] Liste détruite.\n");
}

/* ═══════════════════════════════════════════════════════════════════════
 *  main
 * ═══════════════════════════════════════════════════════════════════════ */
int main(int argc, char *argv[])
{
    FILE  *source = NULL;
    Noeud *liste  = NULL;
    char   ligne[256];

    /* ── Ouverture de la source (fichier ou stdin) ─────────────────── */
    if (argc >= 2) {
        source = fopen(argv[1], "r");
        if (!source) {
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }
        printf("[info] Lecture des applications depuis '%s'\n\n", argv[1]);
    } else {
        source = stdin;
        printf("[info] Entrez les noms d'applications (un par ligne, Ctrl+D pour terminer) :\n");
    }

    /* ── Construction de la liste chaînée ────────────────────────────── */
    while (fgets(ligne, sizeof(ligne), source)) {
        /* Supprimer le '\n' de fin */
        ligne[strcspn(ligne, "\n")] = '\0';

        /* Ignorer les lignes vides ou commentaires (#) */
        if (ligne[0] == '\0' || ligne[0] == '#') continue;

        ajouter(&liste, ligne);
        printf("[liste] Ajout de '%s'\n", ligne);
    }

    if (source != stdin) fclose(source);

    if (!liste) {
        fprintf(stderr, "Erreur : aucune application dans la liste.\n");
        exit(EXIT_FAILURE);
    }

    /* ── Affichage initial ───────────────────────────────────────────── */
    printf("\n");
    afficher_liste(liste);

    /* ── Lancement ───────────────────────────────────────────────────── */
    lancer_applications(liste);

    /* ── Affichage avec PIDs ─────────────────────────────────────────── */
    printf("\n");
    afficher_liste(liste);

    /* ── Attente ─────────────────────────────────────────────────────── */
    attendre_applications(liste);

    /* ── Destruction ─────────────────────────────────────────────────── */
    detruire_liste(&liste);

    printf("\n[parent] Programme terminé proprement.\n");
    return EXIT_SUCCESS;
}
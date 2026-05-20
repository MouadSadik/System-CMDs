#include "common.h"

void *client_handler(void *arg)
{
    int clientSocket = *((int *)arg);

    char pseudo[50];
    char buffer[BUFFER_SIZE];

    memset(pseudo, 0, sizeof(pseudo));
    memset(buffer, 0, BUFFER_SIZE);

    recv(clientSocket, pseudo, sizeof(pseudo), 0);

    printf("Client connecté : %s\n", pseudo);

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        int bytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);

        if (bytes <= 0)
        {
            printf("%s déconnecté.\n", pseudo);
            break;
        }

        if (strcmp(buffer, "quitter") == 0)
        {
            printf("%s a quitté.\n", pseudo);
            break;
        }

        printf("%s : %s\n", pseudo, buffer);

        send(clientSocket, buffer, BUFFER_SIZE, 0);
    }

    close(clientSocket);
    free(arg);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int serveurSocket;

    struct sockaddr_in adresseServeur;
    struct sockaddr_in adresseClient;

    socklen_t tailleClient = sizeof(adresseClient);

    pthread_t thread_id;

    if (argc != 2)
    {
        printf("Usage : %s <PORT>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    serveurSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serveurSocket < 0)
    {
        perror("Erreur socket");
        return 1;
    }

    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_addr.s_addr = INADDR_ANY;
    adresseServeur.sin_port = htons(port);

    if (bind(serveurSocket,
             (struct sockaddr *)&adresseServeur,
             sizeof(adresseServeur)) < 0)
    {
        perror("Erreur bind");
        return 1;
    }

    if (listen(serveurSocket, MAX_CLIENTS) < 0)
    {
        perror("Erreur listen");
        return 1;
    }

    printf("Serveur multi-thread démarré sur le port %d\n", port);

    while (1)
    {
        int *clientSocket = malloc(sizeof(int));

        *clientSocket = accept(
            serveurSocket,
            (struct sockaddr *)&adresseClient,
            &tailleClient);

        if (*clientSocket < 0)
        {
            perror("Erreur accept");
            free(clientSocket);
            continue;
        }

        printf("Nouvelle connexion : %s : %d\n",
               inet_ntoa(adresseClient.sin_addr),
               ntohs(adresseClient.sin_port));

        pthread_create(
            &thread_id,
            NULL,
            client_handler,
            (void *)clientSocket);

        pthread_detach(thread_id);
    }

    close(serveurSocket);

    return 0;
}
#include "common.h"

void *recevoir_messages(void *arg)
{
    int socketClient = *((int *)arg);

    char buffer[BUFFER_SIZE];

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        int bytes = recv(socketClient, buffer, BUFFER_SIZE, 0);

        if (bytes <= 0)
        {
            printf("\nConnexion fermée.\n");
            break;
        }

        printf("\nMessage reçu : %s\n", buffer);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int socketClient;

    struct sockaddr_in adresseServeur;

    pthread_t threadReception;

    char pseudo[50];
    char buffer[BUFFER_SIZE];

    if (argc != 3)
    {
        printf("Usage : %s <IP_SERVEUR> <PORT>\n", argv[0]);
        return 1;
    }

    char *ipServeur = argv[1];
    int port = atoi(argv[2]);

    socketClient = socket(AF_INET, SOCK_STREAM, 0);

    if (socketClient < 0)
    {
        perror("Erreur socket");
        return 1;
    }

    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(port);

    inet_pton(AF_INET,
              ipServeur,
              &adresseServeur.sin_addr);

    if (connect(socketClient,
                (struct sockaddr *)&adresseServeur,
                sizeof(adresseServeur)) < 0)
    {
        perror("Erreur connexion");
        return 1;
    }

    printf("Connecté au serveur.\n");

    printf("Entrer votre pseudonyme : ");
    fgets(pseudo, sizeof(pseudo), stdin);

    pseudo[strcspn(pseudo, "\n")] = '\0';

    send(socketClient, pseudo, sizeof(pseudo), 0);

    pthread_create(
        &threadReception,
        NULL,
        recevoir_messages,
        (void *)&socketClient);

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        send(socketClient, buffer, BUFFER_SIZE, 0);

        if (strcmp(buffer, "quitter") == 0)
        {
            break;
        }
    }

    close(socketClient);

    return 0;
}
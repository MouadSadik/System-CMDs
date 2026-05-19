#include "common.h"

void chats(int clientSocket)
{
    char pseudoLocal[50];
    char pseudoDistant[50];

    char messageEnvoye[BUFFER_SIZE];
    char messageRecu[BUFFER_SIZE];

    memset(messageEnvoye, 0, BUFFER_SIZE);
    memset(messageRecu, 0, BUFFER_SIZE);

    printf("Entrer votre pseudonyme : ");
    fgets(pseudoLocal, sizeof(pseudoLocal), stdin);

    pseudoLocal[strcspn(pseudoLocal, "\n")] = '\0';

    send(clientSocket, pseudoLocal, sizeof(pseudoLocal), 0);

    recv(clientSocket, pseudoDistant, sizeof(pseudoDistant), 0);

    printf("Pseudo distant : %s\n", pseudoDistant);

    while (1)
    {
        memset(messageEnvoye, 0, BUFFER_SIZE);

        printf("%s : ", pseudoLocal);
        fgets(messageEnvoye, BUFFER_SIZE, stdin);

        messageEnvoye[strcspn(messageEnvoye, "\n")] = '\0';

        send(clientSocket, messageEnvoye, BUFFER_SIZE, 0);

        if (strcmp(messageEnvoye, "quitter") == 0)
        {
            break;
        }

        memset(messageRecu, 0, BUFFER_SIZE);

        recv(clientSocket, messageRecu, BUFFER_SIZE, 0);

        if (strcmp(messageRecu, "quitter") == 0)
        {
            printf("%s a quitté la conversation.\n", pseudoDistant);
            break;
        }

        printf("%s : %s\n", pseudoDistant, messageRecu);
    }

    close(clientSocket);
}

int main(int argc, char *argv[])
{
    int serveurSocket;
    int clientSocket;

    struct sockaddr_in adresseLocale;
    struct sockaddr_in adresseClient;

    socklen_t tailleClient = sizeof(adresseClient);

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    serveurSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serveurSocket < 0)
    {
        perror("Erreur socket");
        return 1;
    }

    adresseLocale.sin_family = AF_INET;
    adresseLocale.sin_addr.s_addr = INADDR_ANY;
    adresseLocale.sin_port = htons(port);

    if (bind(serveurSocket,
             (struct sockaddr *)&adresseLocale,
             sizeof(adresseLocale)) < 0)
    {
        perror("Erreur bind");
        return 1;
    }

    if (listen(serveurSocket, MAX_CLIENTS) < 0)
    {
        perror("Erreur listen");
        return 1;
    }

    printf("Serveur en attente sur le port %d...\n", port);

    while (1)
    {
        clientSocket = accept(
            serveurSocket,
            (struct sockaddr *)&adresseClient,
            &tailleClient);

        if (clientSocket < 0)
        {
            perror("Erreur accept");
            continue;
        }

        printf("\nClient connecté : %s : %d\n",
               inet_ntoa(adresseClient.sin_addr),
               ntohs(adresseClient.sin_port));

        chats(clientSocket);
    }

    close(serveurSocket);

    return 0;
}
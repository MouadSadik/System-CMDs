#include "common.h"

void chats(int socketClient)
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

    recv(socketClient, pseudoDistant, sizeof(pseudoDistant), 0);

    send(socketClient, pseudoLocal, sizeof(pseudoLocal), 0);

    printf("Pseudo distant : %s\n", pseudoDistant);

    while (1)
    {
        memset(messageRecu, 0, BUFFER_SIZE);

        recv(socketClient, messageRecu, BUFFER_SIZE, 0);

        if (strcmp(messageRecu, "quitter") == 0)
        {
            printf("%s a quitté la conversation.\n", pseudoDistant);
            break;
        }

        printf("%s : %s\n", pseudoDistant, messageRecu);

        memset(messageEnvoye, 0, BUFFER_SIZE);

        printf("%s : ", pseudoLocal);
        fgets(messageEnvoye, BUFFER_SIZE, stdin);

        messageEnvoye[strcspn(messageEnvoye, "\n")] = '\0';

        send(socketClient, messageEnvoye, BUFFER_SIZE, 0);

        if (strcmp(messageEnvoye, "quitter") == 0)
        {
            break;
        }
    }

    close(socketClient);
}

int main(int argc, char *argv[])
{
    int socketClient;

    struct sockaddr_in adresseServeur;

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

    chats(socketClient);

    return 0;
}
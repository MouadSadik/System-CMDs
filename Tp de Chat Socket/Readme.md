# TP Programmation Système — Chat TCP Client/Serveur en C

## Description

Ce projet est une application simple de chat de type talkie-walkie en langage C, utilisant les sockets TCP/IP sous Linux.

Le système est composé de deux programmes :

- Un serveur (`chatd`)
- Un client (`chatc`)

Les deux programmes communiquent via le protocole TCP.

### Fonctionnalités

- Connexion client/serveur
- Échange de pseudonymes
- Envoi et réception de messages
- Fermeture avec la commande `quitter`

---

## Structure du projet

```bash
chat-tcp/
├── serveur.c
├── client.c
├── common.h
├── Makefile
└── README.md
```

## Technologies utilisées

- Langage C
- Sockets TCP/IP
- Linux / Unix
- GCC

## Bibliothèques utilisées

- `stdio.h`
- `stdlib.h`
- `string.h`
- `unistd.h`
- `arpa/inet.h`
- `sys/socket.h`
- `sys/types.h`
- `netinet/in.h`

## Compilation du projet

### Méthode 1 — Avec Makefile

Dans le dossier du projet :

```bash
make
```

Cela génère :

- `chatd`
- `chatc`

### Méthode 2 — Compilation manuelle

#### Compilation serveur

```bash
gcc serveur.c -o chatd -Wall
```

#### Compilation client

```bash
gcc client.c -o chatc -Wall
```

## Exécution du projet

### 1. Lancer le serveur

Ouvrir un terminal :

```bash
./chatd 5000
```

`5000` représente le numéro de port.

Le serveur attend maintenant les connexions.

### 2. Lancer le client

Ouvrir un second terminal :

```bash
Ctrl + Alt + F3
./chatc 127.0.0.1 5000
Ctrl + Alt + F1
```

Arguments :

- `127.0.0.1` → adresse IP du serveur
- `5000` → port du serveur

## Exemple d'exécution

### Serveur

```text
Serveur en attente sur le port 5000...
Client connecté : 127.0.0.1 : 52344
Entrer votre pseudonyme :
Serveur
Pseudo distant : Client
Serveur : Bonjour
Client : Salut
```

### Client

```text
Connecté au serveur.
Entrer votre pseudonyme :
Client
Pseudo distant : Serveur
Serveur : Bonjour
Client : Salut
```

## Quitter la conversation

Pour fermer la connexion :

```text
quitter
```

La socket sera automatiquement fermée.

## Explication des fonctions utilisées

| Fonction | Description |
|---|---|
| `socket()` | Création de la socket |
| `bind()` | Association IP + port |
| `listen()` | Mise en attente des connexions |
| `accept()` | Acceptation d’un client |
| `connect()` | Connexion au serveur |
| `send()` | Envoi des données |
| `recv()` | Réception des données |
| `close()` | Fermeture de la socket |

## Fonctionnement général

### Serveur

- Création de la socket
- Configuration de l’adresse locale
- Association IP/port avec `bind()`
- Mise en écoute avec `listen()`
- Acceptation du client avec `accept()`
- Communication avec le client

### Client

- Création de la socket
- Configuration de l’adresse du serveur
- Connexion avec `connect()`
- Communication avec le serveur

## Structure réseau utilisée

```c
struct sockaddr_in
{
    short sin_family;
    unsigned short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};
```

## Concepts pratiqués

Ce TP permet de pratiquer :

- Les sockets TCP/IP
- La programmation système
- Les appels système Linux
- La communication réseau
- L’architecture client/serveur
- La communication temps réel

## Auteur

TP réalisé dans le cadre du module :

**Programmation Système — Langage C**

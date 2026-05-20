1. Compilation

```bash
make
```

2. Exécution

Serveur
```bash
./chatd 5000
```

Client 1
```bash
./chatc 127.0.0.1 5000
```

Client 2
```bash
./chatc 127.0.0.1 5000
```

3. Ce que les threads ajoutent

Avant : 1 client à la fois

Maintenant : plusieurs clients simultanément

Chaque client possède :
- son propre thread,
- sa propre communication,
- sa propre exécution parallèle.

8. Fonctions Threads utilisées

- Fonction	Rôle
- pthread_create()	Création d’un thread
- pthread_detach()	Libération automatique
- pthread_exit()	Fin du thread

9. Concepts pratiqués

Ce TP pratique :

- sockets TCP/IP,
- programmation système,
- multi-threading,
- concurrence,
- communication réseau,
- parallélisme en C,
- POSIX Threads (pthread).
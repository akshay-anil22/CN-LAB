#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int sockfd, newsock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    char buffer[1024] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Waiting for connection...\n");
    newsock = accept(sockfd, (struct sockaddr *)&client, &len);

    srand(time(0));

    while (1) {
        read(newsock, buffer, sizeof(buffer));

        if (strlen(buffer) == 0)
            break;

        printf("Received packet: %s\n", buffer);

        if (rand() % 100 < 70) {
            send(newsock, "ACK", 3, 0);
            printf("ACK sent\n");
        } else {
            printf("ACK lost\n");
        }

        memset(buffer, 0, sizeof(buffer));
    }

    close(newsock);
    close(sockfd);

    return 0;
}
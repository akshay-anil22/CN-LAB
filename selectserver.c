#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    int frame, ack;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Waiting...\n");
    newsock = accept(sockfd, (struct sockaddr *)&client, &len);

    while (1) {
        read(newsock, &frame, sizeof(frame));

        if (frame == -1)
            break;

        printf("Received frame %d\n", frame);

        printf("Send ACK? (1=yes 0=lost): ");
        scanf("%d", &ack);

        if (ack == 1)
            write(newsock, &frame, sizeof(frame));
        else {
            int nack = -1;
            write(newsock, &nack, sizeof(nack));
        }
    }

    close(newsock);
    close(sockfd);

    return 0;
}
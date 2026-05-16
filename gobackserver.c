#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int sockfd, newsock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    int frame;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Waiting for connection...\n");
    newsock = accept(sockfd, (struct sockaddr *)&client, &len);

    srand(time(0));

    // Add this variable
    int expected = 1;

        while (1) {
            read(newsock, &frame, sizeof(frame));
            if (frame == -1) break;

            printf("Received packet %d\n", frame);

            if (frame == expected) {          // Only ACK in-order frames
                if (rand() % 100 < 70) {
                    write(newsock, &frame, sizeof(frame));
                    printf("ACK sent for %d\n", frame);
                    expected++;               // Advance only on in-order ACK
                } else {
                    printf("ACK lost for %d\n", frame);
                }
            } else {
                printf("Out-of-order packet %d, expected %d — discarded\n", frame, expected);
                // Send ACK for last successfully received in-order packet
                int last_ack = expected - 1;
                if (last_ack >= 1)
                    write(newsock, &last_ack, sizeof(last_ack));
            }
        }

    close(newsock);
    close(sockfd);

    return 0;
}
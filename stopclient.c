#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

int main() {
    int sockfd;
    struct sockaddr_in server;
    char buffer[1024] = {0};
    char packet[20];
    struct timeval tv = {3, 0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int i = 1;

    while (i <= 5) {
        sprintf(packet, "%d", i);

        send(sockfd, packet, strlen(packet), 0);
        printf("Sent packet %d\n", i);

        read(sockfd, buffer, sizeof(buffer));

        if (strcmp(buffer, "ACK") == 0) {
            printf("ACK received\n");
            i++;
        } else {
            printf("Timeout... resend\n");
        }

        memset(buffer, 0, sizeof(buffer));
    }

    close(sockfd);

    return 0;
}
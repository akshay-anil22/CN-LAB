#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

int main() {
    int sockfd;
    struct sockaddr_in server;
    struct timeval tv = {3, 0};
    int base = 1, next = 1;
    int window = 4;
    int ack;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (base <= 10) {
        while (next < base + window && next <= 10) {
            write(sockfd, &next, sizeof(next));
            printf("Sent packet %d\n", next);
            next++;
        }

        if (read(sockfd, &ack, sizeof(ack)) > 0) {
              if(ack >= base) {

                 printf("ACK received for %d\n", ack);
                 base = ack + 1;
        }
        } else {
            printf("Timeout... resend from %d\n", base);
            next = base;
        }
    }

    int end = -1;
    write(sockfd, &end, sizeof(end));

    close(sockfd);

    return 0;
}
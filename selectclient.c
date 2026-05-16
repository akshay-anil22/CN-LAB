#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server;
    int total, window;
    int ack[50] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Enter total frames: ");
    scanf("%d", &total);

    printf("Enter window size: ");
    scanf("%d", &window);

    int base = 0;

    while (base < total) {
        for (int i = base; i < base + window && i < total; i++) {
            if (ack[i] == 0) {
                printf("Sending frame %d\n", i);
                write(sockfd, &i, sizeof(i));
            }
        }

        for (int i = base; i < base + window && i < total; i++) {
            if (ack[i] == 0) {
                int res;
                read(sockfd, &res, sizeof(res));

                if (res == i) {
                    printf("ACK received for %d\n", i);
                    ack[i] = 1;
                } else {
                    printf("Frame %d lost\n", i);
                }
            }
        }

        while (base < total && ack[base] == 1)
            base++;
    }

    int end = -1;
    write(sockfd, &end, sizeof(end));

    close(sockfd);

    return 0;
}
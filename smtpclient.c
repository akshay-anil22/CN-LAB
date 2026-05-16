#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2525

int main() {

    int sockfd;
    struct sockaddr_in server;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    strcpy(buffer, "HELO localhost\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    strcpy(buffer, "MAIL FROM:<sender@example.com>\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    strcpy(buffer, "RCPT TO:<receiver@example.com>\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    strcpy(buffer, "DATA\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    printf("Enter email message (end with . ):\n");

    while (1) {

        fgets(buffer, sizeof(buffer), stdin);

        send(sockfd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, ".\n") == 0)
            break;
    }

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    printf("Enter QUIT to exit:\n");
    fgets(buffer, sizeof(buffer), stdin);

    send(sockfd, buffer, strlen(buffer), 0);

    recv(sockfd, buffer, sizeof(buffer), 0);

    printf("%s", buffer);

    close(sockfd);

    return 0;
}
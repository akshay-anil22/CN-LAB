#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server;
    char filename[1024], buffer[1024] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Enter filename: ");
    scanf("%s", filename);

    send(sockfd, filename, strlen(filename), 0);

    printf("File content:\n");

    while (read(sockfd, buffer, sizeof(buffer)) > 0) {
        printf("%s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    close(sockfd);

    return 0;
}
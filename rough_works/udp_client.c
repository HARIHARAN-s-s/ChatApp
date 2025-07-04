#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER "127.0.0.1" // Change to server IP if needed
#define PORT 8888
#define BUFLEN 512

int main() {
    struct sockaddr_in si_other;
    int sockfd;
    char buf[BUFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset((char *)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER, &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    printf("Enter messages to send to %s:%d\n", SERVER, PORT);
    while (fgets(buf, BUFLEN, stdin) != NULL) {
        if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&si_other, sizeof(si_other)) < 0) {
            perror("sendto");
            break;
        }
    }

    close(sockfd);
    return 0;
}

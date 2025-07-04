#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888
#define BUFLEN 512

int main() {
    struct sockaddr_in si_me, si_other;
    int sockfd, slen = sizeof(si_other);
    char buf[BUFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset((char *)&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&si_me, sizeof(si_me)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    printf("UDP server listening on port %d...\n", PORT);

    while (1) {
        int recv_len = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&si_other, (socklen_t *)&slen);
        if (recv_len < 0) {
            perror("recvfrom");
            break;
        }
        buf[recv_len] = '\0';
        printf("Received from %s:%d: %s\n",
               inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
    }

    close(sockfd);
    return 0;
}

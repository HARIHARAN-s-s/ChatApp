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

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // Set up the server address structure
    memset((char *)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    // Convert server IP address from text to binary form
    if (inet_aton(SERVER, &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    printf("Enter messages to send to %s:%d\n", SERVER, PORT);

    // Read lines from stdin and send them to the server
    while (fgets(buf, BUFLEN, stdin) != NULL) {
        // Send the message
        if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&si_other, sizeof(si_other)) < 0) {
            perror("sendto");
            break;
        }
    }

    // Close the socket when done
    close(sockfd);
    return 0;
}

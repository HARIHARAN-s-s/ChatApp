#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFLEN 512

typedef struct {
    int sockfd;
    struct sockaddr_in peer_addr;
    socklen_t addr_len;
} chat_args_t;

void *recv_thread(void *arg) {
    chat_args_t *chat = (chat_args_t *)arg;
    char buf[BUFLEN];
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    while (1) {
        int recv_len = recvfrom(chat->sockfd, buf, BUFLEN-1, 0,
                                (struct sockaddr *)&from_addr, &from_len);
        if (recv_len < 0) {
            perror("recvfrom");
            break;
        }
        buf[recv_len] = '\0';
        printf("\n[Peer %s:%d]: %s", inet_ntoa(from_addr.sin_addr),
               ntohs(from_addr.sin_port), buf);
        fflush(stdout);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <your_port> <peer_ip> <peer_port>\n", argv[0]);
        return 1;
    }

    int my_port = atoi(argv[1]);
    char *peer_ip = argv[2];
    int peer_port = atoi(argv[3]);

    int sockfd;
    struct sockaddr_in my_addr, peer_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(my_port);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(peer_port);
    if (inet_aton(peer_ip, &peer_addr.sin_addr) == 0) {
        fprintf(stderr, "Invalid peer IP address\n");
        close(sockfd);
        return 1;
    }

    chat_args_t chat;
    chat.sockfd = sockfd;
    chat.peer_addr = peer_addr;
    chat.addr_len = sizeof(peer_addr);

    pthread_t tid;
    if (pthread_create(&tid, NULL, recv_thread, &chat) != 0) {
        perror("pthread_create");
        close(sockfd);
        return 1;
    }

    char buf[BUFLEN];
    while (fgets(buf, BUFLEN, stdin) != NULL) {
        if (sendto(sockfd, buf, strlen(buf), 0,
                   (struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0) {
            perror("sendto");
            break;
        }
    }

    close(sockfd);
    return 0;
}

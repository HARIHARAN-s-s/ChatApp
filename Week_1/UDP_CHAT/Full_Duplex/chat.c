#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFLEN 512
#define CIRC_BUF_SIZE 10

// --- State machine states ---
typedef enum {
    STATE_IDLE,
    STATE_RECEIVING,
    STATE_SENDING
} chat_state_t;

// --- Circular buffer for storing received messages ---
typedef struct {
    char data[CIRC_BUF_SIZE][BUFLEN];
    int head;
    int tail;
    int count;
    pthread_mutex_t lock;
} circ_buf_t;

// Initialize circular buffer
void circbuf_init(circ_buf_t *buf) {
    buf->head = buf->tail = buf->count = 0;
    pthread_mutex_init(&buf->lock, NULL); // Don't forget to init the lock!
}

// Put a message in the circular buffer
int circbuf_push(circ_buf_t *buf, const char *msg) {
    pthread_mutex_lock(&buf->lock);
    if (buf->count == CIRC_BUF_SIZE) { // Buffer is full, can't push
        pthread_mutex_unlock(&buf->lock);
        return -1;
    }
    strncpy(buf->data[buf->tail], msg, BUFLEN);
    buf->tail = (buf->tail + 1) % CIRC_BUF_SIZE;
    buf->count++;
    pthread_mutex_unlock(&buf->lock);
    return 0;
}

// Get a message from the circular buffer
int circbuf_pop(circ_buf_t *buf, char *out) {
    pthread_mutex_lock(&buf->lock);
    if (buf->count == 0) { // Buffer is empty, nothing to pop
        pthread_mutex_unlock(&buf->lock);
        return -1;
    }
    strncpy(out, buf->data[buf->head], BUFLEN);
    buf->head = (buf->head + 1) % CIRC_BUF_SIZE;
    buf->count--;
    pthread_mutex_unlock(&buf->lock);
    return 0;
}

// --- Simple linked list queue for outgoing messages ---
typedef struct msg_node {
    char msg[BUFLEN];
    struct msg_node *next;
} msg_node_t;

typedef struct {
    msg_node_t *front, *rear;
    pthread_mutex_t lock;
} msg_queue_t;

// Initialize message queue
void msgqueue_init(msg_queue_t *q) {
    q->front = q->rear = NULL;
    pthread_mutex_init(&q->lock, NULL);
}

// Add a message to the queue
void msgqueue_enqueue(msg_queue_t *q, const char *msg) {
    msg_node_t *new_node = malloc(sizeof(msg_node_t));
    strcpy(new_node->msg, msg);
    new_node->next = NULL;

    pthread_mutex_lock(&q->lock);
    if (q->rear) q->rear->next = new_node;
    else q->front = new_node;
    q->rear = new_node;
    pthread_mutex_unlock(&q->lock);
}

// Remove a message from the front of the queue
int msgqueue_dequeue(msg_queue_t *q, char *out) {
    pthread_mutex_lock(&q->lock);
    if (!q->front) {
        pthread_mutex_unlock(&q->lock);
        return -1; // Nothing to dequeue
    }

    msg_node_t *tmp = q->front;
    strcpy(out, tmp->msg);
    q->front = tmp->next;
    if (!q->front) q->rear = NULL; // Queue is empty now
    free(tmp);
    pthread_mutex_unlock(&q->lock);
    return 0;
}

// --- Global variables used for sockets and buffers ---
int sockfd;
struct sockaddr_in peer_addr;
circ_buf_t recv_buf;
msg_queue_t send_queue;

// --- Thread: receives UDP packets and stores them in the circular buffer ---
void *receiver(void *arg) {
    char buf[BUFLEN];
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);
    while (1) {
        int recv_len = recvfrom(sockfd, buf, BUFLEN - 1, 0,
                                (struct sockaddr *)&from_addr, &from_len);
        if (recv_len > 0) {
            buf[recv_len] = '\0'; // Null-terminate, just in case!
            circbuf_push(&recv_buf, buf);
        }
    }
    return NULL;
}

// --- Thread: gets user input and puts it in the send queue ---
void *input_handler(void *arg) {
    char buf[BUFLEN];
    while (fgets(buf, BUFLEN, stdin)) {
        msgqueue_enqueue(&send_queue, buf);
    }
    return NULL;
}

// --- Main function: setup, threads, and state machine loop ---
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <your_port> <peer_ip> <peer_port>\n", argv[0]);
        return 1;
    }

    int my_port = atoi(argv[1]);
    char *peer_ip = argv[2];
    int peer_port = atoi(argv[3]);

    struct sockaddr_in my_addr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(my_port);

    // Bind socket to our port
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    // Set up peer info (who we're chatting with)
    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(peer_port);
    inet_aton(peer_ip, &peer_addr.sin_addr);

    circbuf_init(&recv_buf);
    msgqueue_init(&send_queue);

    // Start the receiver and input handler threads
    pthread_t recv_tid, input_tid;
    pthread_create(&recv_tid, NULL, receiver, NULL);
    pthread_create(&input_tid, NULL, input_handler, NULL);

    char buf[BUFLEN];
    chat_state_t state = STATE_IDLE;

    // --- Main loop: our little state machine! ---
    while (1) {
        switch (state) {
            case STATE_IDLE:
                // See if there's a received message or something to send
                if (circbuf_pop(&recv_buf, buf) == 0) {
                    state = STATE_RECEIVING;
                } else if (msgqueue_dequeue(&send_queue, buf) == 0) {
                    state = STATE_SENDING;
                }
                break;

            case STATE_RECEIVING:
                // Got a message from the peer, print it out
                printf("[Peer]: %s", buf);
                fflush(stdout);
                state = STATE_IDLE;
                break;

            case STATE_SENDING:
                // Send a message to the peer
                sendto(sockfd, buf, strlen(buf), 0,
                       (struct sockaddr *)&peer_addr, sizeof(peer_addr));
                state = STATE_IDLE;
                break;
        }
    }

    close(sockfd);
    return 0;
}

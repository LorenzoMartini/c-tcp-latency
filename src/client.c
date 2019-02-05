/*
 *  A client timing the roundtrip time of a message sent to a server multiple times.
 *  Usage: ./client.out <address> <port>
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <inttypes.h>

#include "connection.h"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Init buffers
    uint8_t *rbuffer = malloc(N_BYTES);
    uint8_t *wbuffer = malloc(N_BYTES);

    // Parse args and connect to server
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect and set nonblocking and nodelay
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    int flag = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(int));

    printf("Connection successful! Wait a few sec...");
    fflush( stdout );
    sleep(2);

    // Timed send-receive loop
    uint64_t *times_send = malloc(sizeof(uint64_t) * N_ROUNDS);
    uint64_t *times_recv = malloc(sizeof(uint64_t) * N_ROUNDS);
    for (size_t i = 0; i < N_ROUNDS; i++) {

        uint64_t tstart = rdtscp();

        send_message(N_BYTES, sockfd, wbuffer);
        uint64_t tsend = rdtsc();
        receive_message(N_BYTES, sockfd, rbuffer);

        uint64_t tend = rdtsc();

        times_send[i] = tsend - tstart;
        times_recv[i] = tend - tsend;
    }
    close(sockfd);
    for (size_t i = 0; i < N_ROUNDS; i++) {
        printf("%" PRIu64 "\t%" PRIu64 "\n", times_send[i], times_recv[i]);
    }
    free(times_send);
    free(times_recv);
    free(rbuffer);
    free(wbuffer);

    return 0;
}


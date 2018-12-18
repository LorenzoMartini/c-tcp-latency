/*
 *  A server receiving and sending back a message multiple times.
 *  Usage: ./server.out <port>
 */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include "connection.h"

#define N_BYTES 1000
#define N_ROUNDS 1000000

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[N_BYTES] = {[0 ... (N_BYTES - 1)] = 'a'};
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    // Create listening socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept connection and set nonblocking
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }
    fcntl(newsockfd, F_SETFL, O_NONBLOCK);

    // Receive-send loop
    printf("Connection accepted, ready to receive!\n");
    fflush( stdout );
    for (int i = 0; i < N_ROUNDS; i++) {
        receive_message(N_BYTES, newsockfd, buffer);
        send_message(N_BYTES, newsockfd, buffer);
    }
    printf("Done!\n");

    // Clean state
    close(sockfd);
    close(newsockfd);

    return 0; 
}


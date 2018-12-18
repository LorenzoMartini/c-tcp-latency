#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Init buffers: 1KB
    char rbuffer[1000] = {[0 ... 999] = 'a'};
    char wbuffer[1000] = {[0 ... 999] = 'a'};

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
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
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
    printf("Connection successful! Wait a few sec...");
    time_t now, later;
    struct timespec tstart={0,0}, tend={0,0};
    fflush( stdout );
    sleep(2);
    for (int i = 0; i < 1000000; i++) {
        clock_gettime(CLOCK_MONOTONIC, &tstart);
	n = write(sockfd, wbuffer, strlen(wbuffer));
	if (n < 0) {
	 error("ERROR writing to socket");
        }
	n = read(sockfd,rbuffer,1000);
	if (n < 0) {
	 error("ERROR reading from socket");
        }
        clock_gettime(CLOCK_MONOTONIC, &tend);
        printf("Round %d took %.5f seconds\n",
            i,
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    }
    return 0;
}

#include <errno.h>

void panic(char *msg)
{
    perror(msg);
    exit(0);
}

int receive_message(int n_bytes, int sockfd, char* buffer) {
    int n = 0;
    int r;
    while (n < n_bytes) {
        r = read(sockfd, buffer, n_bytes);
        if (r < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            panic("ERROR reading from socket");
        }
        if (r > 0) {
            n += r;
        }
    }
    return n;
}

int send_message(int n_bytes, int sockfd, char* buffer) {
    int n = 0;
    int r;
    while (n < n_bytes) {
        r = write(sockfd, buffer, n_bytes);
	if (r < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            panic("ERROR writing to socket");
        }
        if (r > 0) {
            n += r;
        }
    }
    return n;
}


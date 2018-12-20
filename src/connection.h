#include <errno.h>
#include <stdint.h>

#define N_BYTES (1)
#define N_ROUNDS 1000000

void panic(char *msg)
{
    perror(msg);
    exit(0);
}

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

uint64_t rdtscp(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtscp" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}


int receive_message(size_t n_bytes, int sockfd, uint8_t *buffer) {
    int n = 0;
    int r;
    while (n < n_bytes) {
        r = read(sockfd, buffer, n_bytes);
        if (r < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK)) {
            panic("ERROR reading from socket");
        }
        if (r > 0) {
            n += r;
        }
    }
    return n;
}

int send_message(size_t n_bytes, int sockfd, uint8_t *buffer) {
    int n = 0;
    int r;
    while (n < n_bytes) {
        r = write(sockfd, buffer, n_bytes);
        if (r < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK)) {
            panic("ERROR writing to socket");
        }
        if (r > 0) {
            n += r;
        }
    }
    return n;
}


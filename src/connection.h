#include <errno.h>
#include <stdint.h>

#define DEFAULT_N_BYTES 1
#define DEFAULT_PORT 8787
#define DEFAULT_ADDRESS "127.0.0.1"
#define N_ROUNDS 1000000

struct Config {
    char *address;
    int port;
    int n_bytes;
};

void print_config(struct Config config) {
    printf("Address: %s, Port: %d, N_bytes: %d", config.address, config.port, config.n_bytes);
}

struct Config get_config(int argc, char *argv[]) {
    struct Config config;
    int c;
    config.n_bytes = DEFAULT_N_BYTES;
    config.port = DEFAULT_PORT;
    config.address = DEFAULT_ADDRESS;

    while ((c = getopt(argc, argv, "a:p:b:")) != -1) {
        switch (c) {
            case 'a':
                    config.address = optarg;
                    break;
            case 'p':
                    config.port = atoi(optarg);
                    break;
            case 'b':
                    config.n_bytes = atoi(optarg);
                    break;
            default: /* '?' */
                   fprintf(stderr, "Usage: %s [-b bytes] [-a address] [-p port]\n",
                           argv[0]);
                   exit(EXIT_FAILURE);
        }
    }
    print_config(config);
    return config;
}

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


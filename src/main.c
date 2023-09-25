#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include "dns.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <DNS server> <domain>\n", argv[0]);
        return 1;
    }

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], "53", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket()");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "failed to create socket\n");
        return 2;
    }

    char *dns_query;
    size_t querySize;
    makeQuery(&dns_query, &querySize, argv[2]);

    if ((numbytes = sendto(sockfd, dns_query, querySize, 0, p->ai_addr, p->ai_addrlen)) == -1) {
        perror("sendto()");
        return 1;
    }

    freeaddrinfo(servinfo);
    free(dns_query);

    unsigned char dns_response[1024];
    if ((numbytes = recvfrom(sockfd, dns_response, sizeof(dns_response), 0, NULL, NULL)) == -1) {
        perror("recvfrom()");
        return 1;
    }

    printf("DNS Response (%d bytes):\n", numbytes);
    for (int i = 0; i < numbytes; ++i) {
        printf("%02x ", dns_response[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }

    close(sockfd);

    return 0;
}

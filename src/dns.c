#include <stdlib.h>
#include <string.h>
#include "dns.h"

struct DNS_HEADER {
    unsigned short id;
    unsigned short flags;
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
};

void makeQuery(char **buffer, size_t *querySize, char *domain) {
    struct DNS_HEADER header;

    header.id = htons(rand()); // Assign a random ID
    header.flags = htons(0x0100); // This is a query
    header.qdcount = htons(1); // One question
    header.ancount = htons(0);
    header.nscount = htons(0);
    header.arcount = htons(0);

    size_t qnameSize = strlen(domain) + 2; // one for the starting label size, one for the terminal 0
    char *token, *namep, *qname;
    namep = qname = (char *) malloc(qnameSize);
    while ((token = strsep(&domain, ".")) != NULL) {
        size_t len = strlen(token);
        *namep++ = len; // length of the label
        memcpy(namep, token, len);
        namep += len;
    }

    short qtype = htons(1); // Type A
    short qclass = htons(1); // Class IN

    // combine all pieces of query together on one buffer
    *querySize = sizeof(struct DNS_HEADER) + qnameSize + sizeof(qtype) + sizeof(qclass);
    char *qp, *query;
    qp = query = (char *)calloc(*querySize, 1);
    memcpy(qp, &header, sizeof(struct DNS_HEADER));
    qp += sizeof(struct DNS_HEADER);
    memcpy(qp, qname, qnameSize);
    qp += qnameSize;
    memcpy(qp, &qtype, sizeof(short));
    qp += sizeof(short);
    memcpy(qp, &qclass, sizeof(short));

    *buffer = query;
}
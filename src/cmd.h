//
// Created by Ethan Godt on 9/25/23.
//

#ifndef CDNS_CMD_H
#define CDNS_CMD_H

#include "dns.h"

typedef enum {
    A,
    NS
} QUERY_TYPE;

struct CMD_INPUT {
    char *ip;
    char *domain;
    QUERY_TYPE type;
};

int parseInput(int argc, char **argv, struct CMD_INPUT *input);

#endif //CDNS_CMD_H

//
// Created by Ethan Godt on 9/25/23.
//

#include <string.h>
#include <stdio.h>
#include "cmd.h"

int parseInput(int argc, char **argv, struct CMD_INPUT *input) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <DNS Server IP> <Domain> <?Type>\n", argv[0]);
        return 1;
    }

    input->ip = argv[1];
    input->domain = argv[2];
    if (strcmp(argv[3], "A") == 0) {
        input->type = A;
    } else if (strcmp(argv[3], "NS") == 0) {
        input->type = NS;
    }

    return 0;
};

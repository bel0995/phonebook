#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define GENERIC_ERROR_HELPER(cond, errCode, msg) do {               \
        if (cond) {                                                 \
            fprintf(stderr, "%s: %s\n", msg, strerror(errCode));    \
            exit(EXIT_FAILURE);                                     \
        }                                                           \
    } while(0)

#define ERROR_HELPER(ret, msg)          GENERIC_ERROR_HELPER((ret < 0), errno, msg)
#define PTHREAD_ERROR_HELPER(ret, msg)  GENERIC_ERROR_HELPER((ret != 0), ret, msg)
#define SERVER_ADDRESS  "127.0.0.1"
#define SERVER_PORT     1999


#define DEBUG           1   // display debug messages
#define MAX_CODA 3   // max number of connections the server can queue
#endif



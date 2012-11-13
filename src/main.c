#include <stdio.h>
#include <string.h>
#include "ttt.h"

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--server")) {
        server_start();
    }
    else if (argc > 1 && !strcmp(argv[1], "--client")) {
        client_start();
    }
    else {
        printf("Usage:\n");
        printf("    %s --server    Start Server (Player 1)\n", argv[0]);
        printf("    %s --client    Start Client (Player 2)\n", argv[0]);
    }

    close_shm();
    return 0;
}

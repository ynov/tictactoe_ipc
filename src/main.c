#include <stdio.h>
#include <string.h>
#include "ttt.h"

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--server")) {
        if (argc > 2 && !strcmp(argv[2], "--pc"))
            server_start(PLAYER_PC);
        else
            server_start(PLAYER_HUMAN);
    }
    else if (argc > 1 && !strcmp(argv[1], "--client")) {
        if (argc > 2 && !strcmp(argv[2], "--pc"))
            client_start(PLAYER_PC);
        else
            client_start(PLAYER_HUMAN);
    }
    else {
        printf("Usage:\n");
        printf("    %s --server         Start Server (Player 1)\n", argv[0]);
        printf("    %s --client         Start Client (Player 2)\n", argv[0]);
        printf("    %s [......] --pc    Start as PC Player\n", argv[0]);
    }

    return 0;
}

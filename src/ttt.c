#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ttt.h"
#include "tttshm.h"

void print_board(ttt *t)
{
    int r, c;
    printf("Board: \n");
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            printf("%c ", t->board[r][c]);
        }
        printf("\n");
    }
}

void get_input(ttt *t, int player)
{
    int r, c;
    char p_char;

    p_char = (player == 1) ? 'X' : 'O';

    for(;;) {
        printf("\nYour move [<row> <col>]: ");
        scanf("%d %d", &r, &c);
        
        if (t->board[r-1][c-1] == '_'
            && r >= 1 && r <= 4
            && c >= 1 && c <= 4)
        {
            t->board[r - 1][c - 1] = p_char;
            break;
        } else {
            printf("Bad move, repeat\n");
        }
    }

    t->player_turn = (t->player_turn == 1) ? 2 : 1;
    print_board(t);
}

void server_start()
{
    ttt *t;
    
    printf("[Server Mode]\n");
    t = open_and_init_shm();

    printf("Waiting for client...\n");

    while (t->num_player != 2) {}
    printf("Player 2 connected!\n");

    for(;;) {
        printf("\nWaiting for Player 2...\n\n");
        while (t->player_turn != 1) {}
        
        print_board(t);
        get_input(t, 1);
    }
}

void client_start()
{
    ttt *t;
    
    printf("[Client Mode]\n");
    t = open_shm();

    if (t->num_player == 1) {
        t->num_player = 2;
        printf("Connected to Player 1!\n");
    } else {
        printf("Not connected! Closing...\n");
        return;
    }

    for(;;) {
        printf("\nWaiting for Player 1...\n\n");
        while (t->player_turn != 2) {}

        print_board(t);
        get_input(t, 2);
    }
}



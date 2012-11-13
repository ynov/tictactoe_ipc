#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ttt.h"
#include "tttshm.h"

static void print_board(ttt *t)
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

static bool_t check_board(ttt *t, int player)
{
    int r, c;
    char p_char;

    p_char = (player == 1) ? 'X' : 'O';
    
#   define RC(r, c) (t->board[r][c] == p_char)

    /* check horizontal */
    for (r = 0; r < 3; r++) {
        if (RC(r, 0) && RC(r, 1) && RC(r, 2))
            return TRUE;
    }

    /* check vertical */
    for (c = 0; c < 3; c++) {
        if (RC(0, c) && RC(1, c) && RC(2, c))
            return TRUE;
    }

    /* check diagonal */
    if (RC(0, 0) && RC(1, 1) && RC(2, 2))
        return TRUE;
    if (RC(0, 2) && RC(1, 1) && RC(2, 0))
        return TRUE;

    return FALSE;
}

static bool_t get_input_and_check(ttt *t, int player, player_t p_type)
{
    int r, c, check;
    char p_char;

    p_char = (player == 1) ? 'X' : 'O';

    for(;;) {
        printf("\nYour move [<row> <col>]: ");

        if (p_type == PLAYER_HUMAN) {
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
        } else { /* PLAYER_PC */
            /* TODO */
        }
    }

    check = check_board(t, player);
    if (check) {
        t->end = player;
    }

    t->player_turn = (t->player_turn == 1) ? 2 : 1;
    print_board(t);

    return check;
}

static void player_loop(ttt *t, int player, player_t p_type)
{
     for(;;) {
        printf("\nWaiting for Player %d...\n\n", player);
        
        while (t->player_turn != player)
            ;

        if (t->end > 0) {
            printf("Player %d win!\n", t->end);
            close_shm();
            break;
        }

        print_board(t);
        if (get_input_and_check(t, player, p_type)) {
            printf("\nYou win!\n");
            break;
        }
    }   
}

void server_start(player_t p_type)
{
    ttt *t;
    
    printf("[Server Mode]\n");
    t = open_and_init_shm();

    printf("Waiting for client...\n");

    while (t->num_player != 2)
        ;

    printf("Player 2 connected!\n");
    
    player_loop(t, 1, p_type);
}

void client_start(player_t p_type)
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

    player_loop(t, 2, p_type);
}


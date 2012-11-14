#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

static int check_board(ttt *t, int player)
{
    int r, c;
    char p_char;

    p_char = (player == 1) ? 'X' : 'O';
    
#   define RC(r, c) (t->board[r][c] == p_char)

    /* check horizontal */
    for (r = 0; r < 3; r++) {
        if (RC(r, 0) && RC(r, 1) && RC(r, 2))
            return WIN;
    }

    /* check vertical */
    for (c = 0; c < 3; c++) {
        if (RC(0, c) && RC(1, c) && RC(2, c))
            return WIN;
    }

    /* check diagonal */
    if (RC(0, 0) && RC(1, 1) && RC(2, 2))
        return WIN;
    if (RC(0, 2) && RC(1, 1) && RC(2, 0))
        return WIN;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            if (t->board[r][c] == '_') {
                return NEXT;
            }
        }
    }

    return DRAW;
}

static void cpu_move(ttt *t, int player)
{
    int r, c, rc, i, j, k;
    char p_char, o_char; /* player, opponent */

    p_char = (player == 1) ? 'X' : 'O';
    o_char = (p_char == 'X') ? 'O' : 'X';

    /* h */
    for (r = 0; r < 3; r++) {
        i = 0;
        j = 0;
        k = 0;
        for (c = 0; c < 3; c++) {
            i = (t->board[r][c] == o_char) ? i + 1 : i;
            j = (t->board[r][c] == p_char) ? j + 1 : j;
            k = (t->board[r][c] == '_') ? c : k;
        }
        if ((i == 2 || j == 2) && k != 0) {
            printf("%d %d\n", r + 1, k + 1);
            t->board[r][k] = p_char;
            return;
        }
    }

    /* v */
    for (c = 0; c < 3; c++) {
        i = 0;
        j = 0;
        k = 0;
        for (r = 0; r < 3; r++) {
            i = (t->board[r][c] == o_char) ? i + 1 : i;
            j = (t->board[r][c] == p_char) ? j + 1 : j;
            k = (t->board[r][c] == '_') ? r : k;
        }
        if ((i == 2 || j == 2) && k != 0) {
            printf("%d %d\n", k + 1, c + 1);
            t->board[k][c] = p_char;
            return;
        }
    }

    /* diagonal 1 */
    i = 0;
    j = 0;
    k = 0;
    for (rc = 0; rc < 3; rc++) {
        i = (t->board[rc][rc] == o_char) ? i + 1 : i;
        j = (t->board[rc][rc] == p_char) ? j + 1 : j;
        k = (t->board[rc][rc] == '_') ? rc : k;
    }
    if ((i == 2 || j == 2) && k != 0) {
        printf("%d %d\n", k + 1, k + 1);
        t->board[k][k] = p_char;
        return;
    }

    /* diagonal 2 */
    i = 0;
    j = 0;
    k = 0;
    for (rc = 0; rc < 3; rc++) {
        i = (t->board[rc][2 - rc] == o_char) ? i + 1 : i;
        j = (t->board[rc][2 - rc] == p_char) ? j + 1 : j;
        k = (t->board[rc][2 - rc] == '_') ? rc : k;
    }
    if ((i == 2 || j == 2) && k != 0) {
        printf("%d %d\n", k + 1, 2 - k + 1);
        t->board[k][2 - k] = p_char;
        return;
    }

    /* random placement */
    srand(time(NULL));
    for (;;) {
        r = rand() % 3;
        c = rand() % 3;
        if (t->board[r][c] == '_') {
            printf("%d %d\n", r + 1, c + 1);
            t->board[r][c] = p_char;
            return;
        }
    }
}

static int get_input_and_check(ttt *t, int player, player_t p_type)
{
    int r, c, check;
    char p_char;

    p_char = (player == 1) ? 'X' : 'O';

    for(;;) {
        printf("\nP%d - Your move [<row> <col>]: ", player);

        if (p_type == PLAYER_HUMAN) {
            scanf("%d %d", &r, &c);
            
            if (t->board[r - 1][c - 1] == '_'
                && r >= 1 && r <= 4
                && c >= 1 && c <= 4)
            {
                t->board[r - 1][c - 1] = p_char;
                break;
            } else {
                printf("Bad move, repeat\n");
            }
        } else { /* PLAYER_PC */
            cpu_move(t, player);
            break;
        }
    }

    check = check_board(t, player);
    if (check == WIN) {
        t->end = player;
    } else if (check == DRAW) {
        t->end = -1;
    }

    t->player_turn = (t->player_turn == 1) ? 2 : 1;
    print_board(t);

    return check;
}

static void player_loop(ttt *t, int player, player_t p_type)
{
    int res;

    for(;;) {
        printf("\nWaiting for Player %d...\n\n", (player == 1) ? 2 : 1);
        
        while (t->player_turn != player)
            usleep(30 * 1000);

        if (t->end > 0 || t->end == -1) {
            print_board(t);
            if (t->end > 0)
                printf("\nPlayer %d win!\n", t->end);
            else
                printf("\nDraw!\n");
            close_shm();
            break;
        }

        print_board(t);
        
        res = get_input_and_check(t, player, p_type);
        if (res == WIN) {
            printf("\nYou win!\n");
            break;
        } else if (res == DRAW) {
            printf("\nDraw!\n");
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
        usleep(30 * 1000);

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


#ifndef _TTT_H_
#define _TTT_H_

typedef int player_t;
typedef int bool_t;

#define FALSE 0
#define TRUE  1

#define PLAYER_HUMAN 1
#define PLAYER_PC    2

typedef struct {
    char board[3][3];
    int num_player;
    int player_turn;
    int end;
} ttt;

void server_start(player_t p_type);
void client_start(player_t p_type);

#endif

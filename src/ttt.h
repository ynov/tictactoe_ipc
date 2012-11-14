#ifndef _TTT_H_
#define _TTT_H_

typedef int player_t;

#define PLAYER_HUMAN 1
#define PLAYER_PC    2

#define NEXT  0
#define WIN   1
#define DRAW -1

typedef struct {
    char board[3][3];
    int num_player;
    int player_turn;
    int end;
} ttt;

void server_start(player_t p_type);
void client_start(player_t p_type);

#endif

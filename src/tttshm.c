#include <stdlib.h>

#include "tttshm.h"
#include "ttt.h"

static ttt *_open_shm(int flags)
{
    ttt *t;
    int fd;

    if (flags & SHM_INIT) {
        fd = open(SHMFILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
        lseek(fd, SHMSIZE, SEEK_END);
        write(fd, "", 1);
    }
    else if (flags & SHM_OPEN || flags & SHM_END) {
        fd = open(SHMFILE, O_RDWR);
    }

    t = (ttt *) mmap(NULL, SHMSIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0);

    if (flags & SHM_INIT) {
        int r, c; /* row, col */
        for (r = 0; r < 3; r++) {
            for (c = 0; c < 3; c++) {
                t->board[r][c] = '_';
            }
        }

        t->num_player = 1;
        t->player_turn = 1;
        t->end = 0;
    }
    else if (flags & SHM_END) {
        t->num_player = 0;
        t->player_turn = 0;
        t->end = 1;

        munmap(t, SHMSIZE);
        close(fd);
        return NULL;
    }

    close(fd);
    return t;
}

ttt *open_and_init_shm()
{
    return _open_shm(SHM_INIT);
}

ttt *open_shm()
{
    return _open_shm(SHM_OPEN);
}

ttt *close_shm()
{
    return _open_shm(SHM_END);
}


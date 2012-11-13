#ifndef _TTTSHM_H_
#define _TTTSHM_H_

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include "ttt.h"

#define SHMFILE "/tmp/tttshm"
#define SHMSIZE (sizeof(ttt))

#define SHM_OPEN (1 << 0)
#define SHM_INIT (1 << 1)
#define SHM_END  (1 << 2)

ttt *open_and_init_shm();
ttt *open_shm();

#endif

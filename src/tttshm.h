#ifndef _TTTSHM_H_
#define _TTTSHM_H_

#define SHMFILE "/tmp/tttshm"
#define SHMSIZE (sizeof(ttt))

#ifdef __unix__
#   include <sys/mman.h>
#   include <unistd.h>
#   include <fcntl.h>
#endif

#ifdef _WIN32
#   include <windows.h>
#   undef SHMFILE
#   define SHMFILE "tttshm"
#endif

#include "ttt.h"

#define SHM_OPEN (1 << 0)
#define SHM_INIT (1 << 1)
#define SHM_END  (1 << 2)

ttt *open_and_init_shm();
ttt *open_shm();

#endif

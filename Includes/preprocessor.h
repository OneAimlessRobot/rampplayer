#include "../ncurses/include/ncurses.h"
#include "../SDL2/include/SDL.h"
#include "../SDL2/include/SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include "metadata.h"

#define INITDIR (strcat(getcwd(0,STRING_SIZE),"/"))

#include "auxiliar.h"
#include "choicemenu.h"




BINARY= player
CURRDIR = $(shell pwd)



LDLIBS := -L$(CURRDIR)/../SDL2/lib $(CURRDIR)/../SDL2/lib/libSDL2.a  $(CURRDIR)/../SDL2/lib/libSDL2_mixer.a -lm -ldl -lpthread
LDLIBS += -I$(CURRDIR)/../SDL2/include -Wl,-rpath,$(CURRDIR)/../SDL2/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2_mixer
LDLIBS += -L$(CURRDIR)/../ncurses/lib $(CURRDIR)/../ncurses/lib/libncurses.a $(CURRDIR)/../ncurses/lib/libncursesw.a $(CURRDIR)/../ncurses/lib/libtinfo.a
LDLIBS += -I$(CURRDIR)/../ncurses/include -Wl,-rpath,$(CURRDIR)/../ncurses/lib -Wl,--enable-new-dtags -lncurses -lncursesw -ltinfo



INCLUDE= ./Includes .
SOURCES= . ./Sources ../Sources
RESDIR= ./resources
 
CURRDIR=echo `pwd`
 
CC= gcc
DEPFLAGS= -MP -MD

CFLAGS= -Wall  -DPROGRAMPATHAUX="$(CURRDIR)"  -Wextra -g $(foreach D, $(INCLUDE), -I$(D)) $(DEPFLAGS)

SOURCEFILES=$(foreach D,$(SOURCES), $(wildcard $(D)/*.c))


RESOURCEFILES=$(wildcard $(RESDIR)/*.o)


OBJECTS=$(patsubst %.c,%.o,$(SOURCEFILES))

ALLMODULES= $(RESOURCEFILES) $(OBJECTS)

DEPFILES= $(patsubst %.c,%.d,$(SOURCEFILES))

all: $(BINARY)
	echo $(LDLIBS)
	echo $(CURRDIR)


$(BINARY): $(OBJECTS)
	$(CC) -g -v  $(CFLAGS) -o  $@ $^ $(RESOURCEFILES)  $(LDLIBS)


%.o:%.c
	$(CC) -g  $(CFLAGS) -c -o $@ $<  $(LDLIBS)
	echo $(CFLAGS)

clean:
	rm -rf $(BINARY) $(ALLMODULES) $(DEPFILES)

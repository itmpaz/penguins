#!/bin/sh

mkdir -p bin
cd bin


alias mygcc='gcc -I ..'


mygcc ../master/game_io.c      -o game_io.o       -c
mygcc ../master/game_io_json.c -o game_io_json.o  -c
mygcc ../master/game_utils.c   -o game_utils.o    -c
mygcc ../master/game_master.c  -o game_master.o   -c
mygcc ../master/master.c       -o master.o        -c
gcc -o master game_io.o game_io_json.o game_utils.o game_master.o master.o


mygcc ../player/player_template.c       -o player_template.o        -c
gcc -o player player_template.o



rm -f *.o
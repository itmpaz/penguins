#!/bin/sh

alias mygcc='gcc -I ..'


mygcc game_io.c      -o game_io.o       -c
mygcc game_io_json.c -o game_io_json.o  -c
mygcc game_utils.c   -o game_utils.o    -c
mygcc game_master.c  -o game_master.o   -c
mygcc master.c       -o master.o        -c

gcc -o gamemaster game_io.o game_io_json.o game_utils.o game_master.o master.o

rm -f *.o
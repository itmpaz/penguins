#ifndef GAME_MASTER_H
#define GAME_MASTER_H

#include "game.h"





uint16_t gm_del(GAME* game);
uint16_t gm_new(GAME* game);
uint16_t gm_turn(GAME* game, PLAYERTURN* turn);
uint16_t gm_turnnext(GAME* game, uint16_t turnresult);
uint16_t gm_checkgameover(GAME* game);



#endif
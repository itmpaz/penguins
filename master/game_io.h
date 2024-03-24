#ifndef GAME_IO_H
#define GAME_IO_H

#include "game.h"


#define GMIO_LOGFILE         "gamelog.txt"
#define GMIO_GAMEFILE        "game.bin"
#define GMIO_TURNFILE        GAME_TURNFILE
#define GMIO_PLAYERDATAFILE  GAME_PLAYERDATAFILE

uint16_t gm_print(GAME* game);

uint16_t gm_save(GAME* game);
uint16_t gm_load(GAME* game);

uint16_t gm_saveplayerdata(GAME* game);
uint16_t gm_loadplayerdata(PLAYERBOARD* playerdata);

uint16_t gm_loadturn(PLAYERTURN* turn);
uint16_t gm_saveturn(PLAYERTURN* turn);

uint16_t gm_logerror(GAME* game, uint16_t error_code, char* message);
uint16_t gm_logboard(GAME* game,char* key);
uint16_t gm_logturn(GAME* game, PLAYERTURN* turn, uint16_t turnrc);
uint16_t gm_logresults(GAME* game);
uint16_t gm_logparams(GAME* game);
uint16_t gm_logclose(GAME* game); 
uint16_t gm_lognew(GAME* game);

uint16_t gm_saveplayerdataex(GAME* game, FILE* f);

#endif
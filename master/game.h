#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#include "penguins.h"

#include "game_io_json.h"

#define GAME_MASTER_VERSION "Game Master version 1.0"


#define RC_ERROR_NOT_INITIALIZED              (GAME_RC_ERROR+1)
#define RC_ERROR_MEMORY_ALLOCATION            (GAME_RC_ERROR+2)
#define RC_ERROR_OVERFLOW                     (GAME_RC_ERROR+3)
#define RC_ERROR_BOARD_TOO_SMALL              (GAME_RC_ERROR+4)
#define RC_ERROR_WRONG_PLAYER_TURN            (GAME_RC_ERROR+5)
#define RC_ERROR_ADD_WRONG_PLACE              (GAME_RC_ERROR+6)
#define RC_ERROR_ADD_NOONELEFT                (GAME_RC_ERROR+7)
#define RC_ERROR_MOVE_RULES                   (GAME_RC_ERROR+8)
#define RC_ERROR_MOVE_GAP                     (GAME_RC_ERROR+9)
#define RC_ERROR_MOVE_PENGUIN                 (GAME_RC_ERROR+10)
#define RC_ERROR_PLAYERID_OVERFLOW            (GAME_RC_ERROR+11)
#define RC_ERROR_POS_PLACE                    (GAME_RC_ERROR+12)
#define RC_ERROR_POS_VALUE                    (GAME_RC_ERROR+13)
#define RC_ERROR_WRONG_CMD                    (GAME_RC_ERROR+14)
#define RC_ERROR_BAD_BOARD_SHAPE              (GAME_RC_ERROR+15)
#define RC_ERROR_BAD_GAME_PARAMS              (GAME_RC_ERROR+16)
#define RC_ERROR_FILEIO                       (GAME_RC_ERROR+17)



#define RC_GM_RUN_PLAYER               10
#define RC_GM_STOP                     1
#define RC_GM_STOP_GAMEOVER            (RC_GM_STOP +1)
#define RC_GM_STOP_GAME_CREATE_ERROR   (RC_GM_STOP +2)
#define RC_GM_STOP_GAME_LOAD_ERROR     (RC_GM_STOP +3)


#define GM_TURN_OK                       0   
#define GM_TURN_PASS                     1
#define GM_TURN_ERROR                    100
#define GM_TURN_REJECT_POSOVERFLOW_1ST   (GM_TURN_ERROR + 1)
#define GM_TURN_REJECT_POSOVERFLOW_2ND   (GM_TURN_ERROR + 2)
#define GM_TURN_REJECT_PLAYERID          (GM_TURN_ERROR + 3)
#define GM_TURN_REJECT_ADD               (GM_TURN_ERROR + 4)
#define GM_TURN_REJECT_TOOMANY           (GM_TURN_ERROR + 5)
#define GM_TURN_REJECT_NOTSTRIGHT        (GM_TURN_ERROR + 6)
#define GM_TURN_REJECT_GAP               (GM_TURN_ERROR + 7)
#define GM_TURN_REJECT_PENGUIN           (GM_TURN_ERROR + 8)
#define GM_TURN_NODATA                   (GM_TURN_ERROR + 9)
#define GM_TURN_REJECT_PASS_ADD          (GM_TURN_ERROR + 10)
#define GM_TURN_REJECT_PASS_MOVE         (GM_TURN_ERROR + 11)
#define GM_TURN_REJECT_TOOFEW            (GM_TURN_ERROR + 12)
#define GM_TURN_REJECT_SAMEPLACE         (GM_TURN_ERROR + 13)
#define GM_TURN_REJECT_BLOCKED           (GM_TURN_ERROR + 20)


#define GM_STRMAX 1024

#pragma pack(push, 1)

typedef struct tagGAMEINFO
{

    uint32_t turn_counter;
    uint32_t players_count;
    uint32_t penguins_count;
    uint32_t max_fish_count;
    uint16_t board_width;
    uint16_t board_height;
    uint32_t rand_seed;
    GAME_CELLTYPE player_turn;
    uint32_t pass_counter;
    uint32_t fish_left;
    uint16_t gameover;
    uint16_t errorcounter;

    uint32_t score[GAME_PLAYER_MAX];
    uint8_t  block[GAME_PLAYER_MAX];



} GAMEINFO;


typedef struct tagGAME
{

    GAMEINFO info;
    JSON json;
    GAMEBOARD board;

} GAME;

#pragma pack(pop)


uint16_t gm_setat(GAME* game, GAME_POSTYPE col, GAME_POSTYPE row, GAME_CELLTYPE value);
GAME_CELLTYPE gm_getat(GAME* game, GAME_POSTYPE col, GAME_POSTYPE row);
GAME_SIZETYPE gm_count(GAME* game, GAME_CELLTYPE e);
uint16_t gm_create(GAMEBOARD* board, GAME_POSTYPE width, GAME_POSTYPE height);
uint16_t gm_destroy(GAMEBOARD* board);
char gm_boardchar(GAME_CELLTYPE n);
GAME_SIZETYPE gm_count_allfishes(GAME* game);



#endif 
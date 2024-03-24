#pragma once


#include <stdint.h> 


#define GAME_GAP          0  
#define GAME_FISH_0       0x00 
#define GAME_FISH_1       0x01 
#define GAME_FISH_2       0x02 
#define GAME_FISH_3       0x03 
#define GAME_PLAYER_0     0x80
#define GAME_PLAYER_1     0x81
#define GAME_ERROR        0xFF


#define GAME_FISH_MAX     3
#define GAME_PLAYER_MAX   100


#define GAME_RC_OK        0
#define GAME_RC_GIVEUP    1
#define GAME_RC_GAMEOVER  2
#define GAME_RC_ERROR     100

#define GAME_POS_NULL 0xFFFF

#define GAME_CELLTYPE  uint8_t
#define GAME_POSTYPE   uint16_t
#define GAME_SIZETYPE  uint32_t


#define GAME_TURNFILE        "turn.bin"
#define GAME_PLAYERDATAFILE  "player.bin"


#define GAME_PLAYER_OK       0
#define GAME_PLAYER_BLOCKED  1

#pragma pack(push, 1)



typedef struct tagPLAYERTURN
{
    GAME_POSTYPE from_row;
    GAME_POSTYPE from_col;
    GAME_POSTYPE to_row;
    GAME_POSTYPE to_col;

} PLAYERTURN;



typedef struct tagGAMEBOARD
{
    GAME_SIZETYPE bytesize;
    uint8_t* buf;

} GAMEBOARD;



typedef struct tagPLAYERINFO
{
    GAME_CELLTYPE player_id;
    GAME_SIZETYPE unused_penguins_count;
    GAME_SIZETYPE turn_counter;
    GAME_POSTYPE board_width;
    GAME_POSTYPE board_height;
    uint8_t player_status;

} PLAYERINFO;

typedef struct tagPLAYERBOARD
{
    PLAYERINFO info;
    GAMEBOARD board;


} PLAYERBOARD;



#pragma pack(pop)





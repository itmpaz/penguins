#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "game_master.h"




uint16_t gm_fill(GAME* game, size_t min_onefish,size_t max_fish)
{

	GAME_POSTYPE w = game->info.board_width;
	GAME_POSTYPE h = game->info.board_height;

	size_t n = w * h;
	if (n<min_onefish)
		return RC_ERROR_BOARD_TOO_SMALL;
	if( (game->board.buf == NULL) || (game->board.bytesize == 0))
		return RC_ERROR_NOT_INITIALIZED;

	size_t onefishcounter = 0;
	for (GAME_POSTYPE i = 0; i < h; i++)
		for (GAME_POSTYPE j = 0; j < w; j++)
		{
            GAME_CELLTYPE e = (GAME_CELLTYPE)((rand() % max_fish) + 1);
			//GAME_CELLTYPE e = (GAME_CELLTYPE)(rand() * max_fish / RAND_MAX) + 1;
			if (e == GAME_FISH_1)
				onefishcounter++;
			gm_setat(game, j, i, e);
		}

	while (onefishcounter < min_onefish)
	{
		uint16_t i = (rand() * h / RAND_MAX);
		uint16_t j = (rand() * w / RAND_MAX);
		GAME_CELLTYPE e = gm_getat(game, j, i);
		if (e != GAME_FISH_1)
		{
			gm_setat(game, j, i, GAME_FISH_1);
			onefishcounter++;
		}
	}

	return GAME_RC_OK;
}








uint16_t gm_new(GAME* game)
{
	if (game->info.penguins_count == 0)
		return RC_ERROR_BAD_GAME_PARAMS;
	if (game->info.players_count == 0)
		return RC_ERROR_BAD_GAME_PARAMS;

	int rc1 = gm_create(&(game->board), game->info.board_width, game->info.board_height);
	if (rc1 != GAME_RC_OK)
		return rc1;
	game->info.rand_seed = (uint32_t)time(NULL);
	srand((unsigned int)game->info.rand_seed);
	int rc2 = gm_fill(game, game->info.players_count * game->info.penguins_count, game->info.max_fish_count);
	if (rc2 != GAME_RC_OK)
		return rc2;
	game->info.player_turn = GAME_PLAYER_0;
	game->info.fish_left = gm_count_allfishes(game);
	return GAME_RC_OK;
}



uint16_t gm_del(GAME* game)
{
	gm_destroy(&(game->board));

	return GAME_RC_OK;
}





uint16_t gm_addscore(GAME* game, GAME_CELLTYPE playerid, GAME_SIZETYPE score)
{
	uint16_t playerindex = playerid - GAME_PLAYER_0;
	if (playerindex >= GAME_PLAYER_MAX)
		return RC_ERROR_PLAYERID_OVERFLOW;
	game->info.score[playerindex] += score;
	game->info.fish_left -= score;
	return GAME_RC_OK;
}


uint16_t gm_turnnext(GAME* game,uint16_t turnresult)
{
	uint16_t playerindex = game->info.player_turn - GAME_PLAYER_0;

	if ((turnresult != GM_TURN_OK) && (turnresult != GM_TURN_PASS))
	{
		game->info.block[playerindex] = GAME_PLAYER_BLOCKED;
		game->info.score[playerindex] = 0;
	}

	playerindex++;
	if (playerindex >= game->info.players_count)
		playerindex = 0;
	game->info.player_turn = playerindex + GAME_PLAYER_0;

	game->info.turn_counter++;

	if (turnresult == GM_TURN_OK)
	{	game->info.pass_counter=0;
	} else
	{	game->info.pass_counter++;
	}

	return GAME_RC_OK;
}


int gm_turn_countmovepossibilities(GAME* game)
{

	int possibilities_count = 0;
	for (int row = 0; row < game->info.board_height; row++)
		for (int col = 0; col < game->info.board_width; col++)
			if (gm_getat(game,row, col) == game->info.player_turn)
			{
				int ways[] = { 0,1,  0,-1,  1,0, -1,0 };
				for (int wi = 0; wi < 4; wi++)
				{
					int dr = ways[wi * 2];
					int dc = ways[wi * 2 + 1];
					int r = row;
					int c = col;

					while (1)
					{
						r += dr;
						c += dc;
						GAME_CELLTYPE e = gm_getat(game, r, c);
						if ((e <= GAME_FISH_MAX) && (e > GAME_FISH_0))
						{
							possibilities_count++;
							continue;
						}
						break;
					}
				}
			}

	return possibilities_count;

}





uint16_t gm_turn(GAME* game, PLAYERTURN* turn)
{
	GAME_POSTYPE w = game->info.board_width;
	GAME_POSTYPE h = game->info.board_height;
	GAME_POSTYPE fc = turn->from_col;
	GAME_POSTYPE fr = turn->from_row;
	GAME_POSTYPE tc = turn->to_col;
	GAME_POSTYPE tr = turn->to_row;
	GAME_CELLTYPE playerid = game->info.player_turn;
	GAME_CELLTYPE playerindex = playerid - GAME_PLAYER_0;

	if (game->info.block[playerindex] != GAME_PLAYER_OK)
	{
		return GM_TURN_REJECT_BLOCKED; 
	}

	if ((fc == GAME_POS_NULL) || (fr == GAME_POS_NULL))
	{
		if ( (gm_count(game, playerid) < game->info.penguins_count) && (gm_count(game, GAME_FISH_1) > 0) )
			return GM_TURN_REJECT_PASS_ADD;

		int n = gm_turn_countmovepossibilities(game);
		if (n > 0)
			return GM_TURN_REJECT_PASS_MOVE;

		return GM_TURN_PASS;
	}

	if ((fc >= w) || (fr >= h))
	{
		return GM_TURN_REJECT_POSOVERFLOW_1ST;
	}

	GAME_SIZETYPE np = gm_count(game, playerid);
	GAME_CELLTYPE e_from = gm_getat(game, fc, fr);

	if ((tc == GAME_POS_NULL) || (tr == GAME_POS_NULL))
	{
		if ((e_from>=GAME_PLAYER_0) && (e_from != playerid))
			return GM_TURN_REJECT_PLAYERID;

		if (e_from != GAME_FISH_1)
			return GM_TURN_REJECT_ADD;

		if ((np + 1) > game->info.penguins_count)
			return GM_TURN_REJECT_TOOMANY;

		gm_addscore(game, playerid, e_from - GAME_FISH_0);
		gm_setat(game, fc, fr, playerid);
		return  GM_TURN_OK;
	}

	if (np< game->info.penguins_count)
		return GM_TURN_REJECT_TOOFEW;

	if ((fc == tc) && (fr == tr))
		return GM_TURN_REJECT_SAMEPLACE;

	if ((tc >= w) || (tr >= h))
		return GM_TURN_REJECT_POSOVERFLOW_2ND;

	GAME_CELLTYPE e_to = gm_getat(game, tc, tr);

	if ((fc != tc) && (fr != tr))
		return GM_TURN_REJECT_NOTSTRIGHT;


	if (fc != tc)
	{
		uint16_t d = fc > tc ? (-1) : 1;
		uint16_t i = fc+d;
		while (i !=tc)
		{	
			GAME_CELLTYPE e = gm_getat(game, i, fr);
			if (e == GAME_GAP)
				return GM_TURN_REJECT_GAP;
			if (e >= GAME_PLAYER_0)
				return GM_TURN_REJECT_PENGUIN;
			i += d;
		}

	}
	else
	{
		uint16_t d = fr > tr ? (-1) : 1;
		uint16_t i = fr + d;
		while (i != tr)
		{
			GAME_CELLTYPE e = gm_getat(game, fc, i);
			if (e == GAME_GAP)
				return GM_TURN_REJECT_GAP;
			if (e >= GAME_PLAYER_0)
				return GM_TURN_REJECT_PENGUIN;
			i += d;
		}

	}

	gm_setat(game, fc, fr,GAME_GAP);
	gm_setat(game, tc, tr, playerid);
	int scoreinc = e_to - GAME_FISH_0;
	gm_addscore(game, playerid, scoreinc);

	return GM_TURN_OK;

}







uint16_t gm_checkgameover(GAME* game)
{
	if (game->info.pass_counter>game->info.players_count)
	{
		return GAME_RC_GAMEOVER;
	}

	return GAME_RC_OK;
}
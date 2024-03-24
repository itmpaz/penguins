#include <stdlib.h>
#include <string.h>

#include "game.h"


char gm_boardchar(GAME_CELLTYPE n)
{
	if (n == GAME_ERROR)
		return '!';
	if (n == GAME_GAP)
		return '*';
	if (n >= GAME_PLAYER_0)
		return 'A' + (n - GAME_PLAYER_0);
	return '0' + (n - GAME_FISH_0);
}


uint16_t gm_setat(GAME* game, GAME_POSTYPE col, GAME_POSTYPE row, GAME_CELLTYPE value)
{
	if ((row >= game->info.board_height) || (col >= game->info.board_width))
		return RC_ERROR_OVERFLOW;
	size_t pos = row * game->info.board_width * sizeof(GAME_CELLTYPE) + col * sizeof(GAME_CELLTYPE);
	if (pos > game->board.bytesize)
		return RC_ERROR_OVERFLOW;
	*((GAME_CELLTYPE*)(game->board.buf + pos)) = value;
	return GAME_RC_OK;
}

GAME_CELLTYPE gm_getat(GAME* game, GAME_POSTYPE col, GAME_POSTYPE row)
{
	if ((row >= game->info.board_height) || (col >= game->info.board_width))
		return RC_ERROR_OVERFLOW;
	size_t pos = row * game->info.board_width * sizeof(GAME_CELLTYPE) + col * sizeof(GAME_CELLTYPE);
	if (pos > game->board.bytesize)
		return RC_ERROR_OVERFLOW;
	return  *((GAME_CELLTYPE*)(game->board.buf + pos));
}

GAME_SIZETYPE gm_count(GAME* game, GAME_CELLTYPE e)
{
	GAME_SIZETYPE n = 0;
	for (uint16_t i = 0; i < game->info.board_height; i++)
		for (uint16_t j = 0; j < game->info.board_width; j++)
			if (gm_getat(game, j, i) == e)
				n++;
	return n;
}

GAME_SIZETYPE gm_count_allfishes(GAME* game)
{
	GAME_SIZETYPE n = 0;
	for (uint16_t i = 0; i < game->info.board_height; i++)
		for (uint16_t j = 0; j < game->info.board_width; j++)
		{
			GAME_CELLTYPE e = gm_getat(game, j, i);
			if ( (e>GAME_FISH_0) && (e<GAME_FISH_MAX))
				n+=e-GAME_FISH_0;
		}
	return n;
}


uint16_t gm_create(GAMEBOARD* board, GAME_POSTYPE width, GAME_POSTYPE height)
{
	gm_destroy(board);

	if ((width == 0) || (height == 0))
		return RC_ERROR_BAD_BOARD_SHAPE;


	board->bytesize = width * height * sizeof(GAME_CELLTYPE);
	board->buf = malloc(board->bytesize);
	if (board->buf == NULL)
		return RC_ERROR_MEMORY_ALLOCATION;
	memset(board->buf, GAME_ERROR, board->bytesize);

	return GAME_RC_OK;
}


uint16_t gm_destroy(GAMEBOARD* board)
{
	if (board->buf != NULL)
		free(board->buf);

	board->bytesize = 0;
	board->buf = NULL;

	return GAME_RC_OK;
}



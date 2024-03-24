

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "penguins.h"


void clean_up(PLAYERBOARD* game)
{
	free(game->board.buf);
	memset(&game->board, 0, sizeof(GAMEBOARD));
}

uint16_t load(PLAYERBOARD* game)
{
	assert(game->board.buf == NULL);
	clean_up(game);
	int rc = GAME_RC_ERROR;
	FILE* f = fopen(GAME_PLAYERDATAFILE, "rb");
	if (f != NULL)
	{
		if (fread(&game->info, sizeof(PLAYERINFO), 1, f) == 1)
		{
			GAMEBOARD* b = &game->board;
			b->bytesize = game->info.board_width * game->info.board_height * sizeof(GAME_CELLTYPE);
			b->buf = (GAME_CELLTYPE*)malloc(b->bytesize);
			if (b->buf != NULL)
				if (fread(b->buf, b->bytesize, 1, f) == 1)
					rc = GAME_RC_OK;
		}
		fclose(f);
	}
	return rc;
}


uint16_t save(PLAYERTURN* turn)
{
	FILE* f = fopen(GAME_TURNFILE, "wb");
	if (f == NULL)
		return GAME_RC_ERROR;
	int rc = fwrite(turn, sizeof(PLAYERTURN), 1, f) == 1 ? GAME_RC_OK : GAME_RC_ERROR;
	fclose(f);
	return rc;
}

GAME_CELLTYPE getat(PLAYERBOARD* game, GAME_POSTYPE row, GAME_POSTYPE col )
{
	if ( (row >= game->info.board_height) || (col >= game->info.board_width))
		return GAME_ERROR;
	size_t pos = row * game->info.board_width * sizeof(GAME_CELLTYPE) + col * sizeof(GAME_CELLTYPE);
	if (pos > game->board.bytesize)
		return GAME_ERROR;
	return  *((GAME_CELLTYPE*)(game->board.buf + pos));
}
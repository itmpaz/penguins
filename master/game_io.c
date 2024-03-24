#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <stdlib.h> 
#include "game_io.h"
#include "game_io_json.h"









uint16_t gm_fprint(GAME* game, FILE* f)
{
	if ((game->board.buf == NULL) || (game->board.bytesize == 0))
		return RC_ERROR_NOT_INITIALIZED;

	for (uint16_t i = 0; i < game->info.board_height; i++)
	{
		for (uint16_t j = 0; j < game->info.board_width; j++)
		{
			GAME_CELLTYPE e = gm_getat(game,  j,i);
			fprintf(f, "%c", gm_boardchar(e));

		}
		fprintf(f,"\n");
	}
	return GAME_RC_OK;
}

uint16_t gm_print(GAME* game)
{
	return gm_fprint(game, stdout);
}


uint16_t gm_loadplayerdataex(PLAYERBOARD* playerdata, FILE* f)
{
	if (fread(&(playerdata->info), sizeof(PLAYERINFO), 1, f) != 1)
		return RC_ERROR_FILEIO;
	int rc = gm_create(&(playerdata->board), playerdata->info.board_width, playerdata->info.board_height);
	if (fread(playerdata->board.buf, playerdata->board.bytesize, 1, f) != 1)
		return RC_ERROR_FILEIO;
	return GAME_RC_OK;
}


uint16_t gm_loadplayerdata(PLAYERBOARD* playerdata)
{
	FILE* f = fopen(GMIO_PLAYERDATAFILE, "rb");
	if (f == NULL)
		return RC_ERROR_FILEIO;
	int rc = gm_loadplayerdataex(playerdata, f);
	fclose(f);
	return rc;
}




uint16_t gm_saveplayerdataex(GAME* game, FILE* f)
{
	PLAYERINFO pi = { 0 };

	pi.board_height = game->info.board_height;
	pi.board_width = game->info.board_width;
	pi.player_id = game->info.player_turn;
	pi.turn_counter = game->info.turn_counter;
	pi.unused_penguins_count =  game->info.penguins_count -  gm_count( game, game->info.player_turn) ;

	if (fwrite(&pi, sizeof(PLAYERINFO), 1, f) != 1)
		return RC_ERROR_FILEIO;
	if (fwrite(game->board.buf, game->board.bytesize, 1, f) != 1)
		return RC_ERROR_FILEIO;
	return GAME_RC_OK;
}



uint16_t gm_saveplayerdata(GAME* game)
{
	FILE* f = fopen(GMIO_PLAYERDATAFILE, "wb");
	if (f == NULL)
		return RC_ERROR_FILEIO;
	int rc = gm_saveplayerdataex(game, f);
	fclose(f);
	return rc;
}






uint16_t gm_loadex(GAME* game, FILE* f)
{
	if (fread(&(game->info), sizeof(GAMEINFO), 1, f) != 1)
		return RC_ERROR_FILEIO;
	if (fread(&(game->json), sizeof(JSON), 1, f) != 1)
		return RC_ERROR_FILEIO;
	game->json.file = 0;
	int rc = gm_create(&(game->board), game->info.board_width, game->info.board_height);
	if (rc != GAME_RC_OK)
		return rc;
	if (fread(game->board.buf, game->board.bytesize, 1, f) != 1)
		return RC_ERROR_FILEIO;
	return GAME_RC_OK;
}




uint16_t gm_load(GAME* game)
{
	FILE* f = fopen(GMIO_GAMEFILE, "rb");
	if (f == NULL)
		return RC_ERROR_FILEIO;
	int rc = gm_loadex(game, f);
	fclose(f);
	return rc;
}




uint16_t gm_saveex(GAME* game, FILE* f)
{
	if (fwrite(&(game->info), sizeof(GAMEINFO), 1, f) != 1)
		return RC_ERROR_FILEIO;
	if (fwrite(&(game->json), sizeof(JSON), 1, f) != 1)
		return RC_ERROR_FILEIO;
	if (fwrite(game->board.buf, game->board.bytesize, 1, f) != 1)
		return RC_ERROR_FILEIO;
	return GAME_RC_OK;
}

uint16_t gm_save(GAME* game)
{
	FILE* f = fopen(GMIO_GAMEFILE, "wb");
	if (f == NULL)
		return RC_ERROR_FILEIO;
	int rc = gm_saveex(game,f);
	fclose(f);
	return rc;
}


uint16_t gm_loadturn(PLAYERTURN* turn)
{
	FILE* f = fopen(GMIO_TURNFILE, "rb");
	if (f == NULL)
		return RC_ERROR_FILEIO;
	int rc = fread(turn, sizeof(PLAYERTURN), 1, f) == 1 ? GAME_RC_OK : RC_ERROR_FILEIO;
	fclose(f);
	return rc;
}


uint16_t gm_saveturn(PLAYERTURN* turn)
{
	FILE* f = fopen(GMIO_TURNFILE, "wb");
	if (f == NULL)
		return RC_ERROR_FILEIO;
	int rc = fwrite(turn, sizeof(PLAYERTURN), 1, f)==1 ? GAME_RC_OK : RC_ERROR_FILEIO;
	fclose(f);
	return rc;
}









uint16_t gm_logclose(GAME* game)
{
	int rc;
	if (game->info.gameover == GAME_RC_GAMEOVER)
	{
		rc = json_file_stop(&game->json);
	}
	else
	{
		rc = json_file_pause(&game->json);
	}
	return (rc == JSON_OK) ? GAME_RC_OK :GAME_RC_ERROR;
}



uint16_t gm_logopen(GAME* game)
{
	int rc = json_file_open_continue(&game->json);
	return (rc == JSON_OK) ? GAME_RC_OK : GAME_RC_ERROR;
}


uint16_t gm_lognew(GAME* game)
{
	int rc = json_file_open_new(&game->json, GMIO_LOGFILE);
	json_dict_begin(&game->json,NULL);
	return (rc == JSON_OK) ? GAME_RC_OK : GAME_RC_ERROR;

}




uint16_t gm_logerror(GAME* game, uint16_t error_code, char* message)
{
	if (gm_logopen(game) != GAME_RC_OK)
		return RC_ERROR_FILEIO;

	fprintf(stderr, "Error %i: %s\n",error_code,message);
	if (game->info.errorcounter == 0)
	{
		json_end_root(&game->json);
		json_dict_begin(&game->json, "error");
		json_dict_int(&game->json, "code", error_code);
		json_dict_str(&game->json, "text", message);
		json_end(&game->json);
		game->info.errorcounter++;
	}
	return GAME_RC_OK;
}


uint16_t gm_logturn(GAME* game, PLAYERTURN* turn,uint16_t turnrc)
{
	if (gm_logopen(game) != GAME_RC_OK)
		return RC_ERROR_FILEIO;

	GAME_CELLTYPE playerid = game->info.player_turn;
	uint32_t score = game->info.score[game->info.player_turn - GAME_PLAYER_0];

	if (game->info.turn_counter == 0)
	{
		json_end_root(&game->json);
		json_array_begin(&game->json, "turns");
	}

	json_dict_begin(&game->json, NULL);

	json_dict_int(&game->json, "index", game->info.turn_counter);
	json_dict_int(&game->json, "result", turnrc);
	json_dict_int(&game->json, "player", playerid);
	json_dict_int(&game->json, "score", score);

	if (turn != NULL)
	{
		if ((turn->from_col == GAME_POS_NULL) || (turn->from_row == GAME_POS_NULL))
		{
			json_dict_str(&game->json, "type", "pass");
		}
		else
			if ((turn->to_col == GAME_POS_NULL) || (turn->to_row == GAME_POS_NULL))
			{
				json_dict_str(&game->json, "type", "place");
				json_dict_int(&game->json, "src_row", turn->from_row);
				json_dict_int(&game->json, "src_col", turn->from_col);
			}
			else
			{
				json_dict_str(&game->json, "type", "move");
				json_dict_int(&game->json, "src_row", turn->from_row);
				json_dict_int(&game->json, "src_col", turn->from_col);
				json_dict_int(&game->json, "dst_row", turn->to_row);
				json_dict_int(&game->json, "dst_col", turn->to_col);
			}
	}
	json_end(&game->json);



	return GAME_RC_OK;
}


uint16_t gm_logboard(GAME* game, char* key)
{
	if (gm_logopen(game) != GAME_RC_OK)
		return RC_ERROR_FILEIO;

	json_end_root(&game->json);
	json_array_begin(&game->json, key);

	for (uint16_t i = 0; i < game->info.board_height; i++)
	{
		json_array_begin(&game->json, NULL);
		for (uint16_t j = 0; j < game->info.board_width; j++)
			json_array_byte(&game->json, gm_getat(game, j, i) );
		json_end(&game->json);

	}
	json_end(&game->json);
	return GAME_RC_OK;
}


uint16_t gm_logresults(GAME* game)
{
	if (gm_logopen(game) != GAME_RC_OK)
		return RC_ERROR_FILEIO;

	json_end_root(&game->json);
	json_array_begin(&game->json, "results");
	
	for (uint16_t i = 0; i < game->info.players_count; i++)
	{
		json_dict_begin(&game->json, NULL);
		json_dict_int(&game->json, "player", i + GAME_PLAYER_0);
		json_dict_int(&game->json, "score", game->info.score[i]);
		json_end(&game->json);
	}
	json_end(&game->json);

	return GAME_RC_OK;

}


uint16_t gm_logparams(GAME* game)
{
	if (gm_logopen(game) != GAME_RC_OK)
		return RC_ERROR_FILEIO;

	json_end_root(&game->json);
	json_dict_begin(&game->json, "game");
	json_dict_int(&game->json, "width", game->info.board_width);
	json_dict_int(&game->json, "height", game->info.board_height);
	json_dict_int(&game->json, "players", game->info.players_count);
	json_dict_int(&game->json, "penguins", game->info.penguins_count);
	json_dict_int(&game->json, "fishes", game->info.max_fish_count);
	json_end(&game->json);


	return GAME_RC_OK;

}

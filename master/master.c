#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> 


#include "game_master.h"
#include "game_io.h"
#include "game_io_json.h"



//-n 10 5 2 3
//-c


void printusage(char* argv0)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "1) %s -n <board width> <board height> <players count> <penguins count>\n", argv0);
	fprintf(stderr, "\tStart new game\n");
	fprintf(stderr, "2) %s -c \n", argv0);
	fprintf(stderr, "\tContinue current game\n");

}






int main(int argc, char* argv[])
{

	if ((argc < 2) || (argv[1][0]!='-')) 
	{
		printusage(argv[0]);
		return RC_ERROR_WRONG_CMD;
	}



	GAME game = { 0 };

	if ((argv[1][1] == 'n') && (argc>=6))
	{

		game.info.board_width = atoi(argv[2]);
		game.info.board_height = atoi(argv[3]);
		game.info.players_count = atoi(argv[4]);
		game.info.penguins_count = atoi(argv[5]);
		game.info.max_fish_count = GAME_FISH_MAX;

		int rc = gm_new(&game);

		if (rc != GAME_RC_OK)
		{
			gm_logerror(&game, rc, "game create error");
			return RC_GM_STOP_GAME_CREATE_ERROR;

		}
		gm_lognew(&game);
		gm_logparams(&game);
		gm_logboard(&game,"start");
		gm_print(&game);

	} else if (argv[1][1] == 'c')
	{
		int gameloadrc = gm_load(&game);

		if (gameloadrc != GAME_RC_OK)
		{
			gm_logerror(&game, gameloadrc, "game load error");
			return RC_GM_STOP_GAME_LOAD_ERROR;
		}

		PLAYERTURN t = { 0 };
		int turnloadrc = gm_loadturn(&t);
		if (turnloadrc == GAME_RC_OK)
		{
			int turnrc = gm_turn(&game, &t);
			gm_logturn(&game, &t,turnrc);
			gm_turnnext(&game, turnrc);

		}
		else
		{
			//sprintf(game.turnerrormsg, "Player turn file '%s' read error.", GAME_TURNFILE);
			//gm_logerror(&game, turnloadrc, game.turnerrormsg);
			gm_logturn(&game, NULL, GM_TURN_NODATA);
			gm_turnnext(&game, GM_TURN_NODATA);
		}

		//gm_logboard(&game, 0, "turn");



	}
	else
	{
		printusage(argv[0]);
		return RC_ERROR_WRONG_CMD;
	}


	game.info.gameover = gm_checkgameover(&game);

	if (game.info.gameover != GAME_RC_OK)
	{
		gm_logresults(&game);
		gm_logboard(&game, "stop");
		gm_print(&game);
	} else
	{
		gm_saveplayerdata(&game);
	}


	gm_logclose(&game);
	gm_save(&game);
	gm_del(&game);

	return (game.info.gameover== GAME_RC_GAMEOVER) ?  RC_GM_STOP_GAMEOVER : (RC_GM_RUN_PLAYER + game.info.player_turn - GAME_PLAYER_0);
}

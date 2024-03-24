#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "penguins.h"
#include "penguinutils.h"

PLAYERBOARD _game = { 0 };
PLAYERTURN  _turn = { 0 };

int main()
{
	load(&_game);

    //your code here

	save(&_turn);
	clean_up(&_game);

	return GAME_RC_OK;
}

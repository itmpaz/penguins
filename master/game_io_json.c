#define _CRT_SECURE_NO_WARNINGS


#include "game_io_json.h"
#include <assert.h>
#include <string.h>





int json_file_stop(JSON* json)
{
	if (json->file != 0)
	{
		json_end_all(json);
		fclose(json->file);
		json->file = 0;
		json->filename[0] = 0;
	}
	return JSON_OK;
}

int json_file_pause(JSON* json)
{
	if (json->file != 0)
	{
		fclose(json->file);
		json->file = 0;
	}
	return JSON_OK;
}




int json_file_openex(JSON* json, char* filename, char* fileopenparam)
{
	json->file = fopen(filename, fileopenparam);
	strcpy(json->filename, filename);
	if (json->file == 0)
	{
		fprintf(stderr, "Log file '%s' open error\n", filename);
		return JSON_ERROR;
	}

	return JSON_OK;

}

int json_file_open_continue(JSON* json)
{
	if (strlen(json->filename) == 0)
		return JSON_ERROR;

	if (json->file != 0)
		return JSON_OK;
	
	return json_file_openex(json, json->filename, "a");
}


int json_file_open_new(JSON* json,char* filename)
{
	if (strlen(filename) == 0)
		return JSON_ERROR;

	if (json->file != 0)
		return JSON_OK;

	return json_file_openex(json, filename, "w");
}



















void json_print_int(FILE* f, int n)
{
	fprintf(f, "%i", n);
}


void json_print_char(FILE* f, char c)
{
	fprintf(f, "%c", c);
}


void json_print_charasstr(FILE* f, char c)
{
	fprintf(f, "\"%c\"", c);
}

void json_print_str(FILE* f, char* str)
{
	fprintf(f, "\"%s\"", str);
}

void json_print_hexbyte(FILE* f, uint8_t n)
{
	//fprintf(f, "0x%02X", n);
	fprintf(f, "%3i", n);
}


char json_inversetype(char type)
{
	if (type == '{')
		return '}';
	if (type == '[')
		return ']';
	if (type == '(')
		return ')';
	return type;
}

void json_comma(JSON* json)
{
	if (json->lvldat[json->lvl].count != 0)
	{
		json_print_char(json->file, JSON_COMMA);
		json_print_char(json->file, json->lvldat[json->lvl].linefeed);
	}
}

void json_inc(JSON* json)
{
	json->lvldat[json->lvl].count++;
}




void json_begin(JSON* json, char type,char linefeed)
{
	json->lvl += 1;
	assert(json->lvl < JSON_LVL_MAX);

	json->lvldat[json->lvl].type = type;
	json->lvldat[json->lvl].count = 0;
	json->lvldat[json->lvl].linefeed = linefeed;

	json_print_char(json->file, type);
	json_print_char(json->file, linefeed );

}
void json_end(JSON* json)
{
	json_print_char(json->file, json_inversetype(json->lvldat[json->lvl].type));
	json_print_char(json->file, json->lvldat[json->lvl].linefeed );
	json->lvldat[json->lvl].type = 0;
	json->lvldat[json->lvl].count = 0;
	json->lvldat[json->lvl].linefeed = 0;
	assert(json->lvl != 0);
	json->lvl -= 1;

}





void json_dict_int(JSON* json, char* key, int n)
{
	json_comma(json);
	json_print_str(json->file, key);
	json_print_char(json->file, JSON_EQ);
	json_print_int(json->file, n);
	json_inc(json);
}


void json_dict_str(JSON* json, char* key, char* str)
{
	json_comma(json);
	json_print_str(json->file, key);
	json_print_char(json->file, JSON_EQ);
	json_print_str(json->file, str);
	json_inc(json);
}

void json_dict_char(JSON* json, char* key, char c)
{
	json_comma(json);
	json_print_str(json->file, key);
	json_print_char(json->file, JSON_EQ);
	json_print_charasstr(json->file, c);
	json_inc(json);
}


void json_dict_begin(JSON* json,  char* key)
{
	json_comma(json);
	if (key != NULL)
	{
		json_print_str(json->file, key);
		json_print_char(json->file, JSON_EQ);
	}
	json_inc(json);
	json_begin(json, '{',JSON_LINEFEED);
}


void json_end_all(JSON* json)
{
	while (json->lvl != 0)
		json_end(json);
}

void json_end_root(JSON* json)
{
	while (json->lvl != 1)
		json_end(json);

	json_print_char(json->file, JSON_LINEFEED);
	json_print_char(json->file, JSON_LINEFEED);

}



void json_array_begin(JSON* json,char* key)
{
	char lf = JSON_NOLINEFEED;
	json_comma(json);
	if (key != NULL)
	{
		json_print_str(json->file, key);
		json_print_char(json->file, JSON_EQ);
		lf = JSON_LINEFEED;
	}
	json_inc(json);
	json_begin(json, '[', lf);
}



void json_array_byte(JSON* json,uint8_t n)
{
	json_comma(json);
	json_print_hexbyte(json->file, n);
	json_inc(json);
}






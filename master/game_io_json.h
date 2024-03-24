#ifndef GAME_IO_JSON_H
#define GAME_IO_JSON_H


#include <stdio.h>
#include <stdint.h>



#define JSON_LVL_MAX  10
#define JSON_COMMA ','
#define JSON_LINEFEED '\n'
#define JSON_NOLINEFEED ' '
#define JSON_EQ ':'



#define JSON_OK 0
#define JSON_ERROR 1

#define JSON_FILENAME_MAX  1024


typedef struct tagJSONLEVELDATA
{
	char type;
	size_t count;
	char linefeed;

} JSONLEVELDATA;



typedef struct tagJSON
{
	FILE* file;
	char filename[JSON_FILENAME_MAX];
	uint16_t lvl;
	JSONLEVELDATA lvldat[JSON_LVL_MAX];


} JSON;



int json_file_stop(JSON* json);
int json_file_pause(JSON* json);
int json_file_open_continue(JSON* json);
int json_file_open_new(JSON* json, char* filename);



void json_begin(JSON* json, char type, char linefeed);
void json_end(JSON* json);

void json_dict_int(JSON* json, char* key, int n);
void json_dict_str(JSON* json, char* key, char* str);
void json_dict_begin(JSON* json, char* key );
void json_end_all(JSON* json);
void json_end_root(JSON* json);
void json_array_begin(JSON* json, char* key);
void json_array_byte(JSON* json, uint8_t n);
void json_dict_char(JSON* json, char* key, char c);




#endif




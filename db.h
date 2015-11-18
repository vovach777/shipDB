#pragma once
#include <stdbool.h>
#include <stdlib.h>
#pragma pack(push,1)
typedef struct ship_tag {
   bool isDeleted;
   int year;
   char name[64];
} *ship_ptr_t, ship_t;
#pragma pack(pop)
#define SHIP_SIZE sizeof(ship_t)

typedef size_t cursor_t;

bool db_open(char * fileName);
void db_close();

void db_first();
void db_next();
void db_prev();
void db_last();
void db_set_cursor(cursor_t c);
cursor_t db_get_cursor();
cursor_t db_get_begin();


cursor_t db_get_end();
bool db_is_deleted();


ship_ptr_t db_ship(); //last load or save

void db_delete();
void db_new();
void db_save();
void db_load();
void db_copy();
void db_paste();

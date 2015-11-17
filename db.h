#include <stdbool.h>
typedef struct ship_tag {
   bool isDeleted;
   int your;
   char name[64];
} *ship_ptr_t, ship_t;

#define SHIP_SIZE sizeof(ship_t)

typedef size_t cursor_t;

bool db_open(char * fileName);
void db_close();

cursor_t db_first();
cursor_t db_next();
cursor_t db_prev();
bool     db_is_deleted();

ship_ptr_t db_curr();
void db_delete();
void db_save();
void db_load();

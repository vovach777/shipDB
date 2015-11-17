#include <stdlib.h>
#include <strings.h>
#include "db.h"

FILE * file = NULL;
ship_t ship, clipboard;
ship_ptr_t ship_ptr = NULL;
cursor_t cursor = 0;
cursor_t max_cursor = 0;
bool hasclipboard = false;

static void error(char * message) {
  fprintf(stderr,"ERROR: %s!\n", message);
  ship_ptr = NULL;
}

bool db_open(char * fileName) {
    cursor = 0;
    max_cursor = 0;
    ship_ptr = NULL;
    memset(&ship,0, SHIP_SIZE)
    file = fopen(fileName, "r+b");
    if (file == NULL)
       file = fopen(fileName, "w+b");
    if ((file != NULL) &&
       (fseek(file, 0, SEEK_END) == 0) {
      max_cursor = ftell(file);
    }
    return (file != NULL);
}

void db_close() {
  if (file)
    fclose(file);
  file = NULL;
}

void db_first() {
  cursor = 0;
  return cursor;
}

void db_last() {
  cursor = (max_cursor >= SHIP_SIZE) ? (max_cursor-SHIP_SIZE) : 0;
  return (cursor);
}

void db_next() {
   cursor = (cursor+SHIP_SIZE <= max_cursor) ? cursor+SHIP_SIZE : max_cursor;
}
void db_prev() {
  cursor = (cursor >= SHIP_SIZE) ? cursor-SHIP_SIZE : 0;
}

void db_set_cursor(cursor_t c) {
  c -= c % SHIP_SIZE;//fix cursor if needed
  cursor = (c <= max_cursor) ? c : max_cursor;
}
cursor_t db_get_begin() {
  return 0;
}
cursor_t db_get_end() {
  return max_cursor;
}

cursor_t db_get_cursor() {
  return cursor;
}

bool db_is_deleted() {
   return ( (ship_ptr == NULL) || (ship_ptr->isDeleted) );
}

ship_ptr_t db_ship() {
   return ship_ptr;
}
void db_delete() {
  if (ship_ptr) {
      ship_ptr->isDeleted = true;
      db_save();
  }
}
void db_save() {
  if (ship_ptr) {
    if ((FILE != NULL) &&
        (fseek(file, cursor, SEEK_SET) == 0) &&
        (ftell(file) == cursor) &&
        (fwrite(ship_ptr,SHIP_SIZE,1,file) == 1)) {
          if (max_cursor == cursor)
              max_cursor += SHIP_SIZE; //extend
        }
         error("can not save");
  }
}
void db_load() {
  ship_ptr = NULL;
    if ((FILE) &&
       (fseek(file, cursor, SEEK_SET) == 0) &&
        (ftell(file) == cursor) &&
        (fread(&ship,SHIP_SIZE,1,file) == 1))
         ship_ptr = &ship;
      else
         error("can not load");
}

void db_copy(){
  if (ship_ptr)  {
      memcpy(&clipboard, &ship);
      hasclipboard = true;
  }
}
void db_paste() {
   if (hasclipboard) {
      ship_ptr = &clipboard;
      db_save();
   }
}

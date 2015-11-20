//gcc -fexec-charset=cp866 -finput-charset=utf-8 app.c db.c
#include <stdio.h>
#include <string.h>
#include "db.h"


void show() {
   if (db_ship()) {
            printf("+----------\n");
            printf("| Название: %s\n", db_ship()->name);
            printf("| Построен: %d год\n", db_ship()->year);
            printf("+----------\n");
   }
   else
    puts("== нет записи для просмотра ==");
}

enum CID {
    NOPE, HELP, OPEN, CLOSE, FIRST, NEXT, PREV, REMOVE, COPY, PASTE, EXIT, SHOW,
    NEW, SAVE, REFRESH,
    SET_NAME,
    SET_YEAR
};

typedef struct Command_tag {
   char * command;
   char * help;
   enum CID cid;
} cmd_t;

#define CMD_COUNT 15
cmd_t commands[CMD_COUNT] = {
    {"help", "Выводит эту помощь:", HELP},
    {"open", "Открыть базу", OPEN},
    {"close", "Закрыть баху", CLOSE},
    {"first", "Перейти к первой записи", FIRST},
    {"next", "Перейти к следующей записи", NEXT},
    {"prev", "Перейти к предыдущей записи", PREV},
    {"remove", "Удалить запись", REMOVE},
    {"copy", "Скопироваться запись в буффер обмена",COPY},
    {"save", "Сохранить отредактированную запись", SAVE},
    {"show", "Показать текущую запись", SHOW},
    {"load", "Перечитать запись", REFRESH},
    {"set_name", "Установить имя", SET_NAME},
    {"set_year", "Установить год", SET_YEAR},  
    {"new","создать новую запись", NEW},
    {"exit","Закончить работу с программой", EXIT}
};

void show_help() {
    for (int i=0; i<CMD_COUNT; i++) {
        printf("%-8.8s %s\n", commands[i].command, commands[i].help);
    }
}

char command_buff[64];

char * input_str() {
fgets(command_buff, sizeof(command_buff), stdin);
    int i = strlen(command_buff)-1; 
      while ((i >= 0) && ((command_buff[i]=='\n')||(command_buff[i]=='\r')))
         command_buff[i--] = '\0';
    return command_buff;
}

enum CID input() {    
    char * cmd = input_str();       
    for (int i=0; i<CMD_COUNT; i++) {
       if (strcmp(commands[i].command, cmd)==0) 
           return(commands[i].cid);
    }
    return NOPE;
 }


void process_commands(enum CID cid) {
    switch (cid) {
        case NOPE:
              puts("-- команда не распознана --");
              break;
        case SET_NAME:              
              if (db_ship()) {
                printf("Имя>");
                strncpy(db_ship()->name,input_str(),64);
                show();
          }
            break;
        case SET_YEAR:
              if (db_ship()) {
                printf("Год>");
                db_ship()->year = strtoul( input_str(),NULL, 10);
                show();
          }
            break;
        case REFRESH:
             db_load();
        case SHOW:
            show();
            break;

        case NEW:
             db_new();
             show();            
             break;
        case REMOVE:
                db_delete();        
                break;
        case SAVE:
              db_save();
             break; 

        case HELP:
              show_help();
              break;
        case OPEN:
                db_open("ships.db");
                break;
        case CLOSE:
                db_close();
                break;
        case  FIRST:
                first:
                db_first();
                while (db_get_cursor() != db_get_end()) {                   
                   db_load();
                   if (!db_is_deleted()) {
                       show();
                       return;
                   }                   
                   db_next();
                }
                puts("-- в базе нет записей. --");                
                break;
        case  NEXT: 
                db_next();
                while (db_get_cursor() != db_get_end()) {
                    db_load();
                    if (!db_is_deleted()) {
                       show();
                       return;
                   }
                   db_next();
                }
                puts("-- больше записей нет. перемотка в начало. --");
                goto first;                 
        case PREV:              
                while (db_get_cursor() != db_get_begin()) {
                   db_prev();
                   db_load();
                   if (!db_is_deleted()) {
                       show();
                       return;
                   }
                }
                puts("-- первая запись --");                
                goto first;             
                break;
        default:
            puts("== команда не обработана ==");

    }    
}


int main(void) {
    enum CID cid;
    puts("Добропожаловать в программу базы кораблей!");
    db_ship();
    puts("вот команды, которые вам понадобятся для работы:");
    show_help();
    
    while (1) {
       putchar('>'); //fflush(stdout);
       cid = input();
        if (cid == EXIT)
              return 0;
       process_commands(cid);
}

}
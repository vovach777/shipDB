//gcc -fexec-charset=cp866 -finput-charset=utf-8 app.c db.c
#include <stdio.h>
#include <strings.h>
#include "db.h"


void show() {
   if (db_ship()) {
       printf("корабль: \"%s\" (%d)\n", db_ship()->name, db_ship()->year);
   }
}

enum CID {
    NOPE, HELP, OPEN, CLOSE, FIRST, NEXT, PREV, EDIT, REMOVE, COPY, PASTE, EXIT 
};

typedef struct Command_tag {
   char * command;
   char * help;
   enum CID cid;
} cmd_t;

#define CMD_COUNT 10
cmd_t commands[CMD_COUNT] = {
    {"help", "Выводит эту помощь:", HELP},
    {"open", "Открыть базу", OPEN},
    {"close", "Закрыть баху", CLOSE},
    {"first", "Перейти к первой записи", FIRST},
    {"next", "Перейти к следующей записи", NEXT},
    {"prev", "Перейти к предыдущей записи", PREV},
    {"edit", "Режим редактирования", EDIT},
    {"remove", "Удалить запись", REMOVE},
    {"copy", "Скопироваться запись в буффер обмена",COPY},
    {"exit","Закончить работу с программой", EXIT}
};

void show_help() {
    for (int i=0; i<CMD_COUNT; i++) {
        printf("%-8.8s %s\n", commands[i].command, commands[i].help);
    }
}


char command_buff[16];


enum CID input() {    
    fgets(command_buff, sizeof(command_buff), stdin);
    int i = strlen(command_buff)-1; 
      while ((i >= 0) && ((command_buff[i]=='\n')||(command_buff[i]=='\r')))
         command_buff[i--] = '\0';

    for (int i=0; i<CMD_COUNT; i++) {
       if (strcmp(commands[i].command, command_buff)==0) 
           return(commands[i].cid);
    }
    return NOPE;
 }


int main(void) {
    
    puts("Добропожаловать в программу базы кораблей!");
    puts("вот команды, которые вам понадобятся для работы:");
    show_help();
    
    while (1)
    switch (input()) {
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
    	           	   break;
    	           }
    	           db_next();
	    	    }
	    	    puts("-- в базе нет записей. --");
		        break;
    	case  NEXT:    			
    			while (db_get_cursor() != db_get_end()) {	    	        
    				db_next();
    	            db_load();
    	            if (!db_is_deleted()) {
    	           	   show();
    	           	   break;
    	           }
	    	    }
	    	    puts("-- больше записей нет. перемотка в начало. --");
	    	    goto first;	
		        break;
		case PREV:				
    			while (db_get_cursor() != db_get_begin()) {
    			   db_prev();
    	           db_load();
    	           if (!db_is_deleted()) {
    	           	   show();
    	           	   break;
    	           }
	    	    }
	    	    puts("-- первая запись --");
	    	    goto first;	    	    
		        break;
		case EXIT:
		        return 0; 

    }

}
#ifndef CREATE_NEW_GAME_H
#define CREATE_NEW_GAME_H
#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "user.h"
#include "room.h"
#include "create_map.h"
#define START 6

#define MAX_SIZE 50

void create_new_game_func (User *user) {
    create_map(user);
    char gamer = toupper((user -> username)[0]);
    int c;
    clear();
    for (int i = 0; i < 60; i++) {
        for (int j = 0; j < 200; j++) {
            if ((user -> visible)[user -> current_floor][i][j]) {
                mvaddch(START + i, START + j, (user -> map_screen_char)[user -> current_floor][i][j]);
            }
        }
    }
    attron(COLOR_PAIR(2) | A_BLINK);
    mvaddch(user -> current_y + START, user -> current_x + START, gamer);
    attroff(COLOR_PAIR(2) | A_BLINK);
    refresh();
    while ((c = getch()) != KEY_F(1)) {
        //instruction
        clear();
        for (int i = 0; i < 60; i++) {
            for (int j = 0; j < 200; j++) {
                if ((user -> visible)[user -> current_floor][i][j]) {
                    mvaddch(START + i, START + j, (user -> map_screen_char)[user -> current_floor][i][j]);
                }
            }
        }
        attron(COLOR_PAIR(2) | A_BLINK);
	    mvaddch(user -> current_y + START, user -> current_x + START, gamer);
	    attroff(COLOR_PAIR(2) | A_BLINK);
        refresh();
    }
    
}



#endif
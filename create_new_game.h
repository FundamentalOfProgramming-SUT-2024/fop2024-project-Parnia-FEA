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
    curs_set(0);
    char gamer = toupper((user -> username)[0]);
    int c;
    clear();
    for (int i = 0; i < 60; i++) {
        for (int j = 0; j < 200; j++) {
            if ((user -> visible)[user -> current_floor][i][j]) {
                if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                    attron(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                    if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                        attron(A_BLINK);
                    }
                }
                if ((user -> map_screen_char)[user -> current_floor][i][j] == '-') {
                    mvaddch(START + i, START + j, '_');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == '!') {
                    mvaddch(START + i, START + j, '|');
                }
                else {
                    mvaddch(START + i, START + j, (user -> map_screen_char)[user -> current_floor][i][j]);
                }
                if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                    attroff(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                    if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                        attroff(A_BLINK);
                    }
                }
            }
        }
    }
    attron(COLOR_PAIR(2) | A_BLINK);
    mvaddch(user -> current_y + START, user -> current_x + START, gamer);
    attroff(COLOR_PAIR(2) | A_BLINK);
    refresh();
    while ((c = getch()) != KEY_F(1)) {
        int flag = 0;
        //instruction
        if (c == 'j') {
            //up
            if (user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] != ' ' && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] != '|' && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] != '_') {
                (user -> current_y)--;
                flag = 1;
            }
        }
        else if (c == 'k') {
            //down
            if (user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] != ' ' && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] != '|' && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] != '_') {
                (user -> current_y)++;
                flag = 1;
            }
        }
        else if (c == 'h') {
            //left
            if (user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] != ' ' && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] != '|' && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] != '_') {
                (user -> current_x)--;
                flag = 1;
            }
        }
        else if (c == 'l') {
            //right
            if (user -> current_x < 199 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] != ' ' && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] != '|' && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] != '_') {
                (user -> current_x)++;
                flag = 1;
            }
        }
        else if (c == 'y') {
            //up left
            if (user -> current_x > 0 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == '.') {
                (user -> current_x)--;
                (user -> current_y)--;
                flag = 1;
            }
        }
        else if (c == 'u') {
            //up right
            if (user -> current_x < 199 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == '.') {
                (user -> current_x)++;
                (user -> current_y)--;
                flag = 1;
            }
        }
        else if (c == 'b') {
            //down left
            if (user -> current_x > 0 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == '.') {
                (user -> current_x)--;
                (user -> current_y)++;
                flag = 1;
            }
        }
        else if (c == 'n') {
            //down right
            if (user -> current_x < 199 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] != '.') {
                (user -> current_x)++;
                (user -> current_y)++;
                flag = 1;
            }
        }
        if ((user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] > 0) {
            Room *current_room = (user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] - 1];
            for (int i = current_room -> uly; i < current_room -> uly + current_room -> height; i++) {
                for (int j = current_room -> ulx; j < current_room -> ulx + current_room -> width; j++) {
                    (user -> visible)[user -> current_floor][i][j] = 1;
                }
            }
        }
        (user -> visible)[user -> current_floor][user -> current_y][user -> current_x] = 1;
        if (flag) {
            clear();
            for (int i = 0; i < 60; i++) {
                for (int j = 0; j < 200; j++) {
                    int flag1 = 0;
                    if ((user -> map_screen)[user -> current_floor][i][j] < 0 || (user -> map_screen_char)[user -> current_floor][i][j] == '+') {
                        if (user -> current_y == i) {
                            if (j > user -> current_x && j <= user -> current_x + 5) {
                                int flag2 = 1;
                                for (int k = user -> current_x + 1; k < j; k++) {
                                    if ((user -> map_screen)[user -> current_floor][i][k] > -1) {
                                        flag2 = 0;
                                    }
                                }
                                if (flag2) {
                                    flag1 = 1;
                                }
                            }
                            if (j < user -> current_x && j >= user -> current_x - 5) {
                                int flag2 = 1;
                                for (int k = user -> current_x - 1; k > j; k--) {
                                    if ((user -> map_screen)[user -> current_floor][i][k] > -1) {
                                        flag2 = 0;
                                    }
                                }
                                if (flag2) {
                                    flag1 = 1;
                                }
                            }
                        }
                        if (user -> current_x == j) {
                            if (i > user -> current_y && i <= user -> current_y + 5) {
                                int flag2 = 1;
                                for (int k = user -> current_y + 1; k < i; k++) {
                                    if ((user -> map_screen)[user -> current_floor][k][j] > -1) {
                                        flag2 = 0;
                                    }
                                }
                                if (flag2) {
                                    flag1 = 1;
                                }
                            }
                            if (i < user -> current_y && i >= user -> current_y - 5) {
                                int flag2 = 1;
                                for (int k = user -> current_y - 1; k > i; k--) {
                                    if ((user -> map_screen)[user -> current_floor][k][j] > -1) {
                                        flag2 = 0;
                                    }
                                }
                                if (flag2) {
                                    flag1 = 1;
                                }
                            }
                        }
                    }
                    if ((user -> visible)[user -> current_floor][i][j]) {
                        flag1 = 1;
                    }
                    if (flag1) {
                        if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                            attron(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                            if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                                attron(A_BLINK);
                            }
                        }
                        if ((user -> map_screen_char)[user -> current_floor][i][j] == '-') {
                            mvaddch(START + i, START + j, '_');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == '!') {
                            mvaddch(START + i, START + j, '|');
                        }
                        else {
                            mvaddch(START + i, START + j, (user -> map_screen_char)[user -> current_floor][i][j]);
                        }
                        if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                            attroff(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                            if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                                attroff(A_BLINK);
                            }
                        }
                    }
                }
            }
            attron(COLOR_PAIR(2) | A_BLINK);
            mvaddch(user -> current_y + START, user -> current_x + START, gamer);
            attroff(COLOR_PAIR(2) | A_BLINK);
            refresh();
        }
        if (user -> current_x == user -> end_x && user -> current_y == user -> end_y) {
            sleep(2);
            clear();
            refresh();
            attron(COLOR_PAIR(2) | A_BLINK);
            mvprintw(29, 104, "YOU HAVE WON THE GAME!");
            attroff(COLOR_PAIR(2) | A_BLINK);
            refresh();
        }
    }
    
}



#endif
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

int move_directly(User *, int, int);
int move_indirectly(User *, int, int);

void create_new_game_func (User *user) {
    create_map(user);
    curs_set(0);
    user -> health = 100;
    user -> gold = 0;
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
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 't') {
                    mvaddch(START + i, START + j, '.');
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
    mvprintw(2, 0, "health");
    for (int i = 0; i < (user -> health); i++) {
        attron(COLOR_PAIR(7));
        mvprintw(2, i + 7, " ");
        attroff(COLOR_PAIR(7));
    }
    refresh();
    while ((c = getch()) != KEY_F(1)) {
        int flag = 0;
        int flag_stair = 0;
        //instruction
        if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '>' && c == '>') {
            (user -> current_floor)++;
            user -> current_x = (user -> in_staircase)[user -> current_floor] -> x;
            user -> current_y = (user -> in_staircase)[user -> current_floor] -> y;
            flag = 1;
            flag_stair = 1;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '<' && c == '<') {
            (user -> current_floor)--;
            user -> current_x = (user -> out_staircase)[user -> current_floor] -> x;
            user -> current_y = (user -> out_staircase)[user -> current_floor] -> y;
            flag = 1;
            flag_stair = 1;
        }
        else if (c == 'j') {
            //up
            flag = move_directly(user, 0, -1);
        }
        else if (c == 'k') {
            //down
            flag = move_directly(user, 0, 1);
        }
        else if (c == 'h') {
            //left
            flag = move_directly(user, -1, 0);
        }
        else if (c == 'l') {
            //right
            flag = move_directly(user, 1, 0);
        }
        else if (c == 'y') {
            //up left
            flag = move_indirectly(user, -1, -1);
        }
        else if (c == 'u') {
            //up right
            flag = move_indirectly(user, 1, -1);
        }
        else if (c == 'b') {
            //down left
            flag = move_indirectly(user, -1, 1);
        }
        else if (c == 'n') {
            //down right
            flag = move_indirectly(user, 1, 11);
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
            if (flag_stair) {
                mvprintw(0, 0, "New Floor!");
                refresh();
            }
            for (int i = 0; i < 60; i++) {
                for (int j = 0; j < 200; j++) {
                    int flag1 = 0;
                    if ((user -> map_screen)[user -> current_floor][i][j] < 0) {
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
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 't') {
                            mvaddch(START + i, START + j, '.');
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
            mvprintw(2, 0, "health");
            for (int i = 0; i < (user -> health); i++) {
                attron(COLOR_PAIR(7));
                mvprintw(2, i + 7, " ");
                attroff(COLOR_PAIR(7));
    }
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
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '>') {
            mvprintw(0, 0, "If you want to use staircase to go to the next floor, press '>'!");
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '<') {
            mvprintw(0, 0, "If you want to use staircase to go to the previous floor, press '<'!");
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '!') {
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '?';
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 't') {
            (user -> health)--;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '^';
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '^') {
            (user -> health)--;
        }
    }
    
}

int move_directly(User *user, int x, int y) {
    if (user -> current_y + y >= 0 && user -> current_y + y < 60 && user -> current_x + x >= 0 && user -> current_x + x < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != ' ' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '|' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '_' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'O') {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    return 0;
}

int move_indirectly(User *user, int x, int y) {
    if (user -> current_y + y >= 0 && user -> current_y + y < 60 && user -> current_x + x >= 0 && user -> current_x + x < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != ' ' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '|' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '_' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'O' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '#' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '-' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '!') {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    return 0;
}


#endif
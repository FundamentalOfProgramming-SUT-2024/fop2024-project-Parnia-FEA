#ifndef CREATE_NEW_GAME_H
#define CREATE_NEW_GAME_H
#include <curses.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "user.h"
#include "food_menu.h"
#include "enchant_menu.h"
#include "room.h"
#include "create_map.h"
#include "weapon_menu.h"
#define START 6

#define MAX_SIZE 50

struct thread_args {
    int *health;
    int *hunger;
    int *end;
};

int move_directly(User *, int, int, int);
int move_indirectly(User *, int, int, int);
void print_screen(User *, int, char);
void *hunger_rate(void *);
void *health_rate(void *);

void create_new_game_func (User *user) {
    create_map(user);
    curs_set(0);
    user -> visible_mode = 0;
    user -> food = 0;
    user -> health = 100;
    user -> hunger = 20;
    user -> gold = 0;
    (user -> enchant_menu)[0] = 0; //Health
    (user -> enchant_menu)[1] = 0; //Speed
    (user -> enchant_menu)[2] = 0; //Damage
    (user -> weapon_menu)[0] = 1; //Hammer
    (user -> weapon_menu)[1] = 0; //Dagger
    (user -> weapon_menu)[2] = 0; //Wand
    (user -> weapon_menu)[3] = 0; //Normal Arrow
    (user -> weapon_menu)[4] = 0; //Hocho
    int end = 0;
    pthread_t thread_hunger;
    pthread_t thread_health;
    struct thread_args args;
    args.health = &(user -> health);
    args.hunger = &(user -> hunger);
    args.end = &(end);
    pthread_create(&thread_hunger, NULL, hunger_rate, (void *)(&args));
    pthread_create(&thread_health, NULL, health_rate, (void *)(&args));
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
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F354");
                }
                else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && (user -> current_y != i || user -> current_x != j)) {
                    if ((user -> map_screen_char)[user -> current_floor][i][j] == '0') {
                        mvprintw(START + i, START + j, "%s", "\U0001F528");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '1') {
                        mvprintw(START + i, START + j, "%s", "\U0001F5E1");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '2') {
                        attron(COLOR_PAIR(4));
                        mvprintw(START + i, START + j, "%s", "\U0001327B");
                        attroff(COLOR_PAIR(4));
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '3') {
                        mvprintw(START + i, START + j, "%s", "\U0001F3F9");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '4') {
                        mvprintw(START + i, START + j, "%s", "\U0001F52A");
                    }
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == '$') {
                    attron(COLOR_PAIR(4));
                    mvprintw(START + i, START + j, "%s", "\U00002735");
                    attroff(COLOR_PAIR(4));
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'G') {
                    attron(COLOR_PAIR(2));
                    mvprintw(START + i, START + j, "%s", "\U00002742");
                    attron(COLOR_PAIR(2));
                }
                else if (j == 0 || ((user -> map_screen_char)[user -> current_floor][i][j - 1] != 'B' && isdigit((user -> map_screen_char)[user -> current_floor][i][j - 1]) == 0)){
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
        if (user -> health < 20) {
            attron(COLOR_PAIR(8));
        }
        else {
            attron(COLOR_PAIR(7));
        }
        mvprintw(2, i + 7, " ");
        if (user -> health < 20) {
            attroff(COLOR_PAIR(8));
        }
        else {
            attroff(COLOR_PAIR(7));
        }
    }
    attron(COLOR_PAIR(4));
    mvprintw(3, 0, "gold %d", user -> gold);
    attroff(COLOR_PAIR(4));
    refresh();
    while ((c = getch()) != KEY_F(1)) {
        int flag = 0;
        int flag_stair = 0;
        int flag_g = 0;
        //instruction
        if (c == 's') {
            if ((user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] > 0) {
                if (user -> current_y - 1 >= 0 && ((user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] = '?';
                }
                if (user -> current_y - 1 >= 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] = '^';
                }

                if (user -> current_y + 1 < 60 && ((user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] = '?';
                }
                if (user -> current_y + 1 < 60 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] = '^';
                }

                if (user -> current_x - 1 >= 0 && ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '?';
                }
                if (user -> current_x - 1 >= 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '^';
                }

                if (user -> current_x + 1 < 200 && ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] = '?';
                }
                if (user -> current_x + 1 < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] = '^';
                }

                if (user -> current_y - 1 >= 0 && user -> current_x - 1 >= 0 && ((user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] = '?';
                }
                if (user -> current_y - 1 >= 0 && user -> current_x - 1 >= 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] = '^';
                }

                if (user -> current_y - 1 >= 0 && user -> current_x + 1 < 200 && ((user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] = '?';
                }
                if (user -> current_y - 1 >= 0 && user -> current_x + 1 < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] = '^';
                }

                if (user -> current_y + 1 < 60 && user -> current_x - 1 >= 0 && ((user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] = '?';
                }
                if (user -> current_y + 1 < 60 && user -> current_x - 1 >= 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] = '^';
                }

                if (user -> current_y + 1 < 60 && user -> current_x + 1 < 200 && ((user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] = '?';
                }
                if (user -> current_y + 1 < 60 && user -> current_x + 1 < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] = '^';
                }
            }
            c = getch();
        }
        if (c == 'g') {
            flag_g = 1;
            c = getch();
        }
        if (c == 'm') {
            user -> visible_mode = 1 - (user -> visible_mode);
            flag = 1;
        }
        else if (c == 'e') {
            food_menu_func(user);
            flag = 1;
        }
        else if (c == 'a') {
            enchant_menu_func(user);
            flag = 1;
        }
        else if (c == 'i') {
            weapon_menu_func(user);
            flag = 1;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '>' && c == '>') {
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
        else if (c == 'f') {
            c = getch();
            if (c == 'j') {
                //up
                while(move_directly(user, 0, -1, 1)) {
                    print_screen(user, flag_stair, gamer);
                }
                flag = 1;
            }
            else if (c == 'k') {
                //down
                while(move_directly(user, 0, 1, 1)) {
                    print_screen(user, flag_stair, gamer);
                }
                flag = 1;
            }
            else if (c == 'h') {
                //left
                while(move_directly(user, -1, 0, 1)) {
                    print_screen(user, flag_stair, gamer);
                }
                flag = 1;
            }
            else if (c == 'l') {
                //right
                while(move_directly(user, 1, 0, 1)) {
                    print_screen(user, flag_stair, gamer);
                }
                flag = 1;
            }
            else if (c == 'y') {
                //up left
                while(move_indirectly(user, -1, -1, 1)) {
                    print_screen(user, flag_stair, gamer);
                }
                flag = 1;
            }
            else if (c == 'u') {
                //up right
                while(move_indirectly(user, 1, -1, 1)) {
                    print_screen(user, flag_stair, gamer);
                }
                flag = 1;
            }
            else if (c == 'b') {
                //down left
                while(move_indirectly(user, -1, 1, 1)) {
                    print_screen(user, flag_stair, gamer);
                }
                flag = 1;
            }
            else if (c == 'n') {
                //down right
                while(move_indirectly(user, 1, 1, 1)) {
                    print_screen(user, flag_stair, gamer);
                }
                flag = 1;
            }
        }
        else if (c == 'j') {
            //up
            flag = move_directly(user, 0, -1, 0);
        }
        else if (c == 'k') {
            //down
            flag = move_directly(user, 0, 1, 0);
        }
        else if (c == 'h') {
            //left
            flag = move_directly(user, -1, 0, 0);
        }
        else if (c == 'l') {
            //right
            flag = move_directly(user, 1, 0, 0);
        }
        else if (c == 'y') {
            //up left
            flag = move_indirectly(user, -1, -1, 0);
        }
        else if (c == 'u') {
            //up right
            flag = move_indirectly(user, 1, -1, 0);
        }
        else if (c == 'b') {
            //down left
            flag = move_indirectly(user, -1, 1, 0);
        }
        else if (c == 'n') {
            //down right
            flag = move_indirectly(user, 1, 1, 0);
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
                    if (flag1 || user -> visible_mode) {
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
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && user -> current_y == i && user -> current_x == j + 1) {
                            mvaddch(START + i, START + j, '.');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && (user -> current_y != i || user -> current_x != j)) {
                            mvprintw(START + i, START + j, "%s", "\U0001F354");
                        }
                        else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && user -> current_y == i && user -> current_x == j + 1) {
                            mvaddch(START + i, START + j, '.');
                        }
                        else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && (user -> current_y != i || user -> current_x != j)) {
                            if ((user -> map_screen_char)[user -> current_floor][i][j] == '0') {
                                mvprintw(START + i, START + j, "%s", "\U0001F528");
                            }
                            else if ((user -> map_screen_char)[user -> current_floor][i][j] == '1') {
                                mvprintw(START + i, START + j, "%s", "\U0001F5E1");
                            }
                            else if ((user -> map_screen_char)[user -> current_floor][i][j] == '2') {
                                attron(COLOR_PAIR(4));
                                mvprintw(START + i, START + j, "%s", "\U0001327B");
                                attroff(COLOR_PAIR(4));
                            }
                            else if ((user -> map_screen_char)[user -> current_floor][i][j] == '3') {
                                mvprintw(START + i, START + j, "%s", "\U0001F3F9");
                            }
                            else if ((user -> map_screen_char)[user -> current_floor][i][j] == '4') {
                                mvprintw(START + i, START + j, "%s", "\U0001F52A");
                            }
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == '$') {
                            attron(COLOR_PAIR(4));
                            mvprintw(START + i, START + j, "%s", "\U00002735");
                            attroff(COLOR_PAIR(4));
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'G') {
                            attron(COLOR_PAIR(2));
                            mvprintw(START + i, START + j, "%s", "\U00002742");
                            attron(COLOR_PAIR(2));
                        }
                        else if (j == 0 || ((user -> map_screen_char)[user -> current_floor][i][j - 1] != 'B' && isdigit((user -> map_screen_char)[user -> current_floor][i][j - 1]) == 0)){
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
                if (user -> health < 20) {
                    attron(COLOR_PAIR(8));
                }
                else {
                    attron(COLOR_PAIR(7));
                }
                mvprintw(2, i + 7, " ");
                if (user -> health < 20) {
                    attroff(COLOR_PAIR(8));
                }
                else {
                    attroff(COLOR_PAIR(7));
                }
            }
            attron(COLOR_PAIR(4));
            mvprintw(3, 0, "gold %d", user -> gold);
            attroff(COLOR_PAIR(4));
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
            end = 1;
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
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'B') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
                attron(COLOR_PAIR(2) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(2) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + 1 + START, '.');
                (user -> food_menu)[0]++;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 'B') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '.';
                attron(COLOR_PAIR(2) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(2) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x - 1 + START, '.');
                (user -> food_menu)[0]++;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && isdigit((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x])) {
            attron(COLOR_PAIR(2) | A_BLINK);
            mvaddch(user -> current_y + START, user -> current_x + START, gamer);
            attroff(COLOR_PAIR(2) | A_BLINK);
            mvprintw(0, 0, "WEAPON COLLECTED!");
            mvaddch(user -> current_y + START, user -> current_x + 1 + START, '.');
            (user -> weapon_menu)[(user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] - '0']++;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            //mvprintw(0, 0, "%c", (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x]);
            refresh();
        }
        else if (flag_g == 0 && user -> current_x > 0 && isdigit((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1])) {
            attron(COLOR_PAIR(2) | A_BLINK);
            mvaddch(user -> current_y + START, user -> current_x + START, gamer);
            attroff(COLOR_PAIR(2) | A_BLINK);
            mvprintw(0, 0, "WEAPON COLLECTED!");
            mvaddch(user -> current_y + START, user -> current_x - 1 + START, '.');
            (user -> weapon_menu)[(user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] - '0']++;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '.';
            //mvprintw(0, 0, "%c", (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1]);
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '$') {
            (user -> gold) += 5;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            mvprintw(0, 0, "5 Golds Collected");
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'G') {
            (user -> gold) += 20;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            mvprintw(0, 0, "20 Golds Collected");
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'H') {
            (user -> enchant_menu)[0]++;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "HEALTH ENCHANT COLLECTED!");
            attroff(COLOR_PAIR(3));
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'S') {
            (user -> enchant_menu)[1]++;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "SPEED ENCHANT COLLECTED!");
            attroff(COLOR_PAIR(3));
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'D') {
            (user -> enchant_menu)[2]++;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "HEALTH ENCHANT COLLECTED!");
            attroff(COLOR_PAIR(3));
            refresh();
        }
        for (int i = 0; i < (user -> rooms_num)[user -> current_floor]; i++) {
            if ((user -> map_rooms)[user -> current_floor][i] -> theme == 6) {
                /*
                for (int y = ((user -> map_rooms)[user -> current_floor][i] -> uly) + 1; y < ((user -> map_rooms)[user -> current_floor][i] -> uly) + ((user -> map_rooms)[user -> current_floor][i] -> height) - 1; y++) {
                    for (int x = ((user -> map_rooms)[user -> current_floor][i] -> ulx) + 1; x < ((user -> map_rooms)[user -> current_floor][i] -> ulx) + ((user -> map_rooms)[user -> current_floor][i] -> width) - 1; x++) {
                        if ((user -> map_screen_char)[user -> current_floor][y][x] == 'B') {
                            (user -> map_screen_char)[user -> current_floor][y][x] = '.';
                        }
                    }
                }
                */
                int food = rand() % 100;
                if (food == 0) {
                    int food_x = ((user -> map_rooms)[user -> current_floor][i] -> ulx) + 1 + rand() % ((user -> map_rooms)[user -> current_floor][i] -> width - 2);
                    int food_y = ((user -> map_rooms)[user -> current_floor][i] -> uly) + 1 + rand() % ((user -> map_rooms)[user -> current_floor][i] -> height - 2);
                    int steps = 100;
                    while (steps > 0 && ((user -> map_screen_char)[user -> current_floor][food_y][food_x] != '.' || (user -> map_screen_char)[user -> current_floor][food_y][food_x+1] != '.' || (user -> map_screen_char)[user -> current_floor][food_y][food_x-1] != '.')) {
                        food_x = ((user -> map_rooms)[user -> current_floor][i] -> ulx) + 1 + rand() % ((user -> map_rooms)[user -> current_floor][i] -> width - 2);
                        food_y = ((user -> map_rooms)[user -> current_floor][i] -> uly) + 1 + rand() % ((user -> map_rooms)[user -> current_floor][i] -> height - 2);
                        steps --;
                    }
                    if (steps > 0) {
                        (user -> map_screen_char)[user -> current_floor][food_y][food_x] = 'B';
                    }
                }
            }
        }
    }
    pthread_join(thread_hunger, NULL);
    pthread_join(thread_health, NULL);
}

int move_directly(User *user, int x, int y, int type) {
    if (type == 0 && user -> current_y + y >= 0 && user -> current_y + y < 60 && user -> current_x + x >= 0 && user -> current_x + x < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != ' ' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '|' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '_' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'O') {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    if (type == 1 && user -> current_y + y >= 0 && user -> current_y + y < 60 && user -> current_x + x >= 0 && user -> current_x + x < 200 && ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == '.' || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == '#')) {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    return 0;
}

int move_indirectly(User *user, int x, int y, int type) {
    if (type == 0 && user -> current_y + y >= 0 && user -> current_y + y < 60 && user -> current_x + x >= 0 && user -> current_x + x < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != ' ' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '|' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '_' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'O' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '#' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '-' && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '!') {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    if (type == 1 && user -> current_y + y >= 0 && user -> current_y + y < 60 && user -> current_x + x >= 0 && user -> current_x + x < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == '.') {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    return 0;
}

void print_screen(User *user, int flag_stair, char gamer) {
    if ((user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] > 0) {
        Room *current_room = (user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] - 1];
        for (int i = current_room -> uly; i < current_room -> uly + current_room -> height; i++) {
            for (int j = current_room -> ulx; j < current_room -> ulx + current_room -> width; j++) {
                (user -> visible)[user -> current_floor][i][j] = 1;
            }
        }
    }
    (user -> visible)[user -> current_floor][user -> current_y][user -> current_x] = 1;
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
            if (flag1 || user -> visible_mode) {
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
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F354");
                }
                else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && (user -> current_y != i || user -> current_x != j)) {
                    if ((user -> map_screen_char)[user -> current_floor][i][j] == '0') {
                        mvprintw(START + i, START + j, "%s", "\U0001F528");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '1') {
                        mvprintw(START + i, START + j, "%s", "\U0001F5E1");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '2') {
                        attron(COLOR_PAIR(4));
                        mvprintw(START + i, START + j, "%s", "\U0001327B");
                        attroff(COLOR_PAIR(4));
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '3') {
                        mvprintw(START + i, START + j, "%s", "\U0001F3F9");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '4') {
                        mvprintw(START + i, START + j, "%s", "\U0001F52A");
                    }
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == '$') {
                    attron(COLOR_PAIR(4));
                    mvprintw(START + i, START + j, "%s", "\U00002735");
                    attroff(COLOR_PAIR(4));
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'G') {
                    attron(COLOR_PAIR(2));
                    mvprintw(START + i, START + j, "%s", "\U00002742");
                    attron(COLOR_PAIR(2));
                }
                else if (j == 0 || ((user -> map_screen_char)[user -> current_floor][i][j - 1] != 'B' && isdigit((user -> map_screen_char)[user -> current_floor][i][j - 1]) == 0)){
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
        if (user -> health < 20) {
            attron(COLOR_PAIR(8));
        }
        else {
            attron(COLOR_PAIR(7));
        }
        mvprintw(2, i + 7, " ");
        if (user -> health < 20) {
            attroff(COLOR_PAIR(8));
        }
        else {
            attroff(COLOR_PAIR(7));
        }
    }
    attron(COLOR_PAIR(4));
    mvprintw(3, 0, "gold %d", user -> gold);
    attroff(COLOR_PAIR(4));
    refresh();
    usleep(20000);
}

void *hunger_rate(void *arguments) {
    struct thread_args *args = (struct thread_args *) arguments;
    while (!(*(args -> end))) {
        sleep(60);
        (*(args -> hunger))--;
    }
    return NULL;
}

void *health_rate(void *arguments) {
    struct thread_args *args = (struct thread_args *) arguments;
    while (!(*(args -> end))) {
        sleep(20);
        if ((*(args -> hunger)) <= 5) {
            (*(args -> health))--;
        }
    }
    return NULL;
}

#endif
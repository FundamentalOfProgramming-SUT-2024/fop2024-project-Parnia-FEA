#ifndef CREATE_NEW_GAME_H
#define CREATE_NEW_GAME_H
#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include "user.h"
#include "room.h"

#define MAX_SIZE 50

void create_new_game_func (User *user) {
    clear();
    refresh();
    srand(time(0));
    int room_num[4];
    for (int i = 0; i < 4; i++) {
        room_num[i] = rand() % 3 + 6;
    }
    Room* rooms[4][10];
    int screen[4][60][200];
    char screen_char[4][60][200];
    for (int f = 0; f < 4; f++) {
        for (int i = 0; i < 60; i++) {
            for (int j = 0; j < 200; j++) {
                screen[f][i][j] = 0;
                screen_char[f][i][j] = ' ';
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < room_num[i]; j++) {
            rooms[i][j] = (Room *) malloc(sizeof(Room));
            rooms[i][j] -> height = rand() % 3 + 4;
            rooms[i][j] -> width = rand() % 3 + 4;
            rooms[i][j] -> floor = i + 1;
            rooms[i][j] -> room_number = j + 1;
            int x, y;
            while (TRUE) {
                y = rand() % (61 - rooms[i][j] -> height);
                x = rand() % (201 - rooms[i][j] -> width);
                int flag = 1;
                for (int k = y; k < y + rooms[i][j] -> height; k++) {
                    for (int l = x; l < x + rooms[i][j] -> width; l++) {
                        if (screen[i][k][l] != 0) {
                            flag = 0;
                            break;
                        }
                    }
                    if (flag == 0) {
                        break;
                    }
                }
                if (flag)
                    break;
            }
            rooms[i][j] -> ulx = x;
            rooms[i][j] -> uly = y;
            for (int k = y; k < y + rooms[i][j] -> height; k++) {
                for (int l = x; l < x + rooms[i][j] -> width; l++) {
                    screen[i][k][l] = j + 1;
                    if (k == y) {
                        screen_char[i][k][l] = '_';
                    }
                    else if (k == y + rooms[i][j] -> height - 1) {
                        screen_char[i][k][l] = '_';
                    }
                    else if (l == x) {
                        screen_char[i][k][l] = '|';
                    }
                    else if (l == x + rooms[i][j] -> width - 1) {
                        screen_char[i][k][l] = '|';
                    }
                    else {
                        screen_char[i][k][l] = '.';
                    }
                }
            }
            int door_num = rand() % 2;
            rooms[i][j] -> num_of_doors = door_num + 1;
            int door1_coor = rand() % 4;
            Door *door1 = (Door *)malloc(sizeof(Door));
            if (door1_coor == 0) {
                x = rand() % (rooms[i][j] -> width - 2);
                door1 -> x_coor = rooms[i][j] -> ulx + 1 + x;
                door1 -> y_coor = rooms[i][j] -> uly;
            }
            else if (door1_coor == 1) {
                x = rand() % (rooms[i][j] -> height - 2);
                door1 -> x_coor = rooms[i][j] -> ulx + rooms[i][j] -> width - 1;
                door1 -> y_coor = rooms[i][j] -> uly + 1 + x;
            }
            else if (door1_coor == 2) {
                x = rand() % (rooms[i][j] -> width - 2);
                door1 -> x_coor = rooms[i][j] -> ulx + 1 + x;
                door1 -> y_coor = rooms[i][j] -> uly + rooms[i][j] -> height - 1;
            }
            else {
                x = rand() % (rooms[i][j] -> height - 2);
                door1 -> x_coor = rooms[i][j] -> ulx;
                door1 -> y_coor = rooms[i][j] -> uly + 1 + x;
            }
            (rooms[i][j] -> doors)[0] = door1;
            screen_char[i][door1 -> y_coor][door1 -> x_coor] = '+';
            if (door_num == 1) {
                int door2_coor = rand() % 4;
                Door *door2 = (Door *)malloc(sizeof(Door));
                if (door2_coor == 0) {
                    x = rand() % (rooms[i][j] -> width - 2);
                    door2 -> x_coor = rooms[i][j] -> ulx + 1 + x;
                    door2 -> y_coor = rooms[i][j] -> uly;
                }
                else if (door2_coor == 1) {
                    x = rand() % (rooms[i][j] -> height - 2);
                    door2 -> x_coor = rooms[i][j] -> ulx + rooms[i][j] -> width - 1;
                    door2 -> y_coor = rooms[i][j] -> uly + 1 + x;
                }
                else if (door2_coor == 2) {
                    x = rand() % (rooms[i][j] -> width - 2);
                    door2 -> x_coor = rooms[i][j] -> ulx + 1 + x;
                    door2 -> y_coor = rooms[i][j] -> uly + rooms[i][j] -> height - 1;
                }
                else {
                    x = rand() % (rooms[i][j] -> height - 2);
                    door2 -> x_coor = rooms[i][j] -> ulx;
                    door2 -> y_coor = rooms[i][j] -> uly + 1 + x;
                }
                if (door2 -> x_coor == door1 -> x_coor && door2 -> y_coor == door1 -> y_coor) {
                    rooms[i][j] -> num_of_doors --;
                }
                else {
                    (rooms[i][j] -> doors)[1] = door2;
                    screen_char[i][door2 -> y_coor][door2 -> x_coor] = '+';
                }
            }
        }
    }
    for (int f = 0; f < 4; f++) {
        for (int i = 0; i < 60; i++) {
            for (int j = 0; j < 200; j++) {
                printw("%c", screen_char[f][i][j]);
            }
            printw("\n");
        }
        refresh();
        sleep(10);
        clear();
    }
}

#endif
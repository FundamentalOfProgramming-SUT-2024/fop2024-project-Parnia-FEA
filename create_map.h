#ifndef CREATE_MAP_H
#define CREATE_MAP_H
#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include "user.h"
#include "room.h"

#define MAX_SIZE 50

void build_corridor_up(int col, int start1_y, int start2_y, int floor, Room* rooms[4][10], int screen[4][60][200], char screen_char[4][60][200]);
void build_corridor_left(int row, int start1_x, int start2_x, int floor, Room* rooms[4][10], int screen[4][60][200], char screen_char[4][60][200]);
void connected_door(Room *room, char screen_char[4][60][200], int floor, int x, int y, int mark[60][200]);
void create_map (User *user) {
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
            rooms[i][j] -> height = rand() % 3 + 6;
            rooms[i][j] -> width = rand() % 3 + 6;
            rooms[i][j] -> floor = i + 1;
            rooms[i][j] -> room_number = j + 1;
            rooms[i][j] -> num_of_doors = 0;
            rooms[i][j] -> num_of_connected = 0;
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
            rooms[i][j] -> theme = 6;
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
        }
    }
    
    for (int floor = 0; floor < 4; floor++) {
        for (int i = 1; i < room_num[floor]; i++) {
            Room *room1 = rooms[floor][i-1];
            Room *room2 = rooms[floor][i];
            int room1_center_x = rand() % (room1 -> width - 2) + room1 -> ulx + 1;
            int room1_center_y = rand() % (room1 -> height - 2) + room1 -> uly + 1;
            int room2_center_x = rand() % (room2 -> width - 2) + room2 -> ulx + 1;
            int room2_center_y = rand() % (room2 -> height - 2) + room2 -> uly + 1;
            if (room1_center_y < room2_center_y || (room1_center_y == room2_center_y && room1_center_x < room2_center_x)) {
                room1 = rooms[floor][i];
                room2 = rooms[floor][i-1];
                room1_center_x = rand() % (room1 -> width - 2) + room1 -> ulx + 1; 
                room1_center_y = rand() % (room1 -> height - 2) + room1 -> uly + 1;
                room2_center_x = rand() % (room2 -> width - 2) + room2 -> ulx + 1;
                room2_center_y = rand() % (room2 -> height - 2) + room2 -> uly + 1;
            }
            if (room1_center_y != room2_center_y) {
                if (room1_center_x > room2_center_x) {
                    if (room2_center_x >= room1 -> ulx) {
                        if (screen_char[floor][room1 -> uly][room2_center_x] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room2_center_x;
                            door -> y_coor = room1 -> uly;
                            (room1 -> doors)[room1 -> num_of_doors] = door;
                            (room1 -> num_of_doors)++;
                            screen_char[floor][room1 -> uly][room2_center_x] = '+';
                        }
                        if (screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room2_center_x;
                            door -> y_coor = room2 -> uly + room2 -> height - 1;
                            (room2 -> doors)[room2 -> num_of_doors] = door;
                            (room2 -> num_of_doors)++;
                            screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] = '+';
                        }
                        build_corridor_up(room2_center_x, room1 -> uly - 1, room2 -> uly + room2 -> height, floor, rooms, screen, screen_char);
                    }
                    else if (room1_center_y <= room2 -> uly + room2 -> height - 1) {
                        if (screen_char[floor][room1_center_y][room1 -> ulx] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room1 -> ulx;
                            door -> y_coor = room1_center_y;
                            (room1 -> doors)[room1 -> num_of_doors] = door;
                            (room1 -> num_of_doors)++;
                            screen_char[floor][room1_center_y][room1 -> ulx] = '+';
                        }
                        if (screen_char[floor][room1_center_y][room2 -> ulx + room2 -> width - 1] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room2 -> ulx + room2 -> width - 1;
                            door -> y_coor = room1_center_y;
                            (room2 -> doors)[room2 -> num_of_doors] = door;
                            (room2 -> num_of_doors)++;
                            screen_char[floor][room1_center_y][room2 -> ulx + room2 -> width - 1] = '+';
                        }
                        build_corridor_left(room1_center_y, room1 -> ulx - 1, room2 -> ulx + room2 -> width, floor, rooms, screen, screen_char);
                    }
                    else {
                        if (screen_char[floor][room1_center_y][room1 -> ulx] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room1 -> ulx;
                            door -> y_coor = room1_center_y;
                            (room1 -> doors)[room1 -> num_of_doors] = door;
                            (room1 -> num_of_doors)++;
                            screen_char[floor][room1_center_y][room1 -> ulx] = '+';
                        }
                        if (screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room2_center_x;
                            door -> y_coor = room2 -> uly + room2 -> height - 1;
                            (room2 -> doors)[room2 -> num_of_doors] = door;
                            (room2 -> num_of_doors)++;
                            screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] = '+';
                        }
                        if (screen[floor][room1_center_y][room2_center_x] == 0) {
                            build_corridor_left(room1_center_y, room1 -> ulx - 1, room2_center_x, floor, rooms, screen, screen_char);
                            build_corridor_up(room2_center_x, room1_center_y - 1, room2 -> uly + room2 -> height, floor, rooms, screen, screen_char);
                        }
                        else {
                            int room_number = screen[floor][room1_center_y][room2_center_x] - 1;
                            Room *room3 = rooms[floor][room_number];
                            if (screen_char[floor][room1_center_y][room3 -> ulx + room3 -> width - 1] != '+') {
                                Door *door = (Door *) malloc(sizeof(Door));
                                door -> x_coor = room3 -> ulx + room3 -> width - 1;
                                door -> y_coor = room1_center_y;
                                (rooms[floor][room_number] -> doors)[rooms[floor][room_number] -> num_of_doors] = door;
                                (rooms[floor][room_number] -> num_of_doors)++;
                                screen_char[floor][room1_center_y][room3 -> ulx + room3 -> width - 1] = '+';
                            }
                            if (screen_char[floor][room3 -> uly][room2_center_x] != '+') {
                                Door *door = (Door *) malloc(sizeof(Door));
                                door -> x_coor = room2_center_x;
                                door -> y_coor = room3 -> uly;
                                (rooms[floor][room_number] -> doors)[rooms[floor][room_number] -> num_of_doors] = door;
                                (rooms[floor][room_number] -> num_of_doors)++;
                                screen_char[floor][room3 -> uly][room2_center_x] = '+';
                            }
                            build_corridor_left(room1_center_y, room1 -> ulx - 1, room3 -> ulx + room3 -> width, floor, rooms, screen, screen_char);
                            build_corridor_up(room2_center_x, room3 -> uly - 1, room2 -> uly + room2 -> height, floor, rooms, screen, screen_char);
                        }
                    }
                }
                else if (room1_center_x < room2_center_x) {
                    if (room2_center_x <= room1 -> ulx + room1 -> width - 1) {
                        if (screen_char[floor][room1 -> uly][room2_center_x] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room2_center_x;
                            door -> y_coor = room1 -> uly;
                            (room1 -> doors)[room1 -> num_of_doors] = door;
                            (room1 -> num_of_doors)++;
                            screen_char[floor][room1 -> uly][room2_center_x] = '+';
                        }
                        if (screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room2_center_x;
                            door -> y_coor = room2 -> uly + room2 -> height - 1;
                            (room2 -> doors)[room2 -> num_of_doors] = door;
                            (room2 -> num_of_doors)++;
                            screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] = '+';
                        }
                        build_corridor_up(room2_center_x, room1 -> uly - 1, room2 -> uly + room2 -> height, floor, rooms, screen, screen_char);
                    }
                    else if (room1_center_y <= room2 -> uly + room2 -> height - 1) {
                        if (screen_char[floor][room1_center_y][room1 -> ulx + room1 -> width - 1] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room1 -> ulx + room1 -> width - 1;
                            door -> y_coor = room1_center_y;
                            (room1 -> doors)[room1 -> num_of_doors] = door;
                            (room1 -> num_of_doors)++;
                            screen_char[floor][room1_center_y][room1 -> ulx + room1 -> width - 1] = '+';
                        }
                        if (screen_char[floor][room1_center_y][room2 -> ulx] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room2 -> ulx;
                            door -> y_coor = room1_center_y;
                            (room2 -> doors)[room2 -> num_of_doors] = door;
                            (room2 -> num_of_doors)++;
                            screen_char[floor][room1_center_y][room2 -> ulx] = '+';
                        }
                        build_corridor_left(room1_center_y, room2 -> ulx - 1, room1 -> ulx + room1 -> width, floor, rooms, screen, screen_char);
                    }
                    else {
                        if (screen_char[floor][room1_center_y][room1 -> ulx + room1 -> width - 1] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room1 -> ulx + room1 -> width - 1;
                            door -> y_coor = room1_center_y;
                            (room1 -> doors)[room1 -> num_of_doors] = door;
                            (room1 -> num_of_doors)++;
                            screen_char[floor][room1_center_y][room1 -> ulx + room1 -> width - 1] = '+';
                        }
                        if (screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] != '+') {
                            Door *door = (Door *) malloc(sizeof(Door));
                            door -> x_coor = room2_center_x;
                            door -> y_coor = room2 -> uly + room2 -> height - 1;
                            (room2 -> doors)[room2 -> num_of_doors] = door;
                            (room2 -> num_of_doors)++;
                            screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] = '+';
                        }
                        if (screen[floor][room1_center_y][room2_center_x] == 0) {
                            build_corridor_left(room1_center_y, room2_center_x, room1 -> ulx + room1 -> width, floor, rooms, screen, screen_char);
                            build_corridor_up(room2_center_x, room1_center_y - 1, room2 -> uly + room2 -> height, floor, rooms, screen, screen_char);
                        }
                        else {
                            int room_number = screen[floor][room1_center_y][room2_center_x] - 1;
                            Room *room3 = rooms[floor][room_number];
                            if (screen_char[floor][room1_center_y][room3 -> ulx] != '+') {
                                Door *door = (Door *) malloc(sizeof(Door));
                                door -> x_coor = room3 -> ulx;
                                door -> y_coor = room1_center_y;
                                (room3 -> doors)[room3 -> num_of_doors] = door;
                                (room3 -> num_of_doors)++;
                                screen_char[floor][room1_center_y][room3 -> ulx] = '+';
                            }
                            if (screen_char[floor][room3 -> uly][room2_center_x] != '+') {
                                Door *door = (Door *) malloc(sizeof(Door));
                                door -> x_coor = room2_center_x;
                                door -> y_coor = room3 -> uly;
                                (room3 -> doors)[room3 -> num_of_doors] = door;
                                (room3 -> num_of_doors)++;
                                screen_char[floor][room3 -> uly][room2_center_x] = '+';
                            }
                            build_corridor_left(room1_center_y, room3 -> ulx - 1, room1 -> ulx + room1 -> width, floor, rooms, screen, screen_char);
                            build_corridor_up(room2_center_x, room3 -> uly - 1, room2 -> uly + room2 -> height, floor, rooms, screen, screen_char);
                        }
                    }
                }
                else {
                    if (screen_char[floor][room1 -> uly][room2_center_x] != '+') {
                        Door *door = (Door *) malloc(sizeof(Door));
                        door -> x_coor = room2_center_x;
                        door -> y_coor = room1 -> uly;
                        (room1 -> doors)[room1 -> num_of_doors] = door;
                        (room1 -> num_of_doors)++;
                        screen_char[floor][room1 -> uly][room2_center_x] = '+';
                    }
                    if (screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] != '+') {
                        Door *door = (Door *) malloc(sizeof(Door));
                        door -> x_coor = room2_center_x;
                        door -> y_coor = room2 -> uly + room2 -> height - 1;
                        (room2 -> doors)[room2 -> num_of_doors] = door;
                        (room2 -> num_of_doors)++;
                        screen_char[floor][room2 -> uly + room2 -> height - 1][room2_center_x] = '+';
                    }
                    build_corridor_up(room2_center_x, room1 -> uly - 1, room2 -> uly + room2 -> height, floor, rooms, screen, screen_char);
                }
            }
            else {
                if (screen_char[floor][room1_center_y][room1 -> uly] != '+') {
                    Door *door = (Door *) malloc(sizeof(Door));
                    door -> x_coor = room1 -> uly;
                    door -> y_coor = room1_center_y;
                    (room1 -> doors)[room1 -> num_of_doors] = door;
                    (room1 -> num_of_doors)++;
                    screen_char[floor][room1_center_y][room1 -> uly] = '+';
                }
                if (screen_char[floor][room2_center_y][room2 -> ulx + room2 -> width - 1] != '+') {
                    Door *door = (Door *) malloc(sizeof(Door));
                    door -> x_coor = room2 -> ulx + room2 -> width - 1;
                    door -> y_coor = room2_center_y;
                    (room2 -> doors)[room2 -> num_of_doors] = door;
                    (room2 -> num_of_doors)++;
                    screen_char[floor][room2_center_y][room2 -> ulx + room2 -> width - 1] = '+';
                }
                build_corridor_left(room1_center_y, room1 -> ulx - 1, room2 -> ulx + room2 -> width, floor, rooms, screen, screen_char);
            }
        }
    }

    
    /*
    for (int f = 0; f < 4; f++) {
        for (int i = 0; i < 60; i++) {
            for (int j = 0; j < 200; j++) {
                if (screen_char[f][i][j] == '.') {
                    printw("%d", screen[f][i][j] - 1);
                }
                else {
                    printw("%c", screen_char[f][i][j]);
                }
            }
            printw("\n");
        }
        refresh();
        sleep(10);
        clear();
    }
    */
    for (int f = 0; f < 4; f++) {
        for (int i = 0; i < room_num[f]; i++) {
            for (int j = 0; j < (rooms[f][i] -> num_of_doors); j++) {
                int mark[60][200];
                for (int k = 0; k < 60; k++) {
                    for (int l = 0; l < 200 ;l++) {
                        mark[k][l] = 0;
                    }
                }
                int k = (rooms[f][i] -> doors)[j] -> y_coor;
                int l = (rooms[f][i] -> doors)[j] -> x_coor;
                mark[k][l] = 1;
                if (k > 0) { 
                    connected_door(rooms[f][i], screen_char, f, l, k-1, mark);
                }
                if (k < 59) { 
                    connected_door(rooms[f][i], screen_char, f, l, k+1, mark);
                }
                if (l < 199) { 
                    connected_door(rooms[f][i], screen_char, f, l+1, k, mark);
                }
                if (l > 0) { 
                    connected_door(rooms[f][i], screen_char, f, l-1, k, mark);
                }
            }
        }
    }



    for (int i = 0; i < 4; i++) {
        (user -> rooms_num)[i] = room_num[i];
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < room_num[i]; j++) {
            (user -> map_rooms)[i][j] = rooms[i][j];
        }
    }
    for (int f = 0; f < 4; f++) {
        for (int i = 0; i < 60; i++) {
            for (int j = 0; j < 200; j++) {
                (user -> map_screen)[f][i][j] = screen[f][i][j];
                (user -> map_screen_char)[f][i][j] = screen_char[f][i][j];
                (user -> visible)[f][i][j] = 0;
                if (screen_char[f][i][j] == '#') {
                    (user -> map_screen)[f][i][j] = -1;
                }
            }
        }
    }
    Room *start_room = rooms[0][0];
    for (int i = start_room -> uly; i < start_room -> uly + start_room -> height; i++) {
        for (int j = start_room -> ulx; j < start_room -> ulx + start_room -> width; j++) {
            (user -> visible)[0][i][j] = 1;
        }
    }
    int treasure_num = room_num[3] - 1 - rand() % 3;
    rooms[3][treasure_num] -> theme = 4;
    user -> end_y = rooms[3][treasure_num] -> uly + rooms[3][treasure_num] -> height / 2;
    user -> end_x = rooms[3][treasure_num] -> ulx + rooms[3][treasure_num] -> width / 2;
    user -> current_floor = 0;
    user -> current_y = start_room -> uly + start_room -> height / 2;
    user -> current_x = start_room -> ulx + start_room -> width / 2;

    for (int f = 0; f < 4; f++) {
        int enchant_num = room_num[f] - 1 - rand() % 3;
        if (f == 3) {
            while(rooms[f][enchant_num] -> theme == 4) {
                enchant_num = room_num[f] - 1 - rand() % 3;
            }
        }
        (user -> map_rooms)[f][enchant_num] -> theme = 3;
    }

}

void build_corridor_up(int col, int start1_y, int start2_y, int floor, Room* rooms[4][10], int screen[4][60][200], char screen_char[4][60][200]) {
    if (start1_y < start2_y) {
        return;
    }
    if (screen[floor][start1_y][col] == 0) {
        screen_char[floor][start1_y][col] = '#';
        build_corridor_up(col, start1_y - 1, start2_y, floor, rooms, screen, screen_char);
        return;
    }
    int room_number = screen[floor][start1_y][col] - 1;
    if (screen_char[floor][start1_y][col] != '+') {
        Door *door = (Door *) malloc(sizeof(Door));
        door -> x_coor = col;
        door -> y_coor = start1_y;
        (rooms[floor][room_number] -> doors)[rooms[floor][room_number] -> num_of_doors] = door;
        (rooms[floor][room_number] -> num_of_doors)++;
        screen_char[floor][start1_y][col] = '+';
    }
    if (screen_char[floor][rooms[floor][room_number] -> uly][col] != '+') {
        Door *door = (Door *) malloc(sizeof(Door));
        door -> x_coor = col;
        door -> y_coor = rooms[floor][room_number] -> uly;
        (rooms[floor][room_number] -> doors)[rooms[floor][room_number] -> num_of_doors] = door;
        (rooms[floor][room_number] -> num_of_doors)++;
        screen_char[floor][rooms[floor][room_number] -> uly][col] = '+';
    }
    build_corridor_up(col, rooms[floor][room_number] -> uly - 1, start2_y, floor, rooms, screen, screen_char);
}

void build_corridor_left(int row, int start1_x, int start2_x, int floor, Room* rooms[4][10], int screen[4][60][200], char screen_char[4][60][200]) {
    if (start1_x < start2_x) {
        return;
    }
    if (screen[floor][row][start1_x] == 0) {
        screen_char[floor][row][start1_x] = '#';
        build_corridor_left(row, start1_x - 1, start2_x, floor, rooms, screen, screen_char);
        return;
    }
    int room_number = screen[floor][row][start1_x] - 1;
    if (screen_char[floor][row][start1_x] != '+') {
        Door *door = (Door *) malloc(sizeof(Door));
        door -> x_coor = start1_x;
        door -> y_coor = row;
        (rooms[floor][room_number] -> doors)[rooms[floor][room_number] -> num_of_doors] = door;
        (rooms[floor][room_number] -> num_of_doors)++;
        screen_char[floor][row][start1_x] = '+';
    }
    if (screen_char[floor][row][rooms[floor][room_number] -> ulx] != '+') {
        Door *door = (Door *) malloc(sizeof(Door));
        door -> x_coor = rooms[floor][room_number] -> ulx;
        door -> y_coor = row;
        (rooms[floor][room_number] -> doors)[rooms[floor][room_number] -> num_of_doors] = door;
        (rooms[floor][room_number] -> num_of_doors)++;
        screen_char[floor][row][rooms[floor][room_number] -> ulx] = '+';
    }
    build_corridor_left(row, rooms[floor][room_number] -> ulx - 1, start2_x, floor, rooms, screen, screen_char);
}


void connected_door(Room *room, char screen_char[4][60][200], int floor, int x, int y, int mark[60][200]) {
    if (mark[y][x])
        return;
    if (screen_char[floor][y][x] != '+' && screen_char[floor][y][x] != '#')
        return;
    mark[y][x] = 1;
    if (screen_char[floor][y][x] == '+') {
        for (int i = 0; i < room -> num_of_connected; i++) {
            if ((room -> connected)[i] -> x_coor == x && (room -> connected)[i] -> y_coor == y) {
                return;
            }
        }
        (room -> connected)[room -> num_of_connected] = (Door *) malloc(sizeof(Door));
        (room -> connected)[room -> num_of_connected] -> x_coor = x;
        (room -> connected)[room -> num_of_connected] -> y_coor = y;
        (room -> num_of_connected)++;
        return;
    }
    if (y > 0 && (screen_char[floor][y-1][x] == '#' || screen_char[floor][y-1][x] == '+')) {
        connected_door(room, screen_char, floor, x, y-1, mark);
    }
    if (y < 59 && (screen_char[floor][y+1][x] == '#' || screen_char[floor][y+1][x] == '+')) {
        connected_door(room, screen_char, floor, x, y+1, mark);
    }
    if (x > 0 && (screen_char[floor][y][x-1] == '#' || screen_char[floor][y][x-1] == '+')) {
        connected_door(room, screen_char, floor, x-1, y, mark);
    }
    if (x < 199 && (screen_char[floor][y][x+1] == '#' || screen_char[floor][y][x+1] == '+')) {
        connected_door(room, screen_char, floor, x+1, y, mark);
    }
}

#endif
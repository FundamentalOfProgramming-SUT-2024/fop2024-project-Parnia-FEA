#ifndef ROOM_H
#define ROOM_H

#define MAX_SIZE 50

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int x_coor;
    int y_coor;
    int model;
} Door;

typedef struct {
    int ulx;
    int uly;
    int height;
    int width;
    int floor;
    int room_number;
    int num_of_doors;
    Door* doors[1000];
} Room;

typedef struct {
    char total_screen[4][60][200];
} Screen;

#endif
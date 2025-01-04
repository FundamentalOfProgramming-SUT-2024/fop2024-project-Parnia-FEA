#ifndef ROOM_H
#define ROOM_H

#define MAX_SIZE 50

typedef struct {
    int x_coor;
    int y_coor;
} Door;

typedef struct {
    int ulx;
    int uly;
    int height;
    int width;
    int floor;
    int room_number;
    int num_of_doors;
    Door* doors[MAX_SIZE];
} Room;

#endif
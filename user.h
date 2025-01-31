#ifndef USER_H
#define USER_H

#include "room.h"

#define MAX_SIZE 50


typedef struct {
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char email[MAX_SIZE];
    int rooms_num[4];
    Room *map_rooms[4][10];
    int map_screen[4][60][200];
    char map_screen_char[4][60][200];
    Point *in_staircase[4];
    Point *out_staircase[4];
    int visible[4][60][200];
    int end_x;
    int end_y;
    int current_floor;
    int current_x;
    int current_y;
    int health;
    long long total_gold;
    long long gold;
    int food;
    int food_menu[5];
    int enchant_menu[3];
    int weapon_menu[5];
    int visible_mode;
    int hunger;
    int difficulty;
    int color;
    long long score;
    int games;
    int complete_games;
} User;

#endif
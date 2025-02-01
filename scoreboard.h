#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

#define MAX_SIZE 1000

typedef struct {
    char username[MAX_SIZE2];
    int score;
} User_Board;

void show_page(User *, User_Board **, int, int);
int comp(const void *, const void *);

void show_score_board (User *user) {
    FILE *users;
    users = fopen("users.txt", "r");
    char line[MAX_SIZE];
    int num_users = 0;
    while (fgets(line, MAX_SIZE, users)) {
        if (strcmp(line, "{\n") == 0) {
            num_users++;
        }
    }
    fclose(users);
    User_Board **users_board = (User_Board **) malloc(num_users * sizeof(User_Board *));
    users = fopen("users.txt", "r");
    int i = 0;
    while (fgets(line, MAX_SIZE, users)) {
        if (strcmp(line, "{\n") == 0) {
            users_board[i] = (User_Board *) malloc(sizeof(User_Board));
            fgets(line, MAX_SIZE, users);
            line[strlen(line) - 1] = '\0';
            strcpy(users_board[i] -> username, line);
            fgets(line, MAX_SIZE, users); //password
            fgets(line, MAX_SIZE, users); // email
            fgets(line, MAX_SIZE, users); //total gold ++
            fgets(line, MAX_SIZE, users); //score ++
            line[strlen(line)-1] = '\0';
            sscanf(line, "%d", &(users_board[i] -> score));
            i++;
        }
    }
    fclose(users);
    qsort(users_board, num_users, sizeof(users_board[0]), comp);
    int page = 1;
    int c;
    show_page(user, users_board, 1, num_users);
    while ((c = getch()) != KEY_F(1)) {
        switch(c) {
            case KEY_RIGHT :
                if (((double)num_users) / 50 > page) {
                    page++;
                }
                break;
            case KEY_LEFT :
                if (page > 1) {
                    page--;
                }
                break;
        }
        show_page(user, users_board, page, num_users);
    }
}

void show_page(User *user, User_Board **users_board, int page, int num) {
    clear();
    mvprintw(LINES - 3, 0, "Press F1 to exit");
	mvprintw(LINES - 2, 0, "Page %d", page);
    attron(COLOR_PAIR(2) | A_BLINK);
    mvprintw(0, 104, "Scoreboard");
    attroff(COLOR_PAIR(2) | A_BLINK);
    attron(COLOR_PAIR(3));
    mvprintw(1, 11, "Username");
    mvprintw(1, 42, "Score");
    mvprintw(1, 72, "Gold");
    mvprintw(1, 102, "Completed Games");
    mvprintw(1, 132, "Experience");
    attroff(COLOR_PAIR(3));
    int i = (page - 1) * 50;
    while (i < page * 50 && i < num) {
        FILE *users;
        users = fopen("users.txt", "r");
        char line[MAX_SIZE];
        while (fgets(line, MAX_SIZE, users)) {
            if (strcmp(line, "{\n") == 0) {
                fgets(line, MAX_SIZE, users);
                line[strlen(line)-1] = '\0';
                if (strcmp(users_board[i] -> username, line) == 0) {
                    mvprintw(i + 3 - ((page - 1) * 50), 8, "%d  %s", i+1, line);
                    fgets(line, MAX_SIZE, users); //password
                    fgets(line, MAX_SIZE, users); // email
                    char total_gold[MAX_SIZE];
                    char score[MAX_SIZE];
                    char experience[MAX_SIZE];
                    char complete_games[MAX_SIZE];
                    fgets(total_gold, MAX_SIZE, users); //total gold ++
                    total_gold[strlen(total_gold)-1] = '\0';
                    fgets(score, MAX_SIZE, users); //score ++
                    score[strlen(score)-1] = '\0';
                    fgets(line, MAX_SIZE, users); //difficulty
                    fgets(line, MAX_SIZE, users); //color
                    fgets(experience, MAX_SIZE, users); //games ++
                    experience[strlen(experience)-1] = '\0';
                    fgets(complete_games, MAX_SIZE, users); //complete games ++
                    complete_games[strlen(complete_games)-1] = '\0';
                    mvprintw(i + 3 - ((page - 1) * 50), 42, "%s", score);
                    mvprintw(i + 3 - ((page - 1) * 50), 72, "%s", total_gold);
                    mvprintw(i + 3 - ((page - 1) * 50), 102, "%s", complete_games);
                    mvprintw(i + 3 - ((page - 1) * 50), 132, "%s", experience);
                    i++;
                    break;
                }
            }
        }
        fclose(users);
    }
    refresh();
}

int comp(const void *a, const void *b) {
    return ((User_Board *) b) -> score - ((User_Board *) a) -> score;
}

#endif
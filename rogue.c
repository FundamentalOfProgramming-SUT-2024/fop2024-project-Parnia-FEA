#include <curses.h>
#include <form.h>
#include <menu.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "welcome.h"

#define MAX_SIZE 50
#define LENGTH 214
#define WIDTH 66

int main() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
	noecho();
    keypad(stdscr, TRUE);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_GREEN, COLOR_GREEN);
    init_pair(8, COLOR_RED, COLOR_RED);
    /*
    printw("%s","\U0001f375");
    refresh();
    sleep(10);
    */
    welcome_menu();
    endwin();
    return 0;
}
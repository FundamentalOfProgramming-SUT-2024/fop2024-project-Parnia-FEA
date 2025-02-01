#ifndef WEAPON_MENU_H
#define WEAPON_MENU_H
#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "create_new_game.h"
#include "scoreboard.h"
#include "setting.h"
#include "user.h"

#define MAX_SIZE 1000

void hammer(User *);
void dagger(User *);
void wand(User *);
void arrow(User *);
void hocho(User *);

void weapon_menu_func(User *user) {
	clear();
    int weapon = (user -> weapon_menu)[0] +(user -> weapon_menu)[1] + (user -> weapon_menu)[2] + (user -> weapon_menu)[3] + (user -> weapon_menu)[4];
    if (weapon  == 0) {
        attron(COLOR_PAIR(2) | A_BLINK);
        mvprintw(32, 100, "NO WEAPONS!!!");
        attroff(COLOR_PAIR(2) | A_BLINK);
        sleep(3);
        return;
    }
    ITEM **items;			
	MENU *my_menu;
	WINDOW *menu_win;
    int c;
    char choices[5][100];
    char *choices_numbers[5] = {"Hammer", "Dagger", "Wand", "Arrow", "Hocho"};
    void (*func[5]) (User *) = {hammer, dagger, wand, arrow, hocho};
    sprintf(choices[0], "%d", (user -> weapon_menu)[0]);
    sprintf(choices[1], "%d", (user -> weapon_menu)[1]);
    sprintf(choices[2], "%d", (user -> weapon_menu)[2]);
    sprintf(choices[3], "%d", (user -> weapon_menu)[3]);
    sprintf(choices[4], "%d", (user -> weapon_menu)[4]);
    items = (ITEM **)calloc(6, sizeof(ITEM *));
    for(int i = 0; i < 5; i++) {       
        items[i] = new_item(choices_numbers[i], choices[i]);
        set_item_userptr(items[i], func[i]);
	}
	items[5] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)items);
	menu_win = newwin(5, 40, 32, 100);
	keypad(menu_win, TRUE);
	set_menu_win(my_menu, menu_win);
	set_menu_sub(my_menu, derwin(menu_win, 5, 40, 0, 0));
	mvprintw(LINES - 2, 0, "Press <ENTER> to select the weapon");
	refresh();
	post_menu(my_menu);
	wrefresh(menu_win);
	while((c = wgetch(menu_win)) != KEY_F(1)) {       
        switch(c) {	
            case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10:
			{
                
                ITEM *cur_item;
				void (*p)(User *);
				cur_item = current_item(my_menu);
				p = (void (*) (User *)) item_userptr(cur_item);
				pos_menu_cursor(my_menu);
				unpost_menu(my_menu);
				for(int i = 0; i < 5; ++i)
					free_item(items[i]);
				free_menu(my_menu);
				wrefresh(menu_win);
				(*p)(user);
				return;
			}
		}
	}	
	unpost_menu(my_menu);
	for(int i = 0; i < 5; i++)
		free_item(items[i]);
	free_menu(my_menu);
}

void hammer(User *user) {

}

void dagger(User *user) {

}

void wand(User *user) {

}

void arrow(User *user) {

}

void hocho(User *user) {

}

#endif
#ifndef WELCOME_H
#define WELCOME_H
#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include "new_user.h"
#include "login.h"

#define MAX_SIZE 50

void welcome_menu();

void welcome_menu() {
    clear();
	ITEM **items;			
	MENU *my_menu;
	WINDOW *menu_win;
    int c, i;
    char *choices[] = {"Create Account", "Log In"};
    char *choices_numbers[] = {" ", " "};
    items = (ITEM **)calloc(3, sizeof(ITEM *));
    for(i = 0; i < 2; ++i) {       
        items[i] = new_item(choices_numbers[i], choices[i]);
	}
	items[2] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)items);
	menu_win = newwin(2, 40, 32, 100);
	keypad(menu_win, TRUE);
	set_menu_win(my_menu, menu_win);
	set_menu_sub(my_menu, derwin(menu_win, 2, 40, 0, 0));
	mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
	mvprintw(LINES - 2, 0, "Up and Down arrow keys to navigate (F1 to Exit)");
	attron(COLOR_PAIR(2) | A_BLINK);
	mvprintw(29, 104, "WELCOME!");
	attroff(COLOR_PAIR(2) | A_BLINK);
	refresh();
	post_menu(my_menu);
	wrefresh(menu_win);
	int flag = 0;
	while((c = wgetch(menu_win)) != KEY_F(1)) {       
        switch(c) {	
            case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				flag ++;
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				flag ++;
				break;
			case 10:
			{	
                if (flag % 2 == 0) {
                    create_account_menu(0, 0, 0, 0);
					pos_menu_cursor(my_menu);
                    unpost_menu(my_menu);
	                for(i = 0; i < 2; ++i)
		                free_item(items[i]);
	                free_menu(my_menu);
                    welcome_menu();
                    return;
                }
                else {
                    login_menu(0, 0);
					pos_menu_cursor(my_menu);
                    unpost_menu(my_menu);
	                for(i = 0; i < 2; ++i)
		                free_item(items[i]);
	                free_menu(my_menu);
                    welcome_menu();
                    return;
                }
				pos_menu_cursor(my_menu);
				break;
			}
			break;
		}
	}	
	unpost_menu(my_menu);
	for(i = 0; i < 2; ++i)
		free_item(items[i]);
	free_menu(my_menu);
}

#endif
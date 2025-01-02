#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include "new-user.h"
#include "login.h"

#define MAX_SIZE 50

void welcome_menu();

void welcome_menu() {
    ITEM **items;
    ITEM *cur_item;
	int c, i;				
	MENU *my_menu;
    char *choices[] = {"Create Account", "Log In"};
    items = (ITEM **)calloc(3, sizeof(ITEM *));
    for(i = 0; i < 2; ++i) {       
        items[i] = new_item(choices[i], choices[i]);
	}
	items[2] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)items);

	mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
	mvprintw(LINES - 2, 0, "Up and Down arrow keys to navigate (F1 to Exit)");
	post_menu(my_menu);
	refresh();

	while((c = getch()) != KEY_F(1)) {       
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
				cur_item = current_item(my_menu);
                char *cur_item_name;
                if (strcmp(choices[0], (cur_item->name).str) == 0) {
                    create_account_menu(0, 0, 0, 0);
                    unpost_menu(my_menu);
	                for(i = 0; i < 2; ++i)
		                free_item(items[i]);
	                free_menu(my_menu);
                    welcome_menu();
                    return;
                }
                else {
                    login_menu(0, 0);
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
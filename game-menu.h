#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "create-new-game.h"
#include "continue-game.h"
#include "score-board.h"
#include "setting.h"
#include "user.h"

#define MAX_SIZE 50

void game_menu(User *user) {
    ITEM **items;			
	MENU *my_menu;
    int c, i;
    char *choices[] = {"New Game", "Continue Game", "Score Board", "Settings"};
    char *choices_numbers[] = {" ", " ", " ", " "};
    void (*func[4]) (User *) = {create_new_game, continue_game, show_score_board, show_settings};
    items = (ITEM **)calloc(5, sizeof(ITEM *));
    for(i = 0; i < 4; ++i) {       
        items[i] = new_item(choices_numbers[i], choices[i]);
        set_item_userptr(items[i], func[i]);
	}
	items[4] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)items);

	mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
	mvprintw(LINES - 2, 0, "Up and Down arrow keys to navigate (F1 to Log Out)");
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
				void (*p)(User *);
				cur_item = current_item(my_menu);
				p = (void (*) (User *)) item_userptr(cur_item);
				(*p)(user);
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
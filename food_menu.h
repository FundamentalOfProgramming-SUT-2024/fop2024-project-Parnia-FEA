#ifndef FOOD_MENU_H
#define FOOD_MENU_H
#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "create_new_game.h"
#include "scoreboard.h"
#include "setting.h"
#include "user.h"

#define MAX_SIZE 1000

void regular_food(User *, int);
void premium_food(User *, int);
void magical_food(User *, int);
void rotten_food(User *, int);

void food_menu_func(User *user) {
	clear();
    if (user -> food == 0) {
        for (int i = 0; i < (user -> hunger); i++) {
            if (user -> hunger <= 5) {
                attron(COLOR_PAIR(8));
            }
            else {
                attron(COLOR_PAIR(7));
            }
            mvprintw(2, i + 7, " ");
            if (user -> hunger <= 5) {
                attroff(COLOR_PAIR(8));
            }
            else {
                attroff(COLOR_PAIR(7));
            }
        }
        attron(COLOR_PAIR(2) | A_BLINK);
        mvprintw(32, 100, "NO FOOD!");
        attroff(COLOR_PAIR(2) | A_BLINK);
        sleep(3);
        return;
    }
    ITEM **items;			
	MENU *my_menu;
	WINDOW *menu_win;
    int c;
    char **choices = (char **) malloc((user -> food) * sizeof(char *));
    char **choices_numbers = (char **) malloc((user -> food) * sizeof(char *));
    void (**func) (User *, int) = (void (**) (User *, int)) malloc((user -> food) * sizeof(void (*) (User *, int)));
    for (int i = 0; i < (user -> food); i++) {
        choices[i] = (char *) malloc(10 * sizeof(char));
        choices_numbers[i] = (char *) malloc(5 * sizeof(char));
        choices_numbers[i][0] = ' ';
        choices_numbers[i][1] = '\0';
        if ((user -> food_menu)[i] == 0) {
            strcpy(choices[i], "Regular");
            func[i] = &regular_food;
        }
        else if ((user -> food_menu)[i] == 1) {
            strcpy(choices[i], "Premium");
            func[i] = &premium_food;
        }
        else if ((user -> food_menu)[i] == 2) {
            strcpy(choices[i], "Magical");
            func[i] = &magical_food;
        }
        else {
            strcpy(choices[i], "Regular");
            func[i] = &rotten_food;
        }
    }
    items = (ITEM **)calloc((user -> food) + 1, sizeof(ITEM *));
    for(int i = 0; i < (user -> food); i++) {       
        items[i] = new_item(choices_numbers[i], choices[i]);
        set_item_userptr(items[i], func[i]);
	}
	items[(user -> food)] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)items);
	menu_win = newwin(user -> food, 40, 32, 100);
	keypad(menu_win, TRUE);
	set_menu_win(my_menu, menu_win);
	set_menu_sub(my_menu, derwin(menu_win, user -> food, 40, 0, 0));
    mvprintw(2, 0, "hunger");
    for (int i = 0; i < (user -> hunger); i++) {
        if (user -> hunger <= 5) {
            attron(COLOR_PAIR(8));
        }
        else {
            attron(COLOR_PAIR(7));
        }
        mvprintw(2, i + 7, " ");
        if (user -> hunger <= 5) {
            attroff(COLOR_PAIR(8));
        }
        else {
            attroff(COLOR_PAIR(7));
        }
    }
	mvprintw(LINES - 2, 0, "Press <ENTER> to eat the selected food");
	refresh();
	post_menu(my_menu);
	wrefresh(menu_win);
    int index = 0;
	while((c = wgetch(menu_win)) != KEY_F(1)) {       
        switch(c) {	
            case KEY_DOWN:
                if (index != (user -> food) - 1) {
                    index++;
                }
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
                if (index != 0) {
                    index--;
                }
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10:
			{
                
                ITEM *cur_item;
				void (*p)(User *, int);
				cur_item = current_item(my_menu);
				p = (void (*) (User *, int)) item_userptr(cur_item);
				pos_menu_cursor(my_menu);
				unpost_menu(my_menu);
				for(int i = 0; i < (user -> food); ++i)
					free_item(items[i]);
				free_menu(my_menu);
				wrefresh(menu_win);
				//(*p)(user, index);
                regular_food(user, index);
				return;
			}
		}
	}	
	unpost_menu(my_menu);
	for(int i = 0; i < (user -> food); i++)
		free_item(items[i]);
	free_menu(my_menu);
}

void regular_food(User *user, int i) {
    if (user -> hunger < 19) {
        (user -> hunger) += 2;
    }
    else if (user -> hunger == 19) {
        (user -> hunger) = 20;
    }
    for (int j = i; j < (user -> food) - 1; j++) {
        (user -> food_menu)[j] = (user -> food_menu)[j+1];
    }
    (user -> food)--;
    if (user -> health < 99) {
        (user -> health) += 2;
    }
    else if (user -> health == 99){
        (user -> health) += 1;
    }
}
void premium_food(User *user, int i) {

}
void magical_food(User *user, int i) {

}
void rotten_food(User *user, int i) {

}

#endif
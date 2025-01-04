#ifndef LOGIN_H
#define LOGIN_H
#include <curses.h>
#include <form.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "user.h"
#include "game_menu.h"

#define MAX_SIZE 50

void login_menu(int, int);
void add_char1(char *, int);
int has_username(char *, char *, char *);
int check_password_correct(char *, char *);

void login_menu(int is_username, int is_password) {
    clear();
    FIELD *field[3];
    FORM  *login;
    field[0] = new_field(1, MAX_SIZE-1, 33, 106, 0, 0);
	field[1] = new_field(1, MAX_SIZE-1, 34, 106, 0, 0);
	field[2] = NULL; 
	field_opts_off(field[0], O_AUTOSKIP);  	
	field_opts_off(field[1], O_AUTOSKIP);
    login = new_form(field);
	post_form(login);
	refresh();
    attron(COLOR_PAIR(1));
    if (is_username) {
        mvprintw(28, 89,"The entered username doesn't exist.");
    }
    if (is_password) {
        mvprintw(28, 90,"The entered password is incorrect.");
    }
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(3));
    mvprintw(31, 104, "LOG IN");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
    mvprintw(33, 95, "Username :");
    mvprintw(34, 95, "Password :");
    attroff(COLOR_PAIR(2));
    mvprintw(LINES - 2, 0, "Press F1 to return to the previous menu.");
    mvprintw(33, 105, " ");
    refresh();
    int ch;
    char entered_username[MAX_SIZE], entered_password[MAX_SIZE];
    entered_username[0] = '\0';
    entered_password[0] = '\0';
    int flag = 0;
    while((ch = getch()) != '\n' && ch != KEY_F(1)) {	
        switch(ch) {	
            case KEY_DOWN:
				form_driver(login, REQ_NEXT_FIELD);
				form_driver(login, REQ_END_LINE);
                flag = (flag + 1) % 2;
				break;
			case KEY_UP:
				form_driver(login, REQ_PREV_FIELD);
				form_driver(login, REQ_END_LINE);
                flag = (flag + 1) % 2;
				break;
			default:	
				form_driver(login, ch);
                switch(flag) {
                    case 0:
                        add_char1(entered_username, ch);
                        break;
                    case 1:
                        add_char1(entered_password, ch);
                        break;
                }
				break;
		}
	}
    unpost_form(login);
	free_form(login);
	free_field(field[0]);
	free_field(field[1]);
    if (ch == KEY_F(1))
        return;
    char file_password[MAX_SIZE], file_email[MAX_SIZE];
    is_username = has_username(entered_username, file_password, file_email);
    if (is_username) {
        login_menu(1, 0);
    }
    else {
        is_password = check_password_correct(entered_password, file_password);
        if (is_password) {
            login_menu(0, 1);
            return;
        }
        else {
            attron(COLOR_PAIR(2) | A_BLINK);
            mvprintw(33, 107 - ((strlen(entered_username)+8)/2),"Welcome %s", entered_username);
            attroff(COLOR_PAIR(2) | A_BLINK);
            refresh();
            sleep(3);
            clear();
            User user;
            User *user_ptr = &user;
            strcpy(user_ptr->username, entered_username);
            strcpy(user_ptr->password, entered_password);
            strcpy(user_ptr->email, file_email);
            game_menu_func(user_ptr);
            login_menu(0, 0);
        }
    }
}

void add_char1(char *array, int ch) {
    char c[2] = {(char) ch, '\0'};
    strcat(array, c);
}

int has_username(char *entered_username, char *file_password, char *file_email) {
    FILE *users;
    users = fopen("users.txt", "r");
    char line[MAX_SIZE];
    while (fgets(line, MAX_SIZE, users)) {
        if (strcmp(line, "{\n") == 0) {
            fgets(line, MAX_SIZE, users);
            line[strlen(line)-1] = '\0';
            if (strcmp(entered_username, line) == 0) {
                fgets(line, MAX_SIZE, users);
                line[strlen(line)-1] = '\0';
                strcpy(file_password, line);
                fgets(line, MAX_SIZE, users);
                line[strlen(line)-1] = '\0';
                strcpy(file_email, line);
                fclose(users);
                return 0;
            }
        }
    }
    fclose(users);
    return 1;
}

int check_password_correct(char *entered_password, char *file_password) {
    if (strcmp(entered_password, file_password) == 0)
        return 0;
    return 1;
}

#endif
#include <curses.h>
#include <form.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 50
#define LENGTH 214
#define WIDTH 66

typedef struct {
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char email[MAX_SIZE];
} User;

void login_menu(int, int);
void add_char(char *, int);
int has_username(char *);
int check_password_correct(char *, char *);

int main() {
    initscr();
    cbreak();
	noecho();
    keypad(stdscr, TRUE);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    login_menu(0, 0);
    endwin();
    return 0;
}

void login_menu(int is_username, int is_password) {
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
    mvprintw(33, 105, " ");
    attroff(COLOR_PAIR(2));
    refresh();
    int ch;
    char entered_username[MAX_SIZE], entered_password[MAX_SIZE];
    entered_username[0] = '\0';
    entered_password[0] = '\0';
    int flag = 0;
    while((ch = getch()) != '\n') {	
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
                        add_char(entered_username, ch);
                        break;
                    case 1:
                        add_char(entered_password, ch);
                        break;
                }
				break;
		}
	}
    unpost_form(login);
	free_form(login);
	free_field(field[0]);
	free_field(field[1]);
    is_username = has_username(entered_username);
    if (is_username) {
        login_menu(1, 0);
    }
    else {
        is_password = check_password_correct(entered_username, entered_password);
        if (is_password) {
            login_menu(0, 1);
            return;
        }
        else {
            attron(COLOR_PAIR(2));
            mvprintw(33, 107 - ((strlen(entered_username)+8)/2),"Welcome %s", entered_username);
            attroff(COLOR_PAIR(2));
        }
    }
    while (true) {
        char c = getch();
        if (c == 'q')
            break;
        if (c == 'c')
            clear();
    }
}

void add_char(char *array, int ch) {
    char c[2] = {(char) ch, '\0'};
    strcat(array, c);
}

int has_username(char *entered_username) {
    FILE *users;
    users = fopen("users.txt", "r");
    char line[MAX_SIZE];
    int line_number = 0;
    while (fgets(line, MAX_SIZE, users)) {
        if(line_number % 4 == 1) {
            line[strlen(line)-1] = '\0';
            if (strcmp(entered_username, line) == 0) {
                fclose(users);
                return 0;
            }
        }
        line_number++;
    }
    fclose(users);
    return 1;
}

int check_password_correct(char *entered_username, char *entered_password) {
    FILE *users;
    users = fopen("users.txt", "r");
    char line[MAX_SIZE];
    int line_number = 0;
    while (fgets(line, MAX_SIZE, users)) {
        if(line_number % 4 == 1) {
            line[strlen(line)-1] = '\0';
            if (strcmp(entered_username, line) == 0) {
                fgets(line, MAX_SIZE, users);
                line[strlen(line)-1] = '\0';
                fclose(users);
                if (strcmp(entered_password, line) == 0) {
                    return 0;
                }
                else {
                    return 1;
                }
            }
        }
        line_number++;
    }
    fclose(users);
    return 1;
}
#include <curses.h>
#include <form.h>
#include <string.h>

#define MAX_SIZE 50
#define LENGTH 214
#define WIDTH 66

typedef struct {
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char email[MAX_SIZE];
} User;

void create_account_menu(int, int, int, int);
void add_char(char *, int);
int check_username(char *);
int check_length_password(char *);
int check_password(char *);
int check_email(char *);

int main() {
    initscr();
    cbreak();
	noecho();
    keypad(stdscr, TRUE);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    create_account_menu(1, 1, 1, 1);
    endwin();
    return 0;
}

void create_account_menu(int is_username, int length_password, int is_password, int is_email) {
    FIELD *field[4];
    FORM  *new_account;
    field[0] = new_field(1, MAX_SIZE-1, 32, 106, 0, 0);
	field[1] = new_field(1, MAX_SIZE-1, 33, 106, 0, 0);
    field[2] = new_field(1, MAX_SIZE-1, 34, 111, 0, 0);
	field[3] = NULL; 
	field_opts_off(field[0], O_AUTOSKIP);  	
	field_opts_off(field[1], O_AUTOSKIP);
	field_opts_off(field[2], O_AUTOSKIP);
    new_account = new_form(field);
	post_form(new_account);
	refresh();
    attron(COLOR_PAIR(1));
    int line_number = 25;
    if (is_username) {
        mvprintw(line_number, 91,"The entered username does exist.");
        line_number++;
    }
    if (length_password) {
        mvprintw(line_number, 88,"The password lenght must be at least 7.");
        line_number++;
    }
    if (is_password) {
        mvprintw(line_number, 61,"The password must contain at least one digit, one small character and one capital character.");
        line_number++;
    }
    if (is_email) {
        mvprintw(line_number, 87,"The email must have the format xxx@y.zzz");
    }
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    mvprintw(31, 101, "Create Account");
    mvprintw(32, 95, "Username :");
    mvprintw(33, 95, "Password :");
    mvprintw(34, 95, "Email Address :");
    mvprintw(32, 105, " ");
    attroff(COLOR_PAIR(2));
    refresh();
    int ch;
    char entered_username[MAX_SIZE], entered_password[MAX_SIZE], entered_email[MAX_SIZE];
    entered_username[0] = '\0';
    entered_password[0] = '\0';
    entered_email[0] = '\0';
    int flag = 0;
    while((ch = getch()) != '\n') {	
        switch(ch) {	
            case KEY_DOWN:
				form_driver(new_account, REQ_NEXT_FIELD);
				form_driver(new_account, REQ_END_LINE);
                flag = (flag + 1) % 3;
				break;
			case KEY_UP:
				form_driver(new_account, REQ_PREV_FIELD);
				form_driver(new_account, REQ_END_LINE);
                flag = (flag + 2) % 3;
				break;
			default:	
				form_driver(new_account, ch);
                switch(flag) {
                    case 0:
                        add_char(entered_username, ch);
                        break;
                    case 1:
                        add_char(entered_password, ch);
                        break;
                    case 2:
                        add_char(entered_email, ch);
                        break;
                }
				break;
		}
	}
    unpost_form(new_account);
	free_form(new_account);
	free_field(field[0]);
	free_field(field[1]);
    free_field(field[2]);
    //if it's ok the returned value will be 0. Else it will be 1.
    is_username = check_username(entered_username);
    length_password = check_length_password(entered_password);
    is_password = check_password(entered_password);
    is_email = check_email(entered_email);
    if (is_username || length_password || is_password || is_email) {
        create_account_menu(is_username, length_password, is_password, is_email);
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

int check_username(char *array) {
    return 0;
}

int check_length_password(char *array) {
    return 0;
}

int check_password(char *array) {
    return 0;
}

int check_email(char *array) {
    return 0;
}
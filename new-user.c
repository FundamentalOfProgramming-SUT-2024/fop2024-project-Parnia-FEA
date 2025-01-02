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

void create_account_menu();
void add_char(char *, int);

int main() {
    initscr();
    cbreak();
	noecho();
    keypad(stdscr, TRUE);
    create_account_menu();
    endwin();
    return 0;
}

void create_account_menu() {
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
    mvprintw(31, 101, "Create Account");
    mvprintw(32, 95, "Username :");
    mvprintw(33, 95, "Password :");
    mvprintw(34, 95, "Email Address :");
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
    mvprintw(35, 95, "Username : %s", entered_username);
    mvprintw(36, 95, "Password : %s", entered_password);
    mvprintw(37, 95, "Email Address : %s", entered_email);
    while (true) {
        char c = getch();
        if (c == 'q')
            break;
        if (c == 'c')
            clear();
    }
    unpost_form(new_account);
	free_form(new_account);
	free_field(field[0]);
	free_field(field[1]);
    free_field(field[2]);
}

void add_char(char *array, int ch) {
    char c[2] = {(char) ch, '\0'};
    strcat(array, c);
}
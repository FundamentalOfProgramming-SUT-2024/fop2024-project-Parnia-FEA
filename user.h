#ifndef USER_H
#define USER_H

#define MAX_SIZE 50

typedef struct {
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char email[MAX_SIZE];
} User;

#endif
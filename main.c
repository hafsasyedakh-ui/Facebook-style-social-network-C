/* main.c
Author: Hafsa Syeda
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "a3_functions.h"


/* -------- helpers -------- */
static void trim_newline(char *s) {
    if (!s) return;
    s[strcspn(s, "\r\n")] = '\0';
}

static bool password_valid(const char *pw) {
    if (!pw) return false;
    size_t n = strlen(pw);
    if (n < 8 || n > 14) return false;
    for (size_t i = 0; i < n; i++) {
        if (isspace((unsigned char)pw[i])) return false;
    }
    return true;
}

static int read_int(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    return (int)strtol(buf, NULL, 10);
}

int main(void) {
    user_t *users = NULL;

    FILE *csv = fopen("user_details.csv", "r");
    if (csv) {
        users = read_CSV_and_create_users(csv, 50);
        fclose(csv);
    } else {
        printf("Warning: could not open user_details.csv (starting with empty list).\n");
    }

    while (1) {
        print_menu();
        printf("Choice: ");
        int choice = read_int();

        if (choice == 1) {
            char username[64], password[64];
            printf("Username: ");
            if (!fgets(username, sizeof(username), stdin)) continue;
            trim_newline(username);

            printf("Password (8-14 chars, no spaces): ");
            if (!fgets(password, sizeof(password), stdin)) continue;
            trim_newline(password);

            if (!password_valid(password)) {
                printf("Invalid password.\n");
                continue;
            }

            user_t *before = users;
            users = add_user(users, username, password);
            if (users == before) {
                // could be duplicate or invalid (defensive)
                user_t *u = find_user(users, username);
                if (u) printf("Username already exists.\n");
                else printf("User not added.\n");
            } else {
                printf("User registered.\n");
            }
        }

        else if (choice == 2) {
            char username[64], password[64];
            printf("Username: ");
            if (!fgets(username, sizeof(username), stdin)) continue;
            trim_newline(username);

            user_t *u = find_user(users, username);
            if (!u) {
                printf("User not found.\n");
                continue;
            }

            printf("New password (8-14 chars, no spaces): ");
            if (!fgets(password, sizeof(password), stdin)) continue;
            trim_newline(password);

            if (!password_valid(password)) {
                printf("Invalid password.\n");
                continue;
            }

            strncpy(u->password, password, 14);
            u->password[14] = '\0';
            printf("Password changed.\n");
        }

        else if (choice == 3) {
            char username[64], post[512];
            printf("Username: ");
            if (!fgets(username, sizeof(username), stdin)) continue;
            trim_newline(username);

            user_t *u = find_user(users, username);
            if (!u) {
                printf("User not found.\n");
                continue;
            }

            printf("Post text: ");
            if (!fgets(post, sizeof(post), stdin)) continue;
            trim_newline(post);

            if (post[0] == '\0') {
                printf("Empty post rejected.\n");
                continue;
            }

            add_post(u, post);
            printf("Post added.\n");
        }

        else if (choice == 4) {
            char username[64];
            printf("Username: ");
            if (!fgets(username, sizeof(username), stdin)) continue;
            trim_newline(username);

            user_t *u = find_user(users, username);
            if (!u) {
                printf("User not found.\n");
                continue;
            }

            if (delete_post(u)) printf("Latest post deleted.\n");
            else printf("No posts to delete.\n");
        }

        else if (choice == 5) {
            char username[64], fname[64];
            printf("Username: ");
            if (!fgets(username, sizeof(username), stdin)) continue;
            trim_newline(username);

            user_t *u = find_user(users, username);
            if (!u) {
                printf("User not found.\n");
                continue;
            }

            printf("Friend username: ");
            if (!fgets(fname, sizeof(fname), stdin)) continue;
            trim_newline(fname);

            if (fname[0] == '\0') {
                printf("Empty friend name rejected.\n");
                continue;
            }

            add_friend(u, fname); // spec: don't check duplicates, don't check existence
            printf("Friend added.\n");
        }

        else if (choice == 6) {
            char username[64], fname[64];
            printf("Username: ");
            if (!fgets(username, sizeof(username), stdin)) continue;
            trim_newline(username);

            user_t *u = find_user(users, username);
            if (!u) {
                printf("User not found.\n");
                continue;
            }

            printf("Friend username to delete: ");
            if (!fgets(fname, sizeof(fname), stdin)) continue;
            trim_newline(fname);

            if (delete_friend(u, fname)) printf("Friend deleted.\n");
            else printf("Friend not found.\n");
        }

        else if (choice == 7) {
            display_posts_by_n(users, 3);
        }

        else if (choice == 8) {
            char username[64];
            printf("Username: ");
            if (!fgets(username, sizeof(username), stdin)) continue;
            trim_newline(username);

            user_t *u = find_user(users, username);
            display_user_friends(u);
        }

        else if (choice == 9) {
            break;
        }

        else {
            printf("Invalid choice.\n");
        }
    }

    teardown(users);
    printf("Goodbye.\n");
    return 0;
}


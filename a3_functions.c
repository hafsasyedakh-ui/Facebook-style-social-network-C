/*
/*
  Author = Hafsa Syeda
  Implements user list (sorted), friend lists (sorted),
  post stacks (LIFO), CSV loading, and full teardown
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "a3_functions.h"



#include "a3_functions.h"

//helpers 
static void str_to_lower(char *s) {
    if (!s) return;
    for (int i = 0; s[i]; i++) s[i] = (char)tolower((unsigned char)s[i]);
}

static int contains_space(const char *s) {
    if (!s) return 0;
    for (int i = 0; s[i]; i++) {
        if (isspace((unsigned char)s[i])) return 1;
    }
    return 0;
}

//USER OPS 
user_t *add_user(user_t *users, const char *username, const char *password) {
    if (!username || !password) return users;

    char uname[31];
    strncpy(uname, username, 30);
    uname[30] = '\0';
    str_to_lower(uname);

    // Defensive password rule (even if main validates)
    size_t pwlen = strlen(password);
    if (pwlen < 8 || pwlen > 14 || contains_space(password)) {
        return users; // reject silently (or print in main)
    }

    user_t *prev = NULL;
    user_t *curr = users;

    // Find insert point (sorted ascending)
    while (curr && strcmp(curr->username, uname) < 0) {
        prev = curr;
        curr = curr->next;
    }

    // Duplicate username
    if (curr && strcmp(curr->username, uname) == 0) {
        return users;
    }

    user_t *node = (user_t *)malloc(sizeof(user_t));
    if (!node) return users;

    strncpy(node->username, uname, 30);
    node->username[30] = '\0';

    // password is char[15] in struct; ensure null-termination
    strncpy(node->password, password, 14);
    node->password[14] = '\0';

    node->friends = NULL;
    node->posts = NULL;

    // Insert
    node->next = curr;
    if (prev) prev->next = node;
    else users = node;

    return users;
}

user_t *find_user(user_t *users, const char *username) {
    if (!username) return NULL;

    char uname[31];
    strncpy(uname, username, 30);
    uname[30] = '\0';
    str_to_lower(uname);

    for (user_t *cur = users; cur; cur = cur->next) {
        if (strcmp(cur->username, uname) == 0) return cur;
    }
    return NULL;
}

//FRIEND OPS (sorted list) 
friend_t *create_friend(const char *username) {
    if (!username) return NULL;

    friend_t *f = (friend_t *)malloc(sizeof(friend_t));
    if (!f) return NULL;

    strncpy(f->username, username, 30);
    f->username[30] = '\0';
    f->next = NULL;
    return f;
}

void add_friend(user_t *user, const char *friend_name) {
    if (!user || !friend_name) return;

    // store friend usernames lowercase too (keeps sorting consistent)
    char fname[31];
    strncpy(fname, friend_name, 30);
    fname[30] = '\0';
    str_to_lower(fname);

    friend_t *newf = create_friend(fname);
    if (!newf) return;

    friend_t *prev = NULL;
    friend_t *curr = user->friends;

    while (curr && strcmp(curr->username, fname) < 0) {
        prev = curr;
        curr = curr->next;
    }

    newf->next = curr;
    if (prev) prev->next = newf;
    else user->friends = newf;
}

_Bool delete_friend(user_t *user, char *friend_name) {
    if (!user || !friend_name) return 0;

    char fname[31];
    strncpy(fname, friend_name, 30);
    fname[30] = '\0';
    str_to_lower(fname);

    friend_t *prev = NULL;
    friend_t *curr = user->friends;

    while (curr) {
        if (strcmp(curr->username, fname) == 0) {
            if (prev) prev->next = curr->next;
            else user->friends = curr->next;
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

//POST OPS (stack) 
post_t *create_post(const char *text) {
    if (!text) return NULL;

    post_t *p = (post_t *)malloc(sizeof(post_t));
    if (!p) return NULL;

    strncpy(p->content, text, sizeof(p->content) - 1);
    p->content[sizeof(p->content) - 1] = '\0';
    p->next = NULL;
    return p;
}

void add_post(user_t *user, const char *text) {
    if (!user || !text) return;

    post_t *p = create_post(text);
    if (!p) return;

    p->next = user->posts;
    user->posts = p;
}

_Bool delete_post(user_t *user) {
    if (!user || !user->posts) return 0;

    post_t *top = user->posts;
    user->posts = top->next;
    free(top);
    return 1;
}

//DISPLAY
void display_all_user_posts(user_t *user) {
    if (!user) {
        printf("User not found.\n");
        return;
    }
    if (!user->posts) {
        printf("No posts.\n");
        return;
    }
    for (post_t *p = user->posts; p; p = p->next) {
        printf("- %s\n", p->content);
    }
}

void display_user_friends(user_t *user) {
    if (!user) {
        printf("User not found.\n");
        return;
    }
    if (!user->friends) {
        printf("No friends.\n");
        return;
    }
    for (friend_t *f = user->friends; f; f = f->next) {
        printf("- %s\n", f->username);
    }
}

void display_posts_by_n(user_t *users, int number) {
    if (!users) {
        printf("No users loaded.\n");
        return;
    }
    if (number <= 0) number = 3;

    char username[64];
    printf("Enter username: ");
    if (!fgets(username, sizeof(username), stdin)) return;
    username[strcspn(username, "\r\n")] = '\0';

    user_t *user = find_user(users, username);
    if (!user) {
        printf("User not found.\n");
        return;
    }
    if (!user->posts) {
        printf("No posts.\n");
        return;
    }

    post_t *p = user->posts;
    int count = 0;

    while (p) {
        printf("- %s\n", p->content);
        p = p->next;
        count++;

        if (count == number && p) {
            char ans[16];
            printf("More posts? (Y/N): ");
            if (!fgets(ans, sizeof(ans), stdin)) break;
            if (!(ans[0] == 'Y' || ans[0] == 'y')) break;
            count = 0;
        }
    }
}

//CLEANUP 
void teardown(user_t *users) {
    while (users) {
        // friends
        friend_t *f = users->friends;
        while (f) {
            friend_t *nf = f->next;
            free(f);
            f = nf;
        }

        // posts
        post_t *p = users->posts;
        while (p) {
            post_t *np = p->next;
            free(p);
            p = np;
        }

        user_t *nu = users->next;
        free(users);
        users = nu;
    }
}

// MENU 
void print_menu() {
    printf("\n========== A3 MENU ==========\n");
    printf("1. Register user\n");
    printf("2. Change password\n");
    printf("3. Add post\n");
    printf("4. Delete post\n");
    printf("5. Add friend\n");
    printf("6. Delete friend\n");
    printf("7. Display user posts (3 at a time)\n");
    printf("8. Display user friends\n");
    printf("9. Exit\n");
    printf("=============================\n");
}


user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}

# Facebook-Style Social Network (C)

A terminal-based social network simulator written in C.  
The program models users, friendships, and posts using dynamic data structures.

## Features
- Sorted linked list of users (case-insensitive usernames)
- Sorted linked list of friends per user
- Stack-based post system (LIFO)
- CSV file loading for initial users, friends, and posts
- Interactive terminal menu
- Complete dynamic memory cleanup

## Build
```bash
gcc -g *.c -o social_network

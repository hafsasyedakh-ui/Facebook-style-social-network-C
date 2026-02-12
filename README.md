# Facebook-Style Social Network Simulator (C)
  A modular, terminal-based social network simulator written in C that models users, friendships, and posts using dynamic memory and custom data structures.
  This project demonstrates strong understanding of pointers, linked lists, stacks, file I/O, memory management, and structured debugging in a Linux environment.

Overview:
  The program simulates core social network functionality including:
  User creation and management
  Friendship relationships
  Post creation and retrieval
  CSV-based data initialization
  Interactive terminal-driven interface
  All data is stored using dynamically allocated structures with complete memory cleanup.

Technologies Used:
  C Programming
  GCC Compiler
  Ubuntu (Linux CLI)
  Dynamic Memory Allocation
  Linked Lists & Stacks
  File I/O

main.c:
  users.c / users.h
  friends.c / friends.h
  posts.c / posts.h
  Each module is responsible for a specific system component to maintain separation of concerns and scalability.

Build & Run:
  gcc -g *.c -o social_network
  ./social_network

  The -g flag enables debugging symbols for use with tools like gdb.

Key Learning Outcomes:
  Implementing custom data structures in C
  Managing dynamic memory safely
  Debugging pointer-related defects
  Designing modular, maintainable C code
  Working within a Linux development environment


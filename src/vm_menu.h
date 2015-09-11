/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 ***********************************************************************/

#include "vm_options.h"

#ifndef VM_MENU
#define VM_MENU

#define NUM_NORMAL_OPTS 3
#define NUM_ADMIN_OPTS  6
#define NUM_MENU_OPTS   NUM_NORMAL_OPTS + NUM_ADMIN_OPTS
#define ITEM_NAME_LEN   40

/* The data structure to hold information about each menu option, the 
 * name of the function to display and a pointer to the function that 
 * will implement that option. */
struct menu_item
{
    char name[ITEM_NAME_LEN+1];
    BOOLEAN (*func)(struct vm*);
};

/* Builds the menu for the vending machine. */
void menu_init(struct menu_item menu[]);
/* Prints the menu for the vending machine */
void print_menu(struct menu_item menu[]);
/* Get menu selection from user input */
int get_valid_selection(struct menu_item menu[]);

#endif

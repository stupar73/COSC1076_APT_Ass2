/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 ***********************************************************************/
#include "vm_menu.h"

/* 
 * Builds the menu for the vending machine.
 * */
void menu_init(struct menu_item menu[])
{
    int i;
    /* Create arrays for the relevant struct members for each menu item */
    char *options[] = {"Display Items", "Purchase Item", "Save and Exit",
                       "Add Item", "Remove Item", "Display Coins",
                       "Reset Stock", "Reset Coins", "Abort Program"};
    BOOLEAN (*options_funcs[])(struct vm*) =
    {
        display_items, purchase_item, save, add_item, delete_item,
        display_coins, reset_stock_count, reset_coin_count, abort_program
    };
    
    /* Assign the struct members to each menu item */
    for(i = 0; i < NUM_MENU_OPTS; i++)
    {
        strcpy(menu[i].name, options[i]);
        menu[i].func = options_funcs[i];
    }
}

/*
 * Prints the menu for the vending machine
 * */
void print_menu(struct menu_item menu[])
{
    int i;
    
    printf("Main Menu:\n");
    for(i = 0; i < NUM_NORMAL_OPTS; i++)
    {
        printf("  %d. %s\n", i+1, menu[i].name);
    }
    printf("Administrator-Only Menu:\n");
    for(i = NUM_NORMAL_OPTS; i < NUM_MENU_OPTS; i++)
    {
        printf("  %d. %s\n", i+1, menu[i].name);
    }
}

/*
 * Get menu selection from user input
 * */
int get_valid_selection(struct menu_item menu[])
{
    int selection;
    enum int_result selection_result;
    BOOLEAN valid_option_selected = FALSE;
    
    while(!valid_option_selected)
    {
        printf("\nEnter selection: ");
        selection_result = get_int(&selection, BUFFER_SIZE, 1, NUM_MENU_OPTS, stdin);
        
        /* Test if input was a valid selection */
        if(selection_result == INT_SUCCESS)
        {
            valid_option_selected = TRUE;
        }
        else
        {
            printf("\nInvalid input\n\n");
            print_menu(menu);
        }
    }
    return selection;
}

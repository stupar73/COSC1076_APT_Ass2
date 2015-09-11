/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 **********************************************************************/

#include "vm_menu.h"

#define NUM_ARGS 3

int main(int argc, char *argv[])
{
    struct vm vm;
    struct menu_item menu[NUM_MENU_OPTS];
    int selection;
    BOOLEAN exit = FALSE;
    
    /* Check command line arguments */
    if(argc != NUM_ARGS)
    {
        fprintf(stderr, "Fatal error: Incorrect command line arguments supplied\n");
        fprintf(stderr, "Usage: %s <itemsfile> <coinsfile>\n", argv[0]);
        fprintf(stderr, "Exiting...\n");
        return EXIT_FAILURE;
    }
    
    /* Initialise vending machine data structure, exit if it fails */
    if(!vm_init(&vm))
    {
        fprintf(stderr, "Fatal error: Unable to allocate memory for vm\n");
        fprintf(stderr, "Exiting...\n");
        return EXIT_FAILURE;
    }
    
    menu_init(menu);
    
    /* Load data, exit if unable to load data (empty item file is acceptable) */
    if(!load_data(&vm, argv[1], argv[2]))
    {
        fprintf(stderr, "Fatal error: Unable to load data from provided files\n");
        return EXIT_FAILURE;
    }
    
    /* Display menu and allow interaction */
    do
    {
        print_menu(menu);
        selection = get_valid_selection(menu);
        
        /* Call relevant function based on users selection */
        menu[selection-1].func(&vm);
        
        /* Exit when appropriate option has been selected */
        if(selection == 3 || selection == 9)
        {
            exit = TRUE;
        }
        else
        {
            print_multi('-', 30);
            print_multi('\n', 2);
        }
    } while(!exit);
    
    system_free(&vm);
    
    return EXIT_SUCCESS;
}

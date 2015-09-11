/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 **********************************************************************/
#include "vm_options.h"

/* 
 * Displays a formatted list of the items currently for sale in the 
 * machine. This will fulfil requirement 5 of the spec.
 * */
BOOLEAN display_items(struct vm *vm)
{
    struct vm_node *curr;
    struct stock_item *data;
    
    if(!vm)
    {
        return FALSE;
    }
    if(vm->item_list->length == 0)
    {
        printf("\nNo items are in the vending machine.\n");
        return TRUE;
    }
    
    curr = vm->item_list->head;
    
    printf("\nItems Menu\n");
    print_multi('-', 70);
    putchar('\n');
    printf("%-*s ", ID_LEN, "ID");
    printf("| %-*s ", NAME_LEN, "Name");
    printf("| Available | Price\n");
    print_multi('-', 70);
    putchar('\n');
    
    while(curr != NULL)
    {
        data = curr->data;
        printf("%-*s ", ID_LEN, data->id);
        printf("| %-*s ", NAME_LEN, data->name);
        printf("| %-*u ", (int)strlen("Available"), data->on_hand);
        printf("| $%3u.%02u\n", data->price.dollars, data->price.cents);
        
        curr = curr->next;
    }
    putchar('\n');
    
    return TRUE;
}

/* 
 * Displays a formatted list of the coins currently on hand in the 
 * vending machine. This fulfils requirement 12 of the spec.
 * */
BOOLEAN display_coins(struct vm *vm)
{
    int i;
    char *denom_strings[] = {"5 cents", "10 cents", "20 cents",
                             "50 cents", "1 dollar", "2 dollars",
                             "5 dollars", "10 dollars"};
    if(!vm)
    {
        return FALSE;
    }
    
    printf("\nCoins Summary\n");
    print_multi('-', 20);
    putchar('\n');
    printf("Denomination | Count\n");
    print_multi('-', 20);
    putchar('\n');
    
    for(i = 0; i < NUM_DENOMS; i++)
    {
        printf("%-*s ", (int)strlen("Denomination"), denom_strings[i]);
        printf("| %*u\n", (int)strlen("Count"), vm->coins[i].count);
    }
    putchar('\n');
    
    return TRUE;
}

/* 
 * Allows a customer to purchase an item for the vending machine. This
 * fulfils requirement 6 of the spec.
 * */
BOOLEAN purchase_item(struct vm *vm)
{
    char id[ID_LEN];
    struct stock_item *item = NULL;
    enum string_result res;
    BOOLEAN valid_id_read = FALSE;
    int remaining;
    unsigned current, change_owed;
    BOOLEAN paid = FALSE;
    char current_str[PRICE_STR_LEN];
    struct coin coins_entered[NUM_DENOMS], change[NUM_DENOMS];
    
    init_coins(coins_entered);
    init_coins(change);
    
    printf("Press Enter or Ctrl-D at any time to return to the menu, ");
    printf("transaction will not be processed.\n\n");
    
    /* Get id of item from user, print details and price of item selected */
    while(!valid_id_read)
    {
        printf("Enter the id of the item you wish to purchase: ");
        res = get_string(id, ID_LEN+2, stdin);
        if(res == STRING_SUCCESS && is_valid_field(id, ID))
        {
            if(find_item(vm->item_list, id, &item))
            {
                valid_id_read = TRUE;
                printf("You have selected \"%s - %s\". ",
                            item->name, item->desc);
                printf("This will cost you $%u.%02u.\n\n",
                            item->price.dollars, item->price.cents);
                remaining = item->price.dollars*100 + item->price.cents;
            }
            else
            {
                printf("Error: No item with id '%s' found.\n\n", id);
            }
        }
        else if(res == STRING_EMPTY)
        {
            /* Return to menu */
            putchar('\n');
            return FALSE;
        }
        else
        {
            printf("Error: Invalid item id format.\n\n");
        }
    }
    
    printf("Please insert the money - type in the value of each ");
    printf("coin/note in cents.\n\n");
    
    /* Collect money from user until item paid for or return to menu
     * requested */
    while(!paid)
    {
        printf("You still need to pay $%d.%02d, insert money: ",
               remaining / 100, remaining % 100);
        res = get_string(current_str, PRICE_STR_LEN, stdin);
        if(res == STRING_SUCCESS)
        {
            if(str_to_cents(current_str, &current))
            {
                remaining -= current;
                /* Add money entered by user to coins_entered array and
                 * vm coins array */
                add_denom(coins_entered, current, 1);
                add_denom(vm->coins, current, 1);
                if(remaining <= 0)
                {
                    /* Item paid for (may require change) */
                    paid = TRUE;
                    putchar('\n');
                }
            }
            else if(str_to_unsigned(current_str, &current))
            {
                /* Valid price but not valid individual coin */
                printf("Error: $%u.%02u is not a valid denomination.\n\n",
                       current / 100, current % 100);
            }
            else
            {
                printf("Error: Invalid input\n\n");
            }
        }
        else if(res == STRING_EMPTY)
        {
            /* Return to menu requested, remove coins entered by user from
             * vm coins array and return to user (print) */
            printf("\nTransaction cancelled.\n");
            printf("Here is your money back: ");
            remove_coins(vm->coins, coins_entered);
            print_coins(coins_entered);
            printf("Please come again soon.\n\n");
            return FALSE;
        }
        else
        {
            printf("Error: Invalid input\n\n");
        }
    }
    
    /* Hand change back to user (if necessary) */
    if(remaining < 0)
    {
        change_owed = -1*remaining;
        if(!give_change(change_owed, vm->coins, change))
        {
            /* Insufficient change in vm, return coins entered to user and
             * cancel transaction */
            printf("Error: Not enough change.\n");
            printf("Here is your money back: ");
            remove_coins(vm->coins, coins_entered);
            print_coins(coins_entered);
        }
        else
        {
            printf("Thank you. Here is your %s, ", item->name);             
            printf("and your change of $%d.%02d: ",
                   change_owed / 100, change_owed % 100);
            remove_coins(vm->coins, change);
            print_coins(change);
            item->on_hand--;
        }
    }
    else
    {
        printf("Thank you. Here is your coke.\n\n");
        item->on_hand--;
    }
    
    printf("Please come again soon.\n\n");
    
    return TRUE;
}

/*
 * Add coins to change array to return to user, if not enough change in vm
 * return FALSE
 * */
BOOLEAN give_change(unsigned change_owed, struct coin available[],
    struct coin change[])
{
    while(change_owed / 1000 &&
          (available[TEN_DOLLARS].count - change[TEN_DOLLARS].count) > 0)
    {
        add_coin(change, TEN_DOLLARS, 1);
        change_owed -= 1000;
    }
    while(change_owed / 500 &&
          (available[FIVE_DOLLARS].count - change[FIVE_DOLLARS].count) > 0)
    {
        add_coin(change, FIVE_DOLLARS, 1);
        change_owed -= 500;
    }
    while(change_owed / 200 &&
          (available[TWO_DOLLARS].count - change[TWO_DOLLARS].count) > 0)
    {
        add_coin(change, TWO_DOLLARS, 1);
        change_owed -= 200;
    }
    while(change_owed / 100 &&
          (available[ONE_DOLLAR].count - change[ONE_DOLLAR].count) > 0)
    {
        add_coin(change, ONE_DOLLAR, 1);
        change_owed -= 100;
    }
    while(change_owed / 50 &&
          (available[FIFTY_CENTS].count - change[FIFTY_CENTS].count) > 0)
    {
        add_coin(change, FIFTY_CENTS, 1);
        change_owed -= 50;
    }
    while(change_owed / 20 &&
          (available[TWENTY_CENTS].count - change[TWENTY_CENTS].count) > 0)
    {
        add_coin(change, TWENTY_CENTS, 1);
        change_owed -= 20;
    }
    while(change_owed / 10 &&
          (available[TEN_CENTS].count - change[TEN_CENTS].count) > 0)
    {
        add_coin(change, TEN_CENTS, 1);
        change_owed -= 10;
    }
    while(change_owed / 5 &&
          (available[FIVE_CENTS].count - change[FIVE_CENTS].count) > 0)
    {
        add_coin(change, FIVE_CENTS, 1);
        change_owed -= 5;
    }
    
    if(change_owed)
    {
        return FALSE;
    }
    return TRUE;
}

/* 
 * Adds an item to the vending machine. This fulfils requirement 8 of
 * the spec.
 * */
BOOLEAN add_item(struct vm *vm)
{
    struct stock_item *new_item;
    char id[ID_LEN+1], name[NAME_LEN+1], desc[DESC_LEN+1];
    char price[PRICE_STR_LEN], *dollars_str, *cents_str;
    unsigned dollars, cents;
    enum string_result res;
    BOOLEAN valid_name_read = FALSE;
    BOOLEAN valid_desc_read = FALSE;
    BOOLEAN valid_price_read = FALSE;
    
    get_next_id(id, vm->item_list);
    
    printf("This new item will have the item id of %s\n\n", id);
    printf("Press Enter or Ctrl-D at any time to return to the menu, ");
    printf("item will not be added\n\n");
    
    /* Request user input for name, loop until valid name entered or return to
     * menu requested */
    while(!valid_name_read)
    {
        printf("Enter the item name: ");
        res = get_string(name, NAME_LEN+2, stdin);
        if(res == STRING_SUCCESS && is_valid_field(name, NAME))
        {
            valid_name_read = TRUE;
        }
        else if(res == STRING_EMPTY)
        {
            putchar('\n');
            return FALSE;
        }
        else
        {
            printf("Error: Invalid item name\n\n");
        }
    }
    
    /* Request user input for description, loop until valid description
     * entered or return to menu requested */
    while(!valid_desc_read)
    {
        printf("Enter the item description: ");
        res = get_string(desc, DESC_LEN+2, stdin);
        if(res == STRING_SUCCESS && is_valid_field(desc, DESC))
        {
            valid_desc_read = TRUE;
        }
        else if(res == STRING_EMPTY)
        {
            putchar('\n');
            return FALSE;
        }
        else
        {
            printf("Error: Invalid item description\n\n");
        }
    }
    
    /* Request user input for price, loop until valid price entered or return
     * to menu requested */
    while(!valid_price_read)
    {
        printf("Enter the price for this item: ");
        res = get_string(price, PRICE_STR_LEN+2, stdin);
        if(res == STRING_SUCCESS && is_valid_field(price, PRICE))
        {
            valid_price_read = TRUE;
            /* Convert price to two unsigned variables */
            dollars_str = strtok(price, PRICE_DELIM);
            cents_str = strtok(NULL, PRICE_DELIM);
            str_to_unsigned(dollars_str, &dollars);
            str_to_unsigned(cents_str, &cents);
        }
        else if(res == STRING_EMPTY)
        {
            putchar('\n');
            return FALSE;
        }
        else
        {
            printf("Error: Invalid item price\n\n");
        }
    }
    
    /* Allocate memory for item, initialise it to safe values, assign to it
     * the values entered by user and add it to the linked list */
    new_item = malloc(sizeof *new_item);
    memset(new_item, 0, sizeof *new_item);
    create_item(new_item, id, name, desc, dollars, cents, DEFAULT_ONHAND);
    add_node(vm->item_list, new_item);
    
    printf("\n\"%s - %s - %s\" has been added to the system.\n\n",
           new_item->id, new_item->name, new_item->desc);
    
    return TRUE;
}

/* 
 * Removes an item from the vending machine and frees all dynamically 
 * allocated memory as required. This fulfils requirement 9 of the  spec.
 * */
BOOLEAN delete_item(struct vm *vm)
{
    char id[ID_LEN+1];
    struct stock_item deleted_item;
    enum string_result res;
    BOOLEAN valid_id_read = FALSE;
    
    /* Zero out memory for deleted_item */
    memset(&deleted_item, 0, sizeof deleted_item);
    
    /* Request user input for id, loop until valid id entered or return to
     * menu requested */
    while(!valid_id_read)
    {
        printf("Enter the item id of the item to remove from the menu ");
        printf("(Enter or Ctrl-D to return to menu): ");
        res = get_string(id, ID_LEN+2, stdin);
        if(res == STRING_SUCCESS && is_valid_field(id, ID))
        {
            if(delete_node(vm->item_list, id, &deleted_item))
            {
                valid_id_read = TRUE;
                printf("\n\"%s - %s - %s\" has been removed from the system.\n\n",
                       deleted_item.id, deleted_item.name, deleted_item.desc);
            }
            else
            {
                fprintf(stderr, "Error: No item with id '%s' found\n\n", id);
            }
        }
        else if(res == STRING_EMPTY)
        {
            putchar('\n');
            return FALSE;
        }
        else
        {
            fprintf(stderr, "Error: Invalid item id format\n\n");
        }
    }
    
    return TRUE;
}

/* 
 * Sets the count of each coin to the default value. This fulfils 
 * requirement 11 of the spec.
 * */
BOOLEAN reset_coin_count(struct vm *vm)
{
    int i;
    
    if(!vm)
    {
        return FALSE;
    }
    
    /* Iterate through each coin and set its count to DEFAULT_COIN_COUNT */
    for(i = 0; i < NUM_DENOMS; i++)
    {
        vm->coins[i].count = DEFAULT_COIN_COUNT;
    }
    
    printf("\nCoin count reset to default values.\n\n");
    
    return TRUE;
}

/* 
 * Sets the count of each stock item to the default value. This fulfils 
 * requirement 10 of the spec.
 * */
BOOLEAN reset_stock_count(struct vm *vm)
{
    struct vm_node *node;
    
    if(!vm)
    {
        return FALSE;
    }
    
    /* Starting at the head of the list, traverse through the list setting
     * each stock items on hand count to the default value DEFAULT_ONHAND */
    node = vm->item_list->head;
    while(node != NULL)
    {
        node->data->on_hand = DEFAULT_ONHAND;
        
        node = node->next;
    }
    
    printf("\nStock count reset to default values.\n\n");
    
    return TRUE;
}

/* 
 * Saves the updated stock count to the stock file specified on the 
 * command line. This partially fulfil requirement 7 of the spec.
 * */
BOOLEAN save(struct vm *vm)
{
    /* The UNUSED() function is designed to prevent warnings while your 
     * code is only partially complete. Delete this function call once 
     * you are using vm in your own code 
    UNUSED(vm);
    
    printf("\nsave functionality not implemented yet...\n");*/
    
    int i;
    FILE *item_file, *coins_file;
    struct vm_node *node;
    struct stock_item *item;
    
    if(!vm)
    {
        fprintf(stderr, "Error: Unable to save data.\n");
        return FALSE;
    }
    
    node = vm->item_list->head;
    
    /* Open item file for writing, return FALSE if it fails */
    item_file = fopen(vm->foodfile, "w");
    if(!item_file)
    {
        fprintf(stderr, "Error: %s could not be opened for writing.\n\n",
                vm->foodfile);
        return FALSE;
    }
    
    while(node != NULL)
    {
        item = node->data;
        fprintf(item_file, "%s|%s|%s|%u.%02u|%u\n", item->id, item->name,
                item->desc, item->price.dollars, item->price.cents,
                item->on_hand);
        node = node->next;
    }
    fclose(item_file);
    
    coins_file = fopen(vm->coinsfile, "w");
    if(!coins_file)
    {
        fprintf(stderr, "Error: %s could not be opened for writing.\n\n",
                vm->coinsfile);
        return FALSE;
    }
    
    /* Write coins out to file, in order of $10 ... 5c */
    for(i = NUM_DENOMS; i > 0; i--)
    {
        fprintf(coins_file, "%d,%d\n", denom_to_unsigned(vm->coins[i-1].denom),
                vm->coins[i-1].count);
    }
    fclose(coins_file);
    
    printf("\nExitting program (data will be saved)...\n");
    
    return TRUE;
}

/*
 * Does nothing but print an exiting message, simply so that the abort menu
 * option had a function pointer
 * */
BOOLEAN abort_program(struct vm *vm)
{
    /* vm struct doesn't need to be used in this function */
    UNUSED(vm);
    
    printf("\nExiting program (data will NOT be saved)...\n");
    
    return TRUE;
}

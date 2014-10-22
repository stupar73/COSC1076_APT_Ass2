/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 **********************************************************************/
#include "vm_list.h"

/*
 * Initialises the vm data structure declared in main to safe initial
 * values.
 * */
BOOLEAN vm_init(struct vm *vm)
{
    /* Check input pointer is non-NULL */
    if(!vm)
    {
        return FALSE;
    }
    
    /* Initialise vm to all elements zero or null */
    memset(vm, 0, sizeof *vm);
    
    /* Allocate memory for vm_list struct - return FALSE if it fails */
    vm->item_list = malloc(sizeof *(vm->item_list));
    if(!vm->item_list)
    {
        return FALSE;
    }
    /* Initialise item_list to safe values */
    vm->item_list->head = NULL;
    vm->item_list->length = 0;
    
    init_coins(vm->coins);
    
    return TRUE;
}

/* 
 * Loads data from the .dat files into memory. 
 * */
BOOLEAN load_data(struct vm *vm, const char *item_fname, 
    const char *coins_fname)
{
    FILE *item_file, *coins_file;
    char line[BUFFER_SIZE];
    char *token;
    char *tokens[NUM_FIELDS];
    enum string_result res;
    enum field_type field_type;
    struct stock_item *item;
    char *dollars_str, *cents_str;
    unsigned dollars, cents, on_hand;
    char *denom_str, *count_str;
    unsigned denom, count;
    unsigned denoms_read = 0;
    
    /* Open item file for reading, return FALSE if it fails */
    item_file = fopen(item_fname, "r");
    if(!item_file)
    {
        fprintf(stderr, "Error: %s could not be opened for reading.\n\n",
                item_fname);
        return FALSE;
    }
    
    /* Read each line in the item file and tokenise each field */
    while((res = get_string(line, BUFFER_SIZE, item_file)) != STRING_EMPTY)
    {
        if(res == STRING_TOOLONG)
        {
            return FALSE;
        }
        
        field_type = ID;
        token = strtok(line, ITEM_DELIM);
        while(token != NULL)
        {
            /* Check that the token is valid */
            if(!is_valid_field(token, field_type))
            {
                return FALSE;
            }
            
            tokens[field_type] = token;
            field_type++;
            token = strtok(NULL, ITEM_DELIM);
        }
        
        if(field_type != ON_HAND+1)
        {
            /* Too many or not enough tokens read */
            return FALSE;
        }
        
        /* Convert price token to two unsigned variables */
        dollars_str = strtok(tokens[PRICE], PRICE_DELIM);
        cents_str = strtok(NULL, PRICE_DELIM);
        str_to_unsigned(dollars_str, &dollars);
        str_to_unsigned(cents_str, &cents);
        
        /* Convert on hand token to unsigned variable */
        str_to_unsigned(tokens[ON_HAND], &on_hand);
        
        /* Allocate memory for item, initialise it to safe values, assign to it
         * the values read in from file and add it to the linked list */
        item = malloc(sizeof *item);
        memset(item, 0, sizeof *item);
        create_item(item, tokens[ID], tokens[NAME], tokens[DESC], dollars,
            cents, on_hand);
        add_node(vm->item_list, item);
    }
    fclose(item_file);
    
    coins_file = fopen(coins_fname, "r");
    if(!coins_file)
    {
        fprintf(stderr, "Error: %s could not be opened for reading.\n\n",
                coins_fname);
        return FALSE;
    }
    
    /* Read each line in the coins file and tokenise each field */
    while((res = get_string(line, BUFFER_SIZE, coins_file)) != STRING_EMPTY)
    {
        if(res == STRING_TOOLONG)
        {
            return FALSE;
        }
        
        denom_str = strtok(line, COIN_DELIM);
        count_str = strtok(NULL, COIN_DELIM);
        
        /* Check if there's more of the string still unread */
        if(strtok(NULL, COIN_DELIM) != NULL)
        {
            return FALSE;
        }
        
        /* Attempt to convert each token to an unsigned int */
        if(!str_to_unsigned(denom_str, &denom) ||
           !str_to_unsigned(count_str, &count))
        {
            return FALSE;
        }
        
        /* Attempt to add the coin to the vm */
        if(!add_denom(vm->coins, denom, count))
        {
            return FALSE;
        }
        denoms_read++;
    }
    fclose(coins_file);
    
    if(denoms_read != NUM_DENOMS)
    {
        return FALSE;
    }
    
    /* Assign foodfile and coinsfile members to given file names for saving on
     * exit */
    vm->foodfile = malloc(strlen(item_fname)+1);
    strcpy(vm->foodfile, item_fname);
    vm->coinsfile = malloc(strlen(coins_fname)+1);
    strcpy(vm->coinsfile, coins_fname);
    
    return TRUE;
}

/*
 * Check if the input field is a valid field of type field type
 * */
BOOLEAN is_valid_field(char *field, enum field_type field_type)
{
    switch(field_type)
    {
        case ID:
            if(strlen(field) > ID_LEN || field[0] != 'I')
            {
                return FALSE;
            }
            break;
        case NAME:
            if(strlen(field) > NAME_LEN)
            {
                return FALSE;
            }
            break;
        case DESC:
            if(strlen(field) > DESC_LEN)
            {
                return FALSE;
            }
            break;
        case PRICE:
            if(!is_price_str(field))
            {
                return FALSE;
            }
            break;
        case ON_HAND:
            if(!is_unsigned(field))
            {
                return FALSE;
            }
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

/*
 * Checks if the given string is a valid id
 * */
BOOLEAN is_valid_id(char *s)
{
    if(strlen(s) > ID_LEN)
    {
        return FALSE;
    }
    if(s[0] != 'I')
    {
        return FALSE;
    }
    if(!is_unsigned(s+1))
    {
        return FALSE;
    }
    return TRUE;
}

/*
 * Finds the next id for adding a new item
 * */
void get_next_id(char *next_id, struct vm_list *list)
{
    unsigned highest_id_num = 0;
    unsigned current_id_num;
    struct vm_node *node = list->head;
    
    /* Find the highest id number of existing items */
    while(node != NULL)
    {
        str_to_unsigned((node->data->id)+1, &current_id_num);
        if(current_id_num > highest_id_num)
        {
            highest_id_num = current_id_num;
        }
        node = node->next;
    }
    
    /* Store the next id in next_id string */
    sprintf(next_id, "I%04u", highest_id_num+1);
}

/*
 * Assigns the input values to the input item
 * */
void create_item(struct stock_item *item, char *id, char *name, char *desc,
    unsigned dollars, unsigned cents, unsigned on_hand)
{
    strcpy(item->id, id);
    strcpy(item->name, name);
    strcpy(item->desc, desc);
    item->price.dollars = dollars;
    item->price.cents = cents;
    item->on_hand = on_hand;
}

/*
 * Add node to the vending machine linked list
 * */
BOOLEAN add_node(struct vm_list *list, struct stock_item *item)
{
    struct vm_node *new_node;
    struct vm_node *prev, *curr;
    
    if(list == NULL || item == NULL)
    {
        return FALSE;
    }
    
    /* Start search at the head of the list */
    prev = NULL;
    curr = list->head;
    
    /* Allocate memory for the new node to be inserted */
    new_node = malloc(sizeof *new_node);
    if(new_node == NULL)
    {
        return FALSE;
    }
    memset(new_node, 0, sizeof *new_node);
    
    /* Copy the input item to the new node */
    new_node->data = item;
    
    /* Traverse through list until the appropriate point for insertion has been
     * found. i.e.: loops until the current node is "greater than" (according
     * to strcmp) the new node */
    while(curr != NULL && strcmp(item->name, curr->data->name) > 0)
    {
        prev = curr;
        curr = curr->next;
    }
    
    /* Insert node into list */
    new_node->next = curr;
    if(prev == NULL)
    {
        /* Insertion is at the head of the list */
        list->head = new_node;
    }
    else
    {
        prev->next = new_node;
    }
    
    list->length += 1;
    
    return TRUE;
}

/*
 * Delete node from the vending machine, assign the details of the node deleted
 * to the removed_item pointer for printing. Returns TRUE if it found and
 * deleted, FALSE otherwise
 * */ 
BOOLEAN delete_node(struct vm_list *list, char *item_id,
    struct stock_item *removed_item)
{
    struct vm_node *prev, *curr;
    
    if(list == NULL || item_id == NULL)
    {
        return  FALSE;
    }
    
    prev = NULL;
    curr = list->head;
    
    /* Traverse through list until the item with the given id is found or
     * the end of the list is reached */
    while(curr != NULL && strcmp(item_id, curr->data->id) != 0)
    {
        prev = curr;
        curr = curr->next;
    }
    
    /* Check if item was found */
    if (curr == NULL)
    {
        return FALSE;
    }
    
    /* Copy data from node being removed to the removed_item pointer for
     * printing */
    memcpy(removed_item, curr->data, sizeof *(curr->data));
    
    /* Remove desired node from list and change the link from the node before
     * it to point to the next node in the list */
    if(prev == NULL)
    {
        /* Node to delete is at head of list */
        list->head = curr->next;
    }
    else
    {
        prev->next = curr->next;
    }
    free_node(curr);
    
    return TRUE;
}

/*
 * Traverses through the list searching for the item with id 'item_id'. If
 * found, changes the 'item' pointer to point to the item with that id. If not
 * found, returns FALSE
 * */
BOOLEAN find_item(struct vm_list *list, char *item_id, struct stock_item **item)
{
    struct vm_node *node = list->head;
    
    while(node != NULL && strcmp(item_id, node->data->id) != 0)
    {
        node = node->next;
    }
    
    if(node == NULL)
    {
        /* Item not found */
        return FALSE;
    }
    
    /* Change address of item to point to the data in the matching node */
    *item = node->data;
    
    return TRUE;
}

/* 
 * Frees all dynamically allocated data.
 * */
void system_free(struct vm *vm)
{
    struct vm_node *node, *temp;
    
    if(!vm)
    {
        /* Nothing to free */
        return;
    }
    
    node = vm->item_list->head;
    temp = NULL;
    
    while(node != NULL)
    {
        temp = node;
        node = node->next;
        free_node(temp);
    }
    
    free(vm->item_list);
    vm->item_list = NULL;
    
    free(vm->foodfile);
    free(vm->coinsfile);
    vm->foodfile = NULL;
    vm->coinsfile = NULL;
}

/*
 * Frees an individual node
 * */
void free_node(struct vm_node *node)
{
    free(node->data);
    free(node);
    node = NULL;
}

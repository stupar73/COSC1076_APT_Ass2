/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 ***********************************************************************/

#include "vm_coins.h"

#ifndef VM_LIST
#define VM_LIST

#define ID_LEN               5
#define NAME_LEN            40
#define DESC_LEN           255
#define NUM_FIELDS           5
#define ITEM_DELIM         "|"
#define COIN_DELIM         ","

/* enum to assist in loading of data from provided files */
enum field_type
{
    ID, NAME, DESC, PRICE, ON_HAND
};

/* The data structure that holds the data for each item of stock
 */
struct stock_item
{
    char id[ID_LEN+1];
    char name[NAME_LEN+1];
    char desc[DESC_LEN+1];
    struct price price;
    unsigned on_hand;
};

/* The data structure that holds a pointer to the stock_item data and a
 * pointer to the next node in the list
 */
struct vm_node
{
    struct stock_item *data;
    struct vm_node *next;
};

/* The head of the list - has a pointer to the rest of the list and a 
 * stores the length of the list 
 */
struct vm_list
{
    struct vm_node *head;
    unsigned length;
};

/* This is the head of our overall data structure. We have a pointer to 
 * the vending machine list as well as an array of coins. 
 */
struct vm
{
    struct vm_list *item_list;
    struct coin coins[NUM_DENOMS];
    char *foodfile;
    char *coinsfile;
};

BOOLEAN vm_init(struct vm *vm);
BOOLEAN load_data(struct vm *vm, const char *item_fname, 
    const char *coins_fname);
BOOLEAN is_valid_field(char *field, enum field_type field_type);
BOOLEAN is_valid_id(char *s);
void get_next_id(char *next_id, struct vm_list *list);
void create_item(struct stock_item *item, char *id, char *name, char *desc,
    unsigned dollars, unsigned cents, unsigned on_hand);
BOOLEAN add_node(struct vm_list *list, struct stock_item *item);
BOOLEAN delete_node(struct vm_list *list, char *item_id,
    struct stock_item *removed_item);
BOOLEAN find_item(struct vm_list *list, char *item_id, struct stock_item **item);
void system_free(struct vm *vm);
void free_node(struct vm_node *node);

#endif

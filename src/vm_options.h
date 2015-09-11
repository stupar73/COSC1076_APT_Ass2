/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 ***********************************************************************/

#include "vm_list.h"

#ifndef VM_OPTIONS
#define VM_OPTIONS

#define PRICE_STR_LEN       6
#define DEFAULT_COIN_COUNT 20
#define DEFAULT_ONHAND     20

BOOLEAN display_items(struct vm *vm);
BOOLEAN display_coins(struct vm *vm);
BOOLEAN purchase_item(struct vm *vm);
BOOLEAN give_change(unsigned change_owed, struct coin available[],
    struct coin change[]);
BOOLEAN add_item(struct vm *vm);
BOOLEAN delete_item(struct vm *vm);
BOOLEAN reset_coin_count(struct vm *vm);
BOOLEAN reset_stock_count(struct vm *vm);
BOOLEAN save(struct vm *vm);
BOOLEAN abort_program(struct vm *vm);

#endif

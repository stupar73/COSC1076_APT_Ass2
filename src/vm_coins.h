/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 ***********************************************************************/

#include "vm_utility.h"

#ifndef VM_COINS
#define VM_COINS

#define NUM_DENOMS  8
#define PRICE_DELIM "."

/* Each price will have a dollars and a cents component */
struct price
{
    unsigned dollars,cents;
};

/* The different denominations of coins available */
enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

/* Each coin in the coins array will have a denomination (20 cents, 
 * 50 cents, etc) and a count - how many of that coin do we have on hand
 */
struct coin
{
    enum denomination denom;
    unsigned count;
};

void init_coins(struct coin coins[]);
BOOLEAN is_price_str(char *s);
BOOLEAN str_to_cents(char *cents_str, unsigned *cents);
unsigned denom_to_unsigned(enum denomination denom);
BOOLEAN add_denom(struct coin *coins, unsigned denom, unsigned count);
void add_coin(struct coin *coins, enum denomination denom, unsigned count);
void remove_coins(struct coin vm_coins[], struct coin remove_coins[]);
void print_coins(struct coin coins[]);
void print_coin(struct coin coin);

#endif

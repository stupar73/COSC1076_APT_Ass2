/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 **********************************************************************/
#include "vm_coins.h"

/*
 * Initialises an array of struct coin of size NUM_DENOMS and sets each coins
 * count to 0
 * */
void init_coins(struct coin coins[])
{
    int i;
    
    for(i = 0; i < NUM_DENOMS; ++i)
    {
        /* Value of i corresponds to each denomination
         * i.e.: 0 = FIVE_CENTS ... 7 = TEN_DOLLARS */
        coins[i].denom = i;
        coins[i].count = 0;
    }
}

/*
 * Checks if the string s is a price string, i.e. it contains only one
 * PRICE_DELIM, is a valid double and is divisible by 0.05 (5 cents is lowest
 * multiple for price). Returns TRUE if it is, FALSE otherwise
 * */
BOOLEAN is_price_str(char *s)
{
    char *cents_str;
    unsigned cents;
    
    if((strchr(s, *PRICE_DELIM) == NULL) ||
       (strchr(s, *PRICE_DELIM) != strrchr(s, *PRICE_DELIM)))
    {
        /* There is no PRICE_DELIM in the string, or there is more than one */
        return FALSE;
    }
    if(!is_double(s))
    {
        return FALSE;
    }
    
    
    
    /* Convert cents to unsigned variable */
    cents_str = strchr(s, *PRICE_DELIM)+1;
    str_to_unsigned(cents_str, &cents);
    
    /* Check that there were 2 numbers after the decimal place */
    if(strlen(cents_str) != 2)
    {
        return FALSE;
    }
    
    /* Check the price is divisible into 5 cents */
    if(cents % 5 != 0)
    {
        return FALSE;
    }
    
    return TRUE;
}

/*
 * Converts a price string to an unsigned int representation, returns FALSE
 * if string is not a valid denomination representation
 * */
BOOLEAN str_to_cents(char *cents_str, unsigned *cents)
{
    unsigned cents_u;
    
    if(!str_to_unsigned(cents_str, &cents_u))
    {
        return FALSE;
    }
    
    if(cents_u == 5 || cents_u == 10 || cents_u == 20 || cents_u == 50 ||
       cents_u == 100 || cents_u == 200 || cents_u == 500 || cents_u == 1000)
    {
        *cents = cents_u;
        return TRUE;
    }
    
    return FALSE;
}

/*
 * Takes a denomination and returns it as unsigned cents
 * */
unsigned denom_to_unsigned(enum denomination denom)
{
    switch(denom)
    {
        case FIVE_CENTS:
            return 5;
        case TEN_CENTS:
            return 10;
        case TWENTY_CENTS:
            return 20;
        case FIFTY_CENTS:
            return 50;
        case ONE_DOLLAR:
            return 100;
        case TWO_DOLLARS:
            return 200;
        case FIVE_DOLLARS:
            return 500;
        case TEN_DOLLARS:
            return 1000;
    }
    /* Will never be reached */
    return 0;
}

/*
 * Adds 'count' coins of value 'denom'
 * */
BOOLEAN add_denom(struct coin coins[], unsigned denom, unsigned count)
{
    switch(denom)
    {
        case 5:
            add_coin(coins, FIVE_CENTS, count);
            break;
        case 10:
            add_coin(coins, TEN_CENTS, count);
            break;
        case 20:
            add_coin(coins, TWENTY_CENTS, count);
            break;
        case 50:
            add_coin(coins, FIFTY_CENTS, count);
            break;
        case 100:
            add_coin(coins, ONE_DOLLAR, count);
            break;
        case 200:
            add_coin(coins, TWO_DOLLARS, count);
            break;
        case 500:
            add_coin(coins, FIVE_DOLLARS, count);
            break;
        case 1000:
            add_coin(coins, TEN_DOLLARS, count);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

/*
 * Adds 'count' coins of denomination 'denom'
 * */
void add_coin(struct coin coins[], enum denomination denom, unsigned count)
{
    int i;
    
    for(i = 0; i < NUM_DENOMS; i++)
    {
        if(coins[i].denom == denom)
        {
            coins[i].count += count;
        }
    }
}

/*
 * Removes the coins in 'remove_coins' from 'vm_coins', fails if there are less
 * coins in vm_coins than in remove_coins for any particular denom (will never
 * happen)
 * */
void remove_coins(struct coin vm_coins[], struct coin remove_coins[])
{
    int i;
    
    for(i = 0; i < NUM_DENOMS; i++)
    {
        /* Must be more coins in the vm than are being removed */
        assert(vm_coins[i].count >= remove_coins[i].count);
        
        vm_coins[i].count -= remove_coins[i].count;
    }
}

/*
 * Print each coin from 'coins' coin.count times in descending order 
 * ($10 first ... 5c last)
 * */
void print_coins(struct coin coins[])
{
    int i;
    
    for(i = NUM_DENOMS; i > 0; i--)
    {
        if(coins[i-1].count != 0)
        {
            print_coin(coins[i-1]);
        }
    }
    print_multi('\n', 2);
}

/*
 * Prints 'coin' coin.count times delimited with a space
 * */
void print_coin(struct coin coin)
{
    int i;
    
    switch(coin.denom)
    {
        case FIVE_CENTS:
            for(i = 0; i < coin.count; i++)
            {
                printf("5c ");
            }
            break;
        case TEN_CENTS:
            for(i = 0; i < coin.count; i++)
            {
                printf("10c ");
            }
            break;
        case TWENTY_CENTS:
            for(i = 0; i < coin.count; i++)
            {
                printf("20c ");
            }
            break;
        case FIFTY_CENTS:
            for(i = 0; i < coin.count; i++)
            {
                printf("50c ");
            }
            break;
        case ONE_DOLLAR:
            for(i = 0; i < coin.count; i++)
            {
                printf("$1 ");
            }
            break;
        case TWO_DOLLARS:
            for(i = 0; i < coin.count; i++)
            {
                printf("$2 ");
            }
            break;
        case FIVE_DOLLARS:
            for(i = 0; i < coin.count; i++)
            {
                printf("$5 ");
            }
            break;
        case TEN_DOLLARS:
            for(i = 0; i < coin.count; i++)
            {
                printf("$10 ");
            }
            break;
    }
}

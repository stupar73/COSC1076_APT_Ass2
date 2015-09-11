/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef VM_UTILITY
#define VM_UTILITY

#define UNUSED(var)   (void)var
#define BUFFER_SIZE   1024
#define DECIMAL_BASE  10

/* Type definition for our boolean type */
typedef enum boolean
{
    FALSE, TRUE
} BOOLEAN;

enum string_result
{
    STRING_TOOLONG, STRING_EMPTY, STRING_SUCCESS
};

enum int_result
{
    INT_STRINGTOOBIG, INT_STRINGEMPTY, INT_NOTINT, INT_OUTOFRANGE, INT_SUCCESS
};

void read_rest_of_line(void);
enum string_result get_string(char *s, unsigned size, FILE *stream);
enum int_result get_int(int *n, unsigned size, int min, int max, FILE *stream);
void print_multi(char c, int n);
BOOLEAN is_unsigned(char *s);
BOOLEAN is_double(char *s);
BOOLEAN str_to_unsigned(char* s, unsigned *u);

#endif

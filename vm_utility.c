/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 **********************************************************************/
#include "vm_utility.h"

/* 
 * Clears the input buffer.
 * */
void read_rest_of_line(void)
{
    int ch;
    while(ch = getc(stdin), ch!='\n' && ch != EOF);
    clearerr(stdin);
}

/* Accepts input from stream, removes the trailing newline, stores it in the
 * variable s and returns STRING_SUCCESS if successful. If the input is empty
 * STRING_EMPTY is returned, if the input was greater than the buffer length
 * the remainder of the buffer is cleared and STRING_TOOLONG is returned
 * */
enum string_result get_string(char *s, unsigned size, FILE *stream)
{
    unsigned len;
    
    if(fgets(s, size, stream) == NULL)
    {
        return STRING_EMPTY;
    }
    
    len = strlen(s);
    
    if(len == 1)
    {
        return STRING_EMPTY;
    }
    else if(s[len-1] == '\n')
    {
        s[len-1] = '\0';
        return STRING_SUCCESS;
    }
    else
    {
        read_rest_of_line();
        return STRING_TOOLONG;
    }
}

/* Accepts user input (using get_string defined above) and attempts to convert
 * it to an int within a given range. If get_string returns an error, this
 * error is propagated to the calling function. If the conversion fails
 * INT_NOTINT is returned, if the converted int is out of range INT_OUTOFRANGE
 * is returned, otherwise the converted int is stored in the input variable n
 * and INT_SUCCESS is returned
 * */
enum int_result get_int(int *n, unsigned size, int min, int max, FILE *stream)
{
    char temp_str[BUFFER_SIZE];
    int temp_int = 0;
    char *end_ptr;
    enum string_result input_res;
    
    input_res = get_string(temp_str, size, stream);
    
    if(input_res == STRING_TOOLONG)
    {
        return INT_STRINGTOOBIG;
    }
    else if(input_res == STRING_EMPTY)
    {
        return INT_STRINGEMPTY;
    }
    else /* String successfully read */
    {
        temp_int = (int)strtol(temp_str, &end_ptr, 10);
        
        if(strcmp(end_ptr, "") != 0)
        {
            return INT_NOTINT;
        }
        else if(temp_int < min || temp_int > max)
        {
            return INT_OUTOFRANGE;
        }
        else
        {
            *n = temp_int;
            return INT_SUCCESS;
        }
    }
}

/*
 * Prints character 'c' 'n' times
 * */
void print_multi(char c, int n)
{
    int i;
    for(i = 0; i < n; ++i)
    {
        putchar(c);
    }
}

/* Checks if a given string is an unsigned int. Returns TRUE if it is,
 * FALSE otherwise
 * */
BOOLEAN is_unsigned(char *s)
{
    int i;
    char *end_ptr;
    
    i = (int)strtol(s, &end_ptr, DECIMAL_BASE);
    
    if(i < 0 || strcmp(end_ptr, "") != 0)
    {
        return FALSE;
    }
    return TRUE;
}

/* Checks if a given string is a double. Returns TRUE if it is, FALSE otherwise
 * */
BOOLEAN is_double(char *s)
{
    char *end_ptr;
    
    strtod(s, &end_ptr);
    
    if(strcmp(end_ptr, "") != 0)
    {
        return FALSE;
    }
    return TRUE;
}

/* Attempt to convert a string to an unsigned int. If given string is not a
 * valid unsigned int, will not assign anything and return FALSE. Else it will
 * assign the conversion to input u and return TRUE
 * */
BOOLEAN str_to_unsigned(char *s, unsigned *u)
{
    if(!is_unsigned(s))
    {
        return FALSE;
    }
    
    *u = (unsigned)strtol(s, NULL, DECIMAL_BASE);
    return TRUE;
}

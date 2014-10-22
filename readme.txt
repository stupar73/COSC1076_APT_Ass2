/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP094 - Bachelor of Computer Science
 * Start up code provided by Paul Miller and Virginia King
 **********************************************************************/

vm_utility.c:27 - 'get_string' function borrowed from assignment 1
    implementation

vm_utility.c:61 - 'get_int' function borrowed from assignment 1 implementation

vm_menu.c:59 - 'get_valid_selection' function borrowed from assignment 1
    implementation

General Design choices:
- Where coins are printed/accessed, it is assumed they are in sorted order from
  5c to $10. This is guaranteed since when the coin array is initialised the
  coin.denom is assigned starting from FIVE_CENTS up to TEN_DOLLARS and when
  coins are loaded in, regardless of order in the file, they will follow this
  same ordering
########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2014 Assignment #2
# Full Name        : Stuart Parker
# Student Number   : s3390317
# Course Code      : COSC1076 - Advanced Programming Techniques
# Program Code     : BP094 - Bachelor of Computer Science
# Start up code provided by Paul Miller and Virginia King
########################################################################

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Please add any additional files you add to your 
# project and delete ones your remove. Do not submit your .dat files, 
# your .bin files, containing directories. We only want the files that 
# are part of your implementation
########################################################################

CC     = gcc
CFLAGS = -ansi -Wall -pedantic

OBJ = vm_main.o vm_menu.o vm_options.o vm_list.o vm_coins.o vm_utility.o
EXE = vm
DEP = vm_menu.h vm_options.h vm_list.h vm_coins.h vm_utility.h

vm: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE)

clean:
	rm -f $(OBJ) $(EXE)

archive:
	zip $(USER) vm_main.c vm_menu.h vm_menu.c vm_options.h vm_options.c \
	vm_list.h vm_list.c vm_coins.h vm_coins.c vm_utility.h vm_utility.c \
    Makefile readme.txt

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c $< -o $@
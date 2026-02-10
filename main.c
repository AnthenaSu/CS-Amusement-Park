// CS Amusement Park
// main.c
// Written by <Anthena, Su>, <z5640267>
// on <23/04/2025>
//
// This is the 2nd assingment for COMP1511 
// The task is to design and simulate the operations of an amusement park 
// The purpose of this assingment is to practice lincked list :)


#include <stdio.h>
 #include <stdlib.h> 
#include "cs_amusement_park.h"

int main(void) {
    // Welcome message
    printf("Welcome to CS Amusement Park!\n");
    
    // NOTE: The printing of the ascii park will fail `1511 style`. 
    // You will not be marked on this.
    printf(
        "                                           o\n"
        "                                         o |\n"
        "                                         |\n"
        "      .       .           ._._.    _                     .===.\n"
        "      |`      |`        ..'\\ /`.. |H|        .--.      .:'   `:.\n"
        "     //\\-...-/|\\         |- o -|  |H|`.     /||||\\     ||     ||\n"
        " ._.'//////,'|||`._.    '`./|\\.'` |\\\\||:. .'||||||`.   `:.   .:'\n"
        " ||||||||||||[ ]||||      /_T_\\   |:`:.--'||||||||||`--..`=:='...\n\n"
    );

    printf("Enter the name of the park: ");
    char name[MAX_SIZE];
    scan_name(name);
    struct park *park = initialise_park(name);
    struct schedule *schedule = NULL;

    // Command loop
    command_loop(park, &schedule);

    // End message
    free_park(park, schedule);
    //free(park); 
    printf("\nGoodbye!\n");

    return 0;
}

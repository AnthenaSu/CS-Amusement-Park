// CS Amusement Park
// cs_amusement_park.c
// Written by <Anthena Su>, <z5640267>
// on <23/04/2025>
// This is the 2nd assingment for COMP1511 
// The task is to design and simulate the operations of an amusement park 

////////////////////////////////////////////////////////////////////////////////
// Provided Libraries
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cs_amusement_park.h"

////////////////////////////////////////////////////////////////////////////////
// Your libraries
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////
// Function Definitions
////////////////////////////////////////////////////////////////////////////////

// Stage 1.1
// Function to initialise the park
// Params:
//      name - the name of the park
// Returns: a pointer to the park
struct park *initialise_park(char name[MAX_SIZE]) {
    struct park *new_ride = malloc(sizeof(struct park));
    strcpy(new_ride->name, name);
    new_ride->total_visitors = 0;
    new_ride->rides = NULL;
    new_ride->visitors = NULL;
    return new_ride;
}

// Stage 1.1
// Function to create a visitor
// Params:
//      name - the name of the visitor
//      height - the height of the visitor
// Returns: a pointer to the visitor
struct visitor *create_visitor(char name[MAX_SIZE], double height) {
    struct visitor *new_visitor = malloc(sizeof(struct visitor));
    strcpy(new_visitor->name, name);
    new_visitor->height = height;
    new_visitor->next = NULL;
    
    return new_visitor;
}

// Stage 1.1
// Function to create a ride
// Params:
//      name - the name of the ride
//      type - the type of the ride
// Returns: a pointer to the ride
struct ride *create_ride(char name[MAX_SIZE], enum ride_type type) {
    struct ride *new_ride = malloc(sizeof(struct ride));

    strcpy(new_ride->name, name);
    new_ride->type = type;
    new_ride->queue = NULL;
    new_ride->next = NULL;

    if (type == 0) {
        new_ride->rider_capacity = 4;
        new_ride->queue_capacity = 7;
        new_ride->min_height = 120;
    }
    if (type == 1) {
        new_ride->rider_capacity = 6;
        new_ride->queue_capacity = 9;
        new_ride->min_height = 60;
    }
    if (type == 2) {
        new_ride->rider_capacity = 8;
        new_ride->queue_capacity = 11;
        new_ride->min_height = 75;
    }
    if (type == 3) {
        new_ride->rider_capacity = 10;
        new_ride->queue_capacity = 13; 
        new_ride->min_height = 100;
    }

    return new_ride;
}

// Stage 1.2
// Function to run the command loop
//
// Params:
//     park - a pointer to the park
// Returns: None
void command_loop (struct park *park, struct schedule **schedule) {
    char name[MAX_SIZE];
    char visitor_name[MAX_SIZE];
    char command; 
    char command_append; 
    printf("Enter command: ");
    while (scanf(" %c", &command) == 1) {
        count_down(park, *schedule);

        if (command == '?') {
            print_usage();
        }
        if (command == 'a') {
            scanf(" %c", &command_append);
            append(park, command_append); 
        }
        if (command == 'p') {
            print_welcome_message(park->name);
            print_park(park);
        }
        if (command == 'j') {
            scan_name(name);
            scan_name(visitor_name);
            join_queues(park, name, visitor_name);
        }
        if (command == 'm') {
            scan_name(visitor_name);
            scan_name(name);
            move_visitor(park, visitor_name, name);
        }
        if (command == 'l') {
            scan_name(visitor_name);
            leave(park, visitor_name);
        }
        if (command == 'S') {
            scan_name(name);
            shut_down_ride(park, name);
        }
        if (command == 'q') {
            end_of_day(park, schedule);
            return;
        }
        else {
            // create another command loop 
            // to make original function fewer than 50 lines
            second_command_loop(command, park, schedule);
        }
        printf("Enter command: ");
    }
    end_of_day(park, schedule); 
}

// Stage 1.2
// The above command loop is too long if it stores command 
// This function stores the rest of the command for it 
void second_command_loop (char command, struct park *park, 
                        struct schedule **schedule) {
    char name[MAX_SIZE];
    enum ride_type type; 
    int num; 
    
    if (command == 'd') {
        scan_name(name);
        delete_from_queue(park, name);
    }
    if (command == 'i') {
        scanf(" %d", &num);
        scan_name(name);
        type = scan_type();
        insert_ride(park, num, name, type);
    }
    if (command == 'M') {
        type = scan_type(); 
        merge(park, type);
    }
    if (command == 't') {
        print_total_vis(park);
    }
    if (command == 'c') {
        count(park);
    }
    if (command == 'r') {
        operate_ride(park);
    }
    if (command == 's') {
        scanf(" %d", &num);
        scan_name(name);
        split(park, num, name);
    }
    if (command == 'T') {
        do_schedule(park, schedule);
    }
    if (command == '~') {
        scanf(" %d", &num);
        forward_ticks(park, schedule, num);
    }
}

// Stage 1.3
// Function to add a ride to the park
// Params:
//      park - a pointer to the park
// Returns: None

// This function scan the character after command 'a' 
// r = add ride to the park 
// v = add visitor to the park 
void append (struct park *park, char command_append) {

    if (command_append == 'r') {
        char name[MAX_SIZE]; 
        enum ride_type type;
        scan_name(name);
        type = scan_type();

        add_ride(park, name, type);
    }
    else {
        double height; 
        char visitor_name[MAX_SIZE];
        scan_name(visitor_name);
        scanf(" %lf", &height);

        add_visitor(park, visitor_name, height);
    }
}

// Stage 1.3
// This function add a new ride to the park
// Params:
//      park - a pointer to the park
//      name - a ride name 
//      type - an enum of the ride type
// Returns: None
void add_ride (struct park *park, char name[MAX_SIZE], enum ride_type type) {

    // allocate ride memory bc adding new ride;
    // store name and type
    struct ride *new_ride = malloc(sizeof(struct ride));

    if (check_ride_validity(park, name, type) == 1) {
        strcpy(new_ride->name, name);
        new_ride->type = type;

        // head of park 
        struct park *head = park; 
        // head -> rides go to ride STRUCT 
        // append 

        if (head->rides == NULL) {
            head->rides = create_ride(new_ride->name, new_ride->type);
        } 
        // go to tail
        else {
            // inside ride 
            struct ride *current_ride = head->rides;

            while (current_ride->next != NULL) {
                current_ride = current_ride->next;
            }
            current_ride->next = create_ride(new_ride->name, new_ride->type);
        }

        printf("Ride: '%s' added!\n", new_ride->name);
    }
    free(new_ride);
}

// Stage 1.3
// Function to add a visitor to the park
// Params:
//      park - a pointer to the park
//      name - a name of the visitor
//      height - the visitor's height
// Returns: None
void add_visitor (struct park *park, char name[MAX_SIZE], double height) {
    
    struct visitor *new_visitor = malloc(sizeof(struct visitor));

    // Valid -> store to struct
    if (check_visitor_validity(park, name, height) == 1) {
        strcpy(new_visitor->name, name);
        new_visitor->height = height; 

        //head of the park
        struct park *head = park;
        if (head->visitors == NULL) {
            head->visitors = create_visitor(new_visitor->name, 
                                            new_visitor->height);
            park->total_visitors = park->total_visitors + 1; 
        } 
        // go to tail
        else {
            //inside visitor
            struct visitor *curnt_visitor = head->visitors;
            while (curnt_visitor->next != NULL) {
                curnt_visitor = curnt_visitor->next;
            }
            curnt_visitor->next = create_visitor(new_visitor->name, 
                                                new_visitor->height);
            park->total_visitors = park->total_visitors + 1; 
        }

        printf("Visitor: '%s' has entered the amusement park!\n", 
                new_visitor->name);
    }
    free(new_visitor);
    
}

// Stage 1.4
// Function to print the park
// Params:
//      park - a pointer to the park
// Returns: None
void print_park (struct park *park) {
    
    struct ride *current_ride = park->rides; 
    struct visitor *current_visitor = park->visitors;
    
    if (current_ride == NULL && current_visitor == NULL) {
        printf("The amusement park is empty!\n\n");
        return; 
    }

    printf("Rides:\n");

    if (current_ride == NULL) {
        printf("  No rides!\n");
    } 
    while (current_ride != NULL) {
        print_ride(current_ride);
        current_ride = current_ride->next; 
    }
    
    printf("Visitors:\n");
    if (current_visitor == NULL) {
        printf("  No visitors!\n");
    } 
    while (current_visitor != NULL) {
        print_visitor(current_visitor);
        current_visitor = current_visitor->next; 
    }

    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
// Providing function definitions
////////////////////////////////////////////////////////////////////////////////

// Function to print the usage of the program
// '?' command
// Params: None
// Returns: None
// Usage:
// ```
//      print_usage();
// ```
void print_usage (void) {
    printf(
        "======================[ CS Amusement Park ]======================\n"
        "      ===============[     Usage Info     ]===============       \n"
        "  a r [ride_name] [ride_type]                                    \n"
        "    Add a ride to the park.                                      \n"
        "  a v [visitor_name] [visitor_height]                            \n"
        "    Add a visitor to the park.                                   \n"
        "  i [index] [ride_name] [ride_type]                              \n"
        "    Insert a ride at a specific position in the park's ride list.\n"
        "  j [ride_name] [visitor_name]                                   \n"
        "    Add a visitor to the queue of a specific ride.               \n"
        "  m [visitor_name] [ride_name]                                   \n"
        "    Move a visitor from roaming to a ride's queue.               \n"
        "  d [visitor_name]                                               \n"
        "    Remove a visitor from any ride queue and return to roaming.  \n"
        "  p                                                              \n"
        "    Print the current state of the park, including rides and     \n"
        "    visitors.                                                    \n"
        "  t                                                              \n"
        "    Display the total number of visitors in the park, including  \n"
        "    those roaming and in ride queues.                            \n"
        "  c [start_ride] [end_ride]                                      \n"
        "    Count and display the number of visitors in queues between   \n"
        "    the specified start and end rides, inclusive.                \n"
        "  l [visitor_name]                                               \n"
        "    Remove a visitor entirely from the park.                     \n"
        "  r                                                              \n"
        "    Operate all rides, allowing visitors to enjoy the rides      \n"
        "    and moving them to roaming after their ride.                 \n"
        "  M [ride_type]                                                  \n"
        "    Merge the two smallest rides of the specified type.          \n"
        "  s [n] [ride_name]                                              \n"
        "    Split an existing ride into `n` smaller rides.               \n"
        "  q                                                              \n"
        "    Quit the program and free all allocated resources.           \n"
        "  T [n] [command]                                                \n"
        "    Scheduled the [command] to take place `n` ticks              \n"
        "    in the future.                                               \n"
        "  ~ [n]                                                          \n"
        "    Progress the schedule for `n` ticks.                         \n"
        "  ?                                                              \n"
        "    Show this help information.                                  \n"
        "=================================================================\n");
}

// Function to print a welcome message
// Params:
//      name - the name of the park
// Returns: None
// Usage:
// ```
//      print_welcome_message(name);
// ```
void print_welcome_message(char name[MAX_SIZE]) {
    printf("===================[ %s ]===================\n", name);
}

// Function to print a ride
// Params:
//      ride - the ride to print
// Returns: None
// Usage:
// ```
//      print_ride(ride);
// ```
void print_ride (struct ride *ride) {
    printf("  %s (%s)\n", ride->name, type_to_string(ride->type));
    printf("    Rider Capacity: %d\n", ride->rider_capacity);
    printf("    Queue Capacity: %d\n", ride->queue_capacity);
    printf("    Minimum Height: %.2lfcm\n", ride->min_height);
    printf("    Queue:\n");
    struct visitor *curr_visitor = ride->queue;
    if (curr_visitor == NULL) {
        printf("      No visitors\n");
    } else {
        while (curr_visitor != NULL) {
            printf(
                "      %s (%.2lfcm)\n", curr_visitor->name,
                curr_visitor->height);
            curr_visitor = curr_visitor->next;
        }
    }
}

// Function to print visitor 
// Params:
//      visitor - the visitor to print
// Returns: None
// Usage:
// ```
//      print_visitor(visitor);
// ```
void print_visitor (struct visitor *visitor) {
    struct visitor *current_visitor = visitor;

    if (current_visitor == NULL) {
        printf("  No visitors!\n");
    }
    else {
        printf("  %s (%.2lfcm)\n", 
                current_visitor->name, current_visitor->height);
    }

}

// This function check new RIDE
// Params:
//      park - a pointer to the park
//      name - a ride name 
//      type - an enum of the ride type
// 1) Check if type is valid
// 2) Check if the name of the ride is occupied or not 
int check_ride_validity (
    struct park *park, 
    char name[MAX_SIZE], 
    enum ride_type type) {

    int is_valid = 1;

    //check type 
    if (type == 4) {
        printf("ERROR: Invalid ride type.\n");
        is_valid = 0;
        return is_valid; 
    }

    //point to ride inside park 
    struct ride *current_ride = park->rides; 

    //check name
    while (current_ride != NULL) {
        if (strcmp(current_ride->name, name) == 0) {
            // find the exact same ride name
            printf("ERROR: '%s' already exists.\n", name);
            is_valid = 0;
            return is_valid;
        }
        current_ride = current_ride->next; 
    }

    // All passes the check return 1
    is_valid = 1; 
    return is_valid;

}


// This function check new VISITOR 
// 1) Check if the name of the visitor is occupied or not 
// 2) Check if the height of the visitor is valid or not 
// 3) Check if more than 40 ppl or not
int check_visitor_validity (
    struct park *park, 
    char name[MAX_SIZE], 
    double height) {
    
    int is_valid = 1;

    //check height
    if (height < 50 || height > 250) {
        printf("ERROR: Height must be between 50 and 250.\n");
        is_valid = 0;
        return is_valid; 
    }

    // When adding a visitor
    // if the number of visitors in the park will exceed 40 -> not valid 
    if (park->total_visitors >= 40) {
        printf("ERROR: Cannot add another visitor to the park. ");
        printf("The park is at capacity.\n");
        is_valid = 0;
        return is_valid; 
    }

    //point to ride inside park 
    struct visitor *current_visitor = park->visitors; 

    while (current_visitor != NULL) {
        if (strcmp(current_visitor->name, name) == 0) {
            // find the exact same visitor name
            printf("ERROR: '%s' already exists.\n", name);
            is_valid = 0;
            return is_valid;
        }
        current_visitor = current_visitor->next; 
    }

    // All passes the check return 1
    is_valid = 1; 
    return is_valid;
}

// Stage 2.1 
// Command i: insert new ride to position 
// This function check the validity of new ride
// check the name, type and position 
// if all valid -> insert to the ride struct 
void insert_ride(
    struct park *park, 
    int position, 
    char name[MAX_SIZE], 
    enum ride_type type) {

    if (insert_error(park, name, position, type) == 0) {
        return;
    }
    
    // Finish checking -> insert new ride    
    int count = 1; 
    struct ride *current_ride = park->rides; 
    // Case 1: empty ride
    if (park->rides == NULL) {
        park->rides = create_ride(name, type);
        printf("Ride: '%s' inserted!\n", name);
        return; 
    } 
    //position = 1: insert to head 
    else if (position == 1) {
        current_ride = create_ride(name, type);
        current_ride->next = park->rides;
        park->rides = current_ride; 
    } 
    // insert to after N(position)
    else {
        while (current_ride->next != NULL) {
            if (count == position - 1) {
                struct ride *new_ride = create_ride(name, type);
                new_ride->next = current_ride->next;
                current_ride->next = new_ride;
                printf("Ride: '%s' inserted!\n", name);
                return;
            }
            count ++;
            current_ride = current_ride->next;
        }
        current_ride->next = create_ride(name, type);
    }
    printf("Ride: '%s' inserted!\n", name);
}

// Stage 2.1 
// This function helps check the validity before inerting new ride to park 
// If any insert error, print error message and return 0 
int insert_error (
    struct park *park, 
    char name[MAX_SIZE], 
    int position, 
    enum ride_type type) {

    // check if insert position is valid
    if (position < 1 ) {
        printf("ERROR: n must be at least 1.\n");
        return 0;  
    }
    // check if insert type is valid
    else if (type == 4) {
        printf("ERROR: Invalid ride type.\n");
        return 0;
    }

    // check if name already there or not 
    struct ride *check_vaild_ride = park->rides; 
    while (check_vaild_ride != NULL) {
        if (strcmp(check_vaild_ride->name, name) == 0) {
            printf("ERROR: '%s' already exists.\n", name);
            return 0;
        }
        check_vaild_ride = check_vaild_ride->next; 
    } 

    // no error 
    return 1;
}

// Stage 2.2 
// Command 'j' joins visitors into ride queue 
void join_queues (
    struct park *park, 
    char ride_name[MAX_SIZE], 
    char visitor_name[MAX_SIZE]) {

    // check if the inputs are valid or not.
    // If not valid, return and don't run rest of the function 
    int valid = check_queues(park, ride_name, visitor_name);
    if (valid == 0) {
        return;
    }

    struct ride *current_ride = park->rides;
    
    // Find the ride
    while (current_ride != NULL) {
        if (strcmp(current_ride->name, ride_name) == 0) {

            struct visitor *current_visitor = park->visitors;
            // Find the visitor in park's visitor list
            while (current_visitor != NULL) {
                if (strcmp(current_visitor->name, visitor_name) == 0) {
                    // Create a copy for the ride queue
                    struct visitor *new_visitor = create_visitor(visitor_name, 
                    current_visitor->height);
                    // Add to ride queue (head)
                    if (current_ride->queue == NULL) {
                        current_ride->queue = new_visitor;
                    } 
                    //Add to ride queue (tail)
                    else {
                        struct visitor *tail = current_ride->queue;
                        while (tail->next != NULL) {
                            tail = tail->next;
                        }
                        tail->next = new_visitor;
                    }
                    // Remove the original visitor from park's visitor list
                    free_visitor(park, visitor_name);
                    
                    printf("Visitor: '%s' has entered the queue for '%s'.\n", 
                            visitor_name, ride_name);
                    return;
                }
                current_visitor = current_visitor->next;
            }
        }
        current_ride = current_ride->next;
    }
}

// Stage 2.3
// command 'd' deteles visitors from the queue and move them back to visitors
void delete_from_queue (struct park *park, char name[MAX_SIZE]) {

    // Step 1: Find the ride 
    struct ride *current_ride = park->rides;
    // Step 2: go inside the queue and find visitor
    while (current_ride != NULL) {
        struct visitor *previous = NULL; 
        struct visitor *current_vstr = current_ride->queue; 

        while (current_vstr != NULL) {
            // Found !
            if (strcmp(current_vstr->name, name) == 0) {
                struct visitor *park_visitor = park->visitors;

                // Case 1: no visitor in park -> add to head 
                if (park->visitors == NULL) {
                    park->visitors = create_visitor(name, current_vstr->height);
                } 
                // Case 2: append to tail
                else {
                    while (park_visitor->next != NULL) {
                        park_visitor = park_visitor->next;
                    }
                    park_visitor->next = create_visitor(name, 
                                        current_vstr->height);
                }

                // Remove from ride queue
                if (previous == NULL) {
                    // Deleting head of ride queue
                    current_ride->queue = current_vstr->next;
                } else {
                    previous->next = current_vstr->next;
                }
                free(current_vstr);
                printf("Visitor: '%s' has been removed from their ", name); 
                printf("ride queue and is now roaming the park.\n"); 
                return; 
            }
            previous = current_vstr;
            current_vstr = current_vstr->next;
        }
        current_ride = current_ride->next;
    } 
    printf("ERROR: Visitor '%s' not found in any queue.\n", name);
}


// Stage 2.2
// free the visitor in park->visitor when they (visitors) join ride queue 
void free_visitor (struct park *park, char visitor_name[MAX_SIZE]) {
    if (park == NULL || park->visitors == NULL) {
        return;
    }

    struct visitor *current = park->visitors;
    struct visitor *previous = NULL;

    // Find the visitor to delete
    while (current != NULL && strcmp(current->name, visitor_name) != 0) {
        previous = current;
        current = current->next;
    }

    // Visitor not found
    if (current == NULL) {
        return;
    }

    // Remove from list
    if (previous == NULL) {
        // Deleting the head
        park->visitors = current->next;
    } else {
        previous->next = current->next;
    }

    // Free the visitor
    free(current);
}
        


// Check if visitor can be added to queue or not
// Invalid: no such ride name, no such visitor name
//          height, capacity 
int check_queues (struct park *park, char ride_name[MAX_SIZE], 
    char visitor_name[MAX_SIZE]) {
    struct ride *current_ride = park->rides; 
    int find_ride_name = 0;
    double ride_min_height;
    int max_capacity;
    // find ride name
    while (current_ride != NULL) {
        if (strcmp(current_ride->name, ride_name) == 0) {
            find_ride_name = 1;
            ride_min_height = current_ride->min_height;
            max_capacity = current_ride->queue_capacity;
        }
        current_ride = current_ride->next; 
    }
    if (find_ride_name == 0) {
        printf("ERROR: No ride exists with name '%s'.\n", ride_name);
        return 0;
    } 
    // find visitor name
    int find_visitor_name = 0;
    struct visitor *current_visitor = park->visitors;
    while (current_visitor != NULL) {
        if (strcmp(current_visitor->name, visitor_name) == 0) {
            find_visitor_name = 1; // find visitor -> check height 
            if (current_visitor->height < ride_min_height) {
                printf("ERROR: '%s' is not tall enough to ride '%s'.\n", 
                        visitor_name, ride_name);
                return 0; 
            }
        }
        current_visitor = current_visitor->next; 
    }
    if (find_visitor_name == 0) {
        printf("ERROR: No visitor exists with name '%s'.\n", visitor_name);
        return 0; 
    } 
    int num_queue = 0;
    struct ride *current_ride_2 = park->rides;
    while (current_ride_2 != NULL) {
        if (strcmp(current_ride_2->name, ride_name) == 0) {
            struct visitor *current_queqe = current_ride_2->queue; 
            while (current_queqe != NULL) {
                num_queue++; 
                current_queqe = current_queqe->next; 
            }
        }
        current_ride_2 = current_ride_2->next; 
    }
    return (check_full(num_queue, max_capacity, ride_name, visitor_name));
}

// Check if the visitor can enter the queue or not
// to do this, we check whether the ppl in queue is larger than
// the max capacity of the ride 
int check_full (
    int num_queue, 
    int max_capacity, 
    char ride_name[MAX_SIZE], 
    char visitor_name[MAX_SIZE]) {

    if (num_queue >= max_capacity) {
        printf("ERROR: The queue for '%s' is full. ", ride_name);
        printf("'%s' cannot join the queue.\n", visitor_name);
        return 0; 
    } 
    return 1;

}

// Stage 2.4 
// This function finds the visitor in the park 
// and move them to the queue for ride_name
void move_visitor (
    struct park *park, 
    char visitor_name[MAX_SIZE], 
    char ride_name[MAX_SIZE]) {

    if (check_move(park, visitor_name, ride_name) != 1) {
        return;
    }

    // check roaming visitor
    // go to find_park() 
    // which is another function that move roaming visitors to ride
    if (find_park(park, visitor_name, ride_name) == 1) {
        return;
    }
    // not raoming = lining up
    // start moving from A->B
    struct ride *current_ride = park->rides; 
    while (current_ride != NULL) {
        struct visitor *find_visitor = current_ride->queue;
        struct visitor *previous = NULL;
        double height; 

        while (find_visitor != NULL) {
            if (strcmp(find_visitor->name, visitor_name) == 0) {
                height = find_visitor->height;

                // Find the visitor that we're looking for 
                // go to dd_move_vis() which adds visitor form old to new ride
                add_move_vis(park, ride_name, visitor_name, height); 

                // Remove from A (old) queue queue
                if (previous == NULL) {
                    // Deleting head of ride queue
                    current_ride->queue = find_visitor->next;
                } 
                else {
                    previous->next = find_visitor->next;
                }
                free(find_visitor);
                printf("Visitor: '%s' has been moved to the queue for '%s'.\n", 
                        visitor_name, ride_name);
                return; 
            }
            previous = find_visitor;
            find_visitor = find_visitor->next;
        }
        current_ride = current_ride->next;
    } 
}

// Find roaming visitors in park
// move them to ride
// delete them from park->visitors
int find_park (
    struct park *park, 
    char visitor_name[MAX_SIZE], 
    char ride_name[MAX_SIZE]) {
    struct visitor *prev_visitor = NULL;
    struct visitor *current_visitor = park->visitors;
    double visitor_height = 0;
    int found_in_roaming = 0;
    // Search roaming visitors
    while (current_visitor != NULL) {
        if (strcmp(current_visitor->name, visitor_name) == 0) {
            visitor_height = current_visitor->height;
            found_in_roaming = 1;
            break;
        }
        prev_visitor = current_visitor;
        current_visitor = current_visitor->next;
    }
    if (found_in_roaming) {
        // Remove from roaming list
        if (prev_visitor == NULL) {
            park->visitors = current_visitor->next;
        } else {
            prev_visitor->next = current_visitor->next;
        }
        // Add to ride queue
        struct ride *target_ride = park->rides;
        while (target_ride != NULL) {
            if (strcmp(target_ride->name, ride_name) == 0) {
                struct visitor *new_visitor = create_visitor(visitor_name, 
                                                            visitor_height);
                // Add to end of queue
                if (target_ride->queue == NULL) {
                    target_ride->queue = new_visitor;
                } else {
                    struct visitor *queue_tail = target_ride->queue;
                    while (queue_tail->next != NULL) {
                        queue_tail = queue_tail->next;
                    }
                    queue_tail->next = new_visitor;
                }
                free(current_visitor);
                printf("Visitor: '%s' has been moved to the queue for '%s'.\n", 
                      visitor_name, ride_name);
                return 1;
            }
            target_ride = target_ride->next;
        }
    }
    return 0; 
}

// Stage 2.4
// This function add the visitor form old ride to the new ride
void add_move_vis (
    struct park *park, 
    char ride_name[MAX_SIZE], 
    char visitor_name[MAX_SIZE], 
    double height) {

    struct ride *current_ride = park->rides;

    // find the new ride to insert 
    while (current_ride != NULL) {
        if (strcmp(current_ride->name, ride_name) == 0) {
            // find teh correct ride! 
            // add to the queue
            struct visitor *current_visitor = current_ride->queue;
            // Case1: add to head
            if (current_ride->queue == NULL) {
                current_ride->queue = create_visitor(visitor_name, height);
                return; 
            }
            // Case 2: add to tail
            while (current_visitor->next != NULL) {
                current_visitor = current_visitor->next;
            }
            // current_visitor->next == NULL (add to the tail)
            current_visitor->next = create_visitor(visitor_name, height);
            return; 
        }
        current_ride = current_ride->next;
    }
}

// Stage 2.4
// If one of the following errors occur
// then the visitor should not be moved
// and an error message should be printed instead.
int check_move (struct park *park, char vis[MAX_SIZE], char rid[MAX_SIZE]) {
    struct ride *current_ride = park->rides; 
    double ride_min_height; 
    int max_capacity; 
    int find_ride_name = 0;
    while (current_ride != NULL) {
        if (strcmp(current_ride->name, rid) == 0) {
            find_ride_name = 1;
            ride_min_height = current_ride->min_height;
            max_capacity = current_ride->queue_capacity; 
            struct visitor *find_vis = current_ride->queue;
            while (find_vis != NULL) {
                if (strcmp(find_vis->name, vis) == 0) {
                    printf("ERROR: '%s' is already in the queue for '%s'.\n", 
                            vis, rid);
                    return 0;
                } find_vis = find_vis->next; 
            }
        } current_ride = current_ride->next; 
    } 
    if (find_ride_name == 0) {
        printf("ERROR: No ride exists with name '%s'.\n", rid);
        return 0;
    } 
    //check visitor name
    struct visitor *current_visitor = park->visitors;
    while (current_visitor != NULL) {
        if (strcmp(current_visitor->name, vis) == 0) {
            // find visitor -> check if height tail enough or not
            if (current_visitor->height < ride_min_height) {
                printf("ERROR: '%s' is not tall enough to ride '%s'.\n", 
                        vis, rid);
                return 0; 
            }
        } current_visitor = current_visitor->next; 
    }
    int num_queue = 0;
    struct ride *current_ride_2 = park->rides;
    while (current_ride_2 != NULL) {
        if (strcmp(current_ride_2->name, rid) == 0) {
            struct visitor *current_queqe = current_ride_2->queue; 
            while (current_queqe != NULL) {
                num_queue ++; 
                current_queqe = current_queqe->next; 
            }
        } current_ride_2 = current_ride_2->next; 
    }
    // Check if can join the queue or not. If full -> cannot join (return 0)
    return check_full(num_queue, max_capacity, rid, vis);
    return 1; 
}


// Stage 2.5 
// Command 't' print total visitors
void print_total_vis (struct park *park) {
    
    int total_visitors = 0;
    int roaming_visitors = 0;
    int queued_visitors = 0;

    total_visitors = park->total_visitors; 

    // roaming_visitors = those in the park->visitor
    struct visitor *roaming = park->visitors;
    while (roaming != NULL) {
        roaming_visitors ++; 
        roaming = roaming->next;
    }

    // queued_visitors = those lining up in all rides
    // add every num of visitor lining up in the rides
    struct ride *current = park->rides;
    while (current != NULL) {
        struct visitor *queuing = current->queue;
        while (queuing != NULL) {
            queued_visitors ++; 
            queuing = queuing->next;
        }
        current = current->next;
    }

    printf("Total visitors: %d\n", total_visitors);
    printf("Visitors walking around: %d\n", roaming_visitors);
    printf("Visitors in queues: %d\n", queued_visitors);

}

// Stage 2.5
// Command 'c' — counts visitors between two rides, based on direction
void count (struct park *park) {
    char start[MAX_SIZE];
    char end[MAX_SIZE];
    char direction; 
    scan_name(start);
    scan_name(end);
    scanf(" %c", &direction);

    // First, check if the start and end rides exist 
    if (check_start_end(park, start, end) != 1) {
        return;
    }

    // foward direction 
    if (direction == '>') {
        forward (park, start, end);
        return; 
    }

    // Backward direction 
    struct ride *curr_ride = park->rides;
    int count_mode = 1;
    int num_backward = 0;
    while (curr_ride != NULL && direction == '<') {
        if (strcmp(curr_ride->name, end) == 0) { 
            count_mode = 1;
        }
        if (count_mode == 1) {
            struct visitor *visitor = curr_ride->queue;
            while (visitor != NULL) {
                // Count visitors in current ride queue
                num_backward ++;
                visitor = visitor->next;
            }
        }
        if (strcmp(curr_ride->name, start) == 0) { 
            count_mode = 0;
        } curr_ride = curr_ride->next;
    } 
    printf("Total visitors from '%s' to '%s': %d.\n", 
            end, start, num_backward);
}

// Stage 2.5
// Forward direction with '>'
// This function calculates the total number of visitors in the queue 
// from 'start' ride to 'end' ride in a forward direction
void forward (struct park *park, char start[MAX_SIZE], char end[MAX_SIZE]) {
    int num_visitor = 0;
    int counting = 0;
    int found_start = 0;
    int found_end = 0; 
    // First pass: Look for start to end directly in forward order
    struct ride *current = park->rides;
    while (current != NULL) {
        if (strcmp(current->name, start) == 0) {
            found_start = 1;
            counting = 1;
        }
        if (counting) {
            struct visitor *current_visitor = current->queue;
            while (current_visitor != NULL) {
                num_visitor ++;
                current_visitor = current_visitor->next;
            }
        }
        if (strcmp(current->name, end) == 0) {
            found_end = 1;
            break;  
        } 
        current = current->next;
    }
    if (found_start && found_end) {
        printf("Total visitors from '%s' to '%s': %d.\n", 
                start, end, num_visitor);
        return;
    } 
    // Case when end was not found after start 
    num_visitor = 0;
    counting = 0;
    current = park->rides;
    // Second pass: count from start to end assuming wraparound
    while (current != NULL) {
        if (strcmp(current->name, start) == 0) {
            counting = 1;
        }
        if (counting) {
            struct visitor *current_visitor = current->queue;
            while (current_visitor != NULL) {
                num_visitor ++;
                current_visitor = current_visitor->next;
            }
        } current = current->next;
    } 
    // Use case_two to continue counting 
    num_visitor = case_two (park, num_visitor, end); 
    printf("Total visitors from '%s' to '%s': %d.\n", start, end, num_visitor);
}

// Stage 2.5
// Helper function for 'forward' that continues counting visitors
// Used when a direct sequence from 'start' to 'end' isn't found in first pass
int case_two (struct park *park, int num_visitor, char end[MAX_SIZE]) {
    struct ride *current = park->rides;
    while (current != NULL && strcmp(current->name, end) != 0) {
        struct visitor *current_visitor = current->queue;
        while (current_visitor != NULL) {
            num_visitor ++;
            current_visitor = current_visitor->next;
        } current = current->next;
    } 
    if (current != NULL && strcmp(current->name, end) == 0) {
        struct visitor *current_visitor = current->queue;
        while (current_visitor != NULL) {
            num_visitor ++;
            current_visitor = current_visitor->next;
        }
    } 
    return num_visitor;
}

// Stage 2.5
// check if start_name and end_name exist
// return 1 if both names exist
int check_start_end (
    struct park *park, 
    char start_name[MAX_SIZE], 
    char end_name[MAX_SIZE]) {
    
    int find_start = 0;
    int find_end = 0;

    struct ride *current = park->rides;

    while (current != NULL) {
        if (strcmp(current->name, start_name) == 0) {
            find_start = 1;
        }
        if (strcmp(current->name, end_name) == 0) {
            find_end = 1;
        }
        current = current->next;
    }
    // both are invalid -> print start name
    if (find_start == 0 || find_end == 0) {
        printf("ERROR: One or both rides do not exist ('%s' or '%s').\n", 
                start_name, end_name);
        return 0;
    }
    // both names do exist 
    return 1;
}

// Stage 3.1 
// close (free) all rides and having all visitors leave the park
// free all schedule 
void end_of_day (struct park *park, struct schedule **schedule) {
    if (park == NULL) {
        return;
    }
    
    // free ride->visitors
    struct ride *current_ride = park->rides;
    while (current_ride != NULL) {
        struct visitor *current_visitor = current_ride->queue;
        while (current_visitor != NULL) {
            current_ride->queue = current_visitor->next;
            free(current_visitor);
            current_visitor = current_ride->queue;
        }
        current_ride->queue = NULL;
        park->rides = current_ride->next;
        free(current_ride);
        current_ride = park->rides;
    }

    // free park's roaming visitors 
    struct visitor *roaming_visitor = park->visitors;
    while (roaming_visitor != NULL) {
        park->visitors = roaming_visitor->next;
        free(roaming_visitor);
        roaming_visitor = park->visitors;
    }
    
    // free schedule (the struct I created)
    while (*schedule != NULL) {
        struct schedule *temp = *schedule;
        *schedule = (*schedule)->next;
        free(temp);
    }

    // Double check
    park->visitors = NULL;
    park->rides = NULL;
    park->total_visitors = 0;
    *schedule = NULL;
}

// Being called in main function 
// This function call the end_of_day() which free everything in struct
void free_park (struct park *park, struct schedule *schedule) {
    if (park == NULL) {
        return;
    }
    
    end_of_day(park, &schedule);
    if (schedule != NULL) {
        free(schedule);
    }
    free(park);
}


// Stage 3.2
// find a visitor with a matching name
// delete visitor from the park 
void leave (struct park *park, char visitor_name[MAX_SIZE]) {

    // check park->visitor
    struct visitor *park_visitor = park->visitors;
    struct visitor *park_previous = NULL;
    while (park_visitor != NULL) {
        if (strcmp(park_visitor->name, visitor_name) == 0) {
            if (park_previous == NULL) {
                park->visitors = park_visitor->next;
            }
            else {
                park_previous->next = park_visitor->next;
            }
            free(park_visitor);
            printf("Visitor: '%s' has left the park.\n", visitor_name);
            return; 
        }
        park_previous = park_visitor;
        park_visitor = park_visitor->next;
    }

    // check if visitor exists 
    struct ride *curr_ride = park->rides;
    while (curr_ride != NULL) {
        struct visitor *current_visitor = curr_ride->queue;
        struct visitor *previous = NULL;

        while (current_visitor != NULL) {
            if (strcmp(current_visitor->name, visitor_name) == 0) {
                
                // free head
                if (previous == NULL) {
                    curr_ride->queue = current_visitor->next;
                }
                else {
                    previous->next = current_visitor->next;
                }
                free(current_visitor);
                printf("Visitor: '%s' has left the park.\n", visitor_name);
                return; 
            }
            previous = current_visitor;
            current_visitor = current_visitor->next;
        }
        curr_ride = curr_ride->next;
    }
    printf("ERROR: Visitor '%s' not found in the park.\n", visitor_name);
}

// Stage 3.3
// Operate the rides
void operate_ride (struct park *park) {
    int count = 0;
    char visitor_name[MAX_SIZE];
    // count how many rides
    int num_ride = 0;
    struct ride *count_ride = park->rides;
    while (count_ride != NULL) {
        num_ride ++;
        count_ride = count_ride->next;
    }
    int done = 0;
    while (done < num_ride) {
        int max_capacity;
        struct ride *current = park->rides;
        for (int i = 0; i < num_ride - done -1; i ++) {
            current = current->next; 
        }
        max_capacity = current->rider_capacity;
        struct visitor *curr_vis = current->queue;
        // STEP 1: Add to park 
        while (count < max_capacity && curr_vis != NULL) {
            strcpy(visitor_name, curr_vis->name);
            struct visitor *park_visitor = park->visitors; 
            // add to head
            if (park->visitors == NULL) {
                park->visitors = create_visitor(visitor_name, curr_vis->height);
            } 
            else {  
                while (park_visitor->next != NULL) {
                    park_visitor = park_visitor->next;
                }
                park_visitor->next = create_visitor(visitor_name, 
                                                    curr_vis->height);
            }
            curr_vis = curr_vis->next; 
            count ++; 
        }
        // STEP 2: delete from the original ride queue 
        count = 0;
        struct visitor *delete_vis = current->queue;
        while (count < max_capacity && delete_vis != NULL ) {
            current->queue = delete_vis->next;
            free(delete_vis);
            count ++;
            delete_vis = current->queue;
        }
        // finished deleting Move on to the next ride 
        current = current->next;
        done ++; 
    }
}


// Stage 3.4
// Ride Shut Down
// move visitors to other ride queue
// delete shut-down ride from the park 
void shut_down_ride (struct park *park, char ride_name[MAX_SIZE]) {
    
    int shutdown_type;

    if (check_shutdown_name(park, ride_name) == 0) {
        // no name found 
        return; 
    }

    // get type 
    struct ride *find_type = park->rides;
    while (find_type != NULL) {
        if (strcmp(find_type->name, ride_name) == 0) {
            shutdown_type = find_type->type;

            // move visitors to other rides with same type
            move_same_type(park, find_type, shutdown_type);
            delete_shutdown_ride(park, find_type);
            printf("Ride: '%s' shut down.\n", ride_name); 
            return;
        }
        find_type = find_type->next;
    }
    
}

// This fucntion moves visitors from old ride to the ride with same type 
void move_same_type(struct park *park, struct ride *shutdown, int type) {

    int num_in_shutdown = get_queue_num(shutdown);

    struct ride *current = park->rides;
    while (current != NULL) {
        if (current->type == type && current != shutdown) {
            int current_ride_capacity = get_queue_num(current);
            int max_campacity = current->queue_capacity;

            while (current_ride_capacity <= max_campacity) {
                if (num_in_shutdown ==  0) {
                    return;
                }
                struct visitor *new_visitor = shutdown->queue;
                // find same type and start adding visitors
                struct visitor *vis = current->queue;

                // Case 1: no visitor in it 
                if (current->queue == NULL) {
                    current->queue = create_visitor(new_visitor->name,
                                                 new_visitor->height);
                    current_ride_capacity ++;
                    num_in_shutdown --;
                } 
                // Case 2: add to tail
                else {
                    while (vis->next != NULL) {
                        vis = vis->next;
                    }
                    vis->next = create_visitor(new_visitor->name, 
                                            new_visitor->height);
                    current_ride_capacity ++; 
                    num_in_shutdown --; 
                }
                shutdown->queue = new_visitor->next;
                free(new_visitor);
                new_visitor = shutdown->queue;
            }
        }
        current = current->next;
    }
    // Still not enough capacity to hold all visitors 
    if (get_queue_num(shutdown) != 0) {
        // send them back to park roaming
        printf("ERROR: Not enough capacity to redistribute ");
        printf("all visitors from '%s'.\n", shutdown->name);
        back_to_park(park, shutdown);
    }
}

// This function sends visitors back to park roaming
void back_to_park(struct park *park, struct ride *shutdown) {
    struct visitor *current = shutdown->queue;
    
    while (current != NULL) {
        
        // Case 1: currently no visitors in park
        // add to head
        if (park->visitors == NULL) {
            park->visitors = create_visitor(current->name, current->height);
        }
        //Case 2: add to tail
        else {
            struct visitor *park_visitor = park->visitors;
            while (park_visitor->next != NULL) {
                park_visitor = park_visitor->next;
            }
            park_visitor->next = create_visitor(current->name, current->height);
        }
        current = current->next;

    }

    // delete the visitors in old ride
    current = shutdown->queue;
    while (current != NULL) {
        shutdown->queue = current->next;
        free(current);
        current = shutdown->queue;
    }
}


// The function gets total number of visitors in the queue 
// return total number of visitors
int get_queue_num (struct ride *current_ride) {
    struct visitor *current = current_ride->queue;
    int num = 0;
    while (current != NULL) {
        num ++;
        current = current->next;
    }
    return num;
}


// This function checks if the name exist or not
int check_shutdown_name (struct park *park, char ride_name[MAX_SIZE]) {
    struct ride *current = park->rides;
    while (current != NULL) {
        if (strcmp(current->name, ride_name) == 0) {
            // Find name
            return 1;
        }
        current = current->next;
    }

    // Ran through all linked list and didn't find name 
    printf("ERROR: No ride exists with name: '%s'.\n", ride_name);
    return 0;
}


// This function deletes (free) shutdown ride from the park
void delete_shutdown_ride(struct park *park, struct ride *shutdown) {
    struct ride *current = park->rides;
    struct ride *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->name, shutdown->name) == 0) {
            
            // delete head of the ride
            if (previous == NULL) {
                park->rides = current->next; 
                free(current);
                return;
            }
            previous->next = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Stage 4.1
// Merge two rides with the same type 
void merge(struct park *park, enum ride_type type) {
    int smallest = MAX_SIZE;
    int second_small = MAX_SIZE;
    int num_same_type = 0;

    // find the two smallest rides (that have the type)
    struct ride *current = park->rides;
    struct ride *smallest_ride = NULL;
    struct ride *second_small_ride = NULL;

    while (current != NULL) {
        if (current->type == type) {
            if (get_queue_num(current) < smallest) {
                second_small = smallest;
                smallest = get_queue_num(current);
                
                // store the two struct rides
                second_small_ride = smallest_ride;
                smallest_ride = current; 
            }
            else {
                second_small = get_queue_num(current); 

                // store the struct ride 
                second_small_ride = current;
            }
            num_same_type ++;
        }
        current = current->next;
    }
    if (num_same_type < 2) {
        printf("ERROR: Not enough rides of the specified type to merge.\n");
        return;
    }
    merge_statement(type);

    // when the numenbr of visitors in the two ride queues are the same 
    if (smallest == 0 && second_small == 0) {
        delete_shutdown_ride(park, second_small_ride);
    }
    else if (smallest == second_small) {
        merge_them(second_small_ride, smallest_ride, smallest, second_small);
        delete_shutdown_ride(park, second_small_ride);
    }
    else {
        merge_them(smallest_ride, second_small_ride, smallest, second_small);
        change_rides(second_small_ride, smallest_ride);
        delete_shutdown_ride(park, second_small_ride);
    }
}

// Stage 4.1
// sub function for merge 
// alternately merge the two ride 
// short -> long
void merge_them(
    struct ride *short_ride, 
    struct ride *long_ride, 
    int smallest_num, 
    int second_small_num) {
    
    // twice the Rider Capacity and Queue Capacity
    short_ride->rider_capacity *= 2;
    short_ride->queue_capacity *= 2;
    long_ride->rider_capacity *= 2;
    long_ride->queue_capacity *= 2;

    int index = 0; 

    // 0 long (%2 == 0) longvisitor
    // 1 short current short 
    // 2 long longvisitor->next; 
    struct visitor *current_short = short_ride->queue;
    struct visitor *long_visitor = long_ride->queue;
    
    while (current_short != NULL) {
        
        // add short
        if (index %2 == 0) {
            // create a new node to store information 
            struct visitor *new_visitor = create_visitor(current_short->name, 
                                                        current_short->height);

            // add of the list 
            
            new_visitor->next = long_visitor->next; 
            long_visitor->next = new_visitor;
            
            short_ride->queue = current_short->next;
            free(current_short);
            current_short = short_ride->queue;
        }
        // end of the line
        if (long_visitor == NULL) {
            return;
        }
        long_visitor = long_visitor->next;
        index ++; 
    }  
}

// now it's all in the long ride
// move all visitor from long ride to short ride 
void change_rides (struct ride *long_ride, struct ride *short_ride) {
    
    struct visitor *long_visitor = long_ride->queue;

    while (long_visitor != NULL) {
        struct visitor *short_visitor = short_ride->queue;
        if (short_ride->queue == NULL) {
            short_ride->queue = create_visitor(long_visitor->name, 
                                                long_visitor->height);
        }
        else {
            while (short_visitor->next != NULL) {
                short_visitor = short_visitor->next;
            }
            // end starting adding long visitors
            short_visitor->next = create_visitor(long_visitor->name, 
                                                long_visitor->height); 
        }
        long_ride->queue = long_visitor->next;
        free(long_visitor);
        long_visitor = long_ride->queue;
    }
}

// Stage 4.1
// This function print the merge statment depending on the type 
void merge_statement(int type) {
    if (type == 0) {
        printf("Merged the two smallest rides of type 'ROLLER_COASTER'.\n");
    }
    if (type == 1) {
        printf("Merged the two smallest rides of type 'CAROUSEL'.\n");
    }
    if (type == 2) {
        printf("Merged the two smallest rides of type 'FERRIS_WHEEL'.\n");
    }
    if (type == 3) {
        printf("Merged the two smallest rides of type 'BUMPER_CARS'.\n");
    }
}

// Stage 4.2 
// This function splits an existing ride into multiple smaller rides, 
// evenly distributes visitors from the original ride to the new ones, and 
// removes the original ride from the park
void split(struct park *park, int num, char name[MAX_SIZE]) {

    // num of rides before split
    int initial_count = 0;
    struct ride *count_ride = park->rides;
    while (count_ride != NULL) {
        initial_count ++;
        count_ride = count_ride->next;
    }
    if (check_split(park, name, num) == 0) {
        return;
    }

    // step1: find ride name and get type
    printf("Ride '%s' split into %d new rides.\n", name, num);
    struct ride *previous = NULL; 
    struct ride *find_ride = park->rides;
    while (find_ride != NULL) {
        if (strcmp(find_ride->name, name) == 0) {
            // create new ride names and add new rides in park->rides
            create_names(park, find_ride->name, num, find_ride->type);
            // finished adding new rides names
            // Now, spliting visitors 
            split_here(park, find_ride, initial_count, num);
            
            // delete original ride
            if (previous == NULL) {
                park->rides = find_ride->next; 
            }
            else {
                previous->next = find_ride->next;
            }
            free(find_ride);
            return;
        }
        previous = find_ride;
        find_ride = find_ride->next;
    }
}


void split_here (struct park *park, struct ride *target_ride, 
                int initial_count, int num) {
    // count number of rides right now (after creating new ones)
    int final_count = 0;
    struct ride *count_ride = park->rides;
    while (count_ride != NULL) {
        final_count ++;
        count_ride = count_ride->next;
    }
    double num_visitor = 0;
    struct visitor *initial_visitor = target_ride->queue;
    while (initial_visitor != NULL) {
        num_visitor ++;
        initial_visitor = initial_visitor->next;
    } 
    // add ppl go new rides
    struct ride *current = park->rides;
    for (int i = 0; i < initial_count; i++) {
        current = current->next;
    }
    while (current != NULL) {
        double average = num_visitor / num;
        int i; 
        if ((int) num_visitor % num != 0) {
            i  = (int) average + 1; 
            num_visitor -= (int)(average + 1);  
        }
        else {
            i  = (int) average;
            num_visitor -= average;
        }
        num --; 
        for (int move = 0; move < i; move ++) {
            struct visitor *curr_vistr = current->queue;
            struct visitor *target = target_ride->queue; 
            if (current->queue == NULL) {
                current->queue = create_visitor(target->name, target->height);
            }
            else {
                while (curr_vistr->next != NULL) {
                    curr_vistr = curr_vistr->next;
                }
                curr_vistr->next = create_visitor(target->name, target->height);
            }
            target_ride->queue = target->next;
            free(target);
            target = target_ride->queue;
        } 
        current = current->next;
    }
}

// For loop to execute num times ofthe change_names function 
// Two cases: normal name, long name
void create_names (
    struct park *park, 
    char name[MAX_SIZE], 
    int num, 
    enum ride_type type) {

    for (int i = 1; i < num + 1; i++) {
        change_names(park, name, num, type);
    }
}

// This function generates new names for the split command 's' 
void change_names (struct park *park, char name[MAX_SIZE], 
                    int num, enum ride_type type) {
    char new_name[MAX_SIZE];
    int length = strlen(name); 
    // step 1: create default name, starting from 1
    int count = 1; 
    int succeeded = 0;
    int condition = 1; 
    if (length + 2 < MAX_SIZE) {
        for (int j = 0; j < length; j++) {
            new_name[j] = name[j];
        }
    }
    // too long to accommodate the suffix within the maximum allowed length
    else {
        condition = 2; 
        for (int i = 0; i < length - 2; i++) {
            new_name[i] = name[i];
        }
    }
    // start creating new names 
    while (succeeded == 0) {
        succeeded = 1; 
        struct ride *current = park->rides;
        // check if duplicated or not
        while (current != NULL) {
            if (condition == 1) {
                new_name[length] = '_';
                new_name[length + 1] = '0' + count;  
                new_name[length + 2] = '\0';
            }
            else {
                new_name[length - 2] = '_';
                new_name[length - 1] = '0' + count;  
                new_name[length] = '\0'; 
            }
            if (strcmp(current->name, new_name) == 0) {
                // already existed -> change a num; 
                count ++; 
                succeeded = 0;
            }
            current = current->next; 
        }
    }
    // create ride with new names
    struct ride *head = park->rides;
    while (head->next != NULL) {
        head = head->next;
    }
    head->next = create_ride(new_name, type);
}


// This function checks whether the split name exists and whether the num is > 1 
// If doesn't pass -> error message
int check_split (struct park *park, char name[MAX_SIZE], int num) {
    if (num <= 1) {
        printf("ERROR: Cannot split '%s' into %d rides. ", name, num);
        printf("n must be > 1.\n");
        return 0;
    }
    struct ride *current = park->rides;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            // find name 
            return 1;
        }
        current = current->next;
    }

    // end of the list but did not find name
    printf("ERROR: No ride exists with name: '%s'.\n", name);
    return 0;
}


// Stage 4.3
// Commmand '~ which moves the schedule forward by unit of time
void forward_ticks(struct park *park, struct schedule **schedule, int num) {
    struct schedule *current = *schedule;
    while (current != NULL) {
        // Only execute when the schedule has not been done so 
        // and the input after T is valid (n > 1 ... etc)
        if (current->done == 0 && current->run == 1) {
            current->count_down -= num;
            if (current->count_down <= 0) {
                execute_schedule (park, *schedule, current);
                current->done = 1;
            }
        }
        current = current->next;
    }
}

// Stage 4.3
// Command 'T' schedules a command to run n ticks into the future
void do_schedule (struct park *park, struct schedule **schedule_ptr) {

    int ticks;
    char input_command;
    scanf(" %d", &ticks);
    scanf(" %c", &input_command);
    
    struct schedule *new_node = malloc(sizeof(struct schedule));
    if (!new_node) {
        // Handle allocation failure
        return; 
    }
    
    // Initializing
    new_node->count_down = ticks;
    new_node->command = input_command;
    new_node->append_command = '\0';  
    new_node->num = 0;
    new_node->height = 0.0;
    new_node->type = 0; 
    new_node->name[0] = '\0';
    new_node->done = 0;
    new_node->visitor_name[0] = '\0';
    new_node->next = NULL;

    if (ticks < 1) {
        printf("ERROR: Invalid tick delay: %d. Must be > 0.\n", ticks);
        new_node->run = 0;
    }
    else {
        printf("Command scheduled to run in %d ticks.\n", ticks);
        new_node->run = 1;
    }

    // run store_values() to store all the input after 'T'
    store_values(new_node, input_command);

    // create new node for struct schedule 
    // Case1: head 
    if (*schedule_ptr == NULL) {
        *schedule_ptr = new_node;
    } 
    // Case2: tail 
    else {
        struct schedule *current = *schedule_ptr;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    return;
}

// Stage 4.3
// This function scans and stores the values according to command after 'T' 
// a, s, t, p ... etc
// name = name of the ride
// visitor_name = name of the visitor
void store_values (struct schedule *schedule, char command) {
    char name[MAX_SIZE]; 
    char visitor_name[MAX_SIZE]; 
    char command_append;
    double height;
    struct schedule *current = schedule;
    
    if (command == 'a') {
        scanf(" %c", &command_append);
        scan_name(visitor_name);
        scanf(" %lf", &height);

        current->append_command = command_append;
        strcpy(current->visitor_name, visitor_name);
        current->height = height;
    }
    if (command == 'j') {
        scan_name(name);
        scan_name(visitor_name);
        strcpy(current->name, name);
        strcpy(current->visitor_name, visitor_name);
    }
    if (command == 'm') {
        scan_name(visitor_name);
        scan_name(name);
        strcpy(current->name, name);
        strcpy(current->visitor_name, visitor_name);
    }
    if (command == 'l') {
        scan_name(visitor_name);
        strcpy(current->visitor_name, visitor_name);
    }
    if (command == 'S') {
        scan_name(name);
        strcpy(current->name, name);
    }
    else {
        second_schedule_command(schedule, command);
    }
}

// To avoid the above store_values() function to be more than 50 lines 
// a second function is here and does the same thing as above 
void second_schedule_command (struct schedule *schedule, char command) {
    struct schedule *current = schedule;
    char name[MAX_SIZE];
    enum ride_type type; 
    int num; 
    
    if (command == 'd') {
        scan_name(name);
        strcpy(current->name, name);
    }
    if (command == 'i') {
        scanf(" %d", &num);
        scan_name(name);
        type = scan_type();
        strcpy(current->name, name);
        current->type = type; 
    }
    if (command == 'M') {
        type = scan_type(); 
        current->type = type; 

    }
    if (command == 's') {
        scanf(" %d", &num);
        scan_name(name);
        current->num = num; 
        strcpy(current->name, name);
    }
}

// Stage 4.3
// This function countdown one every time 
// Executes commands when their timer reaches zero
// Handles special cases for invalid commands
void count_down(struct park *park, struct schedule *schedule) {

    struct schedule *current = schedule;
    if (schedule == NULL) {
        return;
    }
    
    while (current != NULL) {
        // command 'q' cannot be executed
        if (current->command == 'q' && current->done == 0) {
            printf("ERROR: Cannot schedule quit command.\n");
            current->done = 1;
        }

        current->count_down -= 1;

        // If ticks = 0 execute the scheduled command 
        if (current->count_down <= 0 && current->done == 0) {
            if (current->run == 1) {
                current->done = 1;
                execute_schedule(park, schedule, current);
            }
        }
        current = current->next;
    }
}

// Stage 4.3
// This function execute the scheduled command by calling corresponded functions
void execute_schedule (struct park *park, struct schedule *schedule, 
                        struct schedule *target) {
    if (target->command == '?') {
        print_usage();
    }
    if (target->command == 'a') {
        if (target->append_command == 'v') {
            add_visitor(park, target->visitor_name, target->height);
        }
        else {
            add_ride(park, target->name, target->type);
        }
    }
    if (target->command == 'p') {
        print_welcome_message(park->name);
        print_park(park);
    }
    if (target->command == 'j') {
        join_queues(park, target->name, target->visitor_name);
    }
    if (target->command == 'm') {
        move_visitor(park, target->visitor_name, target->name);
    }
    if (target->command == 'l') {
        leave(park, target->visitor_name);
    }
    if (target->command == 'S') {
        shut_down_ride(park, target->name);
    }
    if (target->command == 'd') {
        delete_from_queue(park, target->name);
    }
    if (target->command == 'i') {
        insert_ride(park, target->num, target->name, target->type);
    }
    if (target->command == 'M') {
        merge(park, target->type);
    }
    if (target->command == 't') {
        print_total_vis(park);
    }
    if (target->command == 'c') {
        count(park);
    }
    if (target->command == 'r') {
        operate_ride(park);
    }
    if (target->command == 's') {
        split(park, target->num, target->name);
    }
} 

// Check if the inputs for command 'T' are valid or not
// If error return 0. If all passes return 1
int check_schedule (int ticks) {
    if (ticks < 1) {
        printf("ERROR: Invalid tick delay: %d. Must be > 0.\n", ticks);
        return 0;
    }

    // all passes return 1
    return 1;
}



// Scan in the a name string into the provided buffer, placing
// '\0' at the end.
//
// Params:
//      name - a char array of length MAX_SIZE, which will be used
//                  to store the name.
// Returns: None
// Usage:
// ```
//      char name[MAX_SIZE];
//      scan_name(name);
// ```
void scan_name(char name[MAX_SIZE]) {
    scan_token(name, MAX_SIZE);
}

// Scans a string and converts it to a ride_type.
//
// Params: None
// Returns:
//      The corresponding ride_type, if the string was valid,
//      Otherwise, returns INVALID.
//
// Usage:
// ```
//      enum ride_type type = scan_type();
// ```
//
enum ride_type scan_type(void) {
    char type[MAX_SIZE];
    scan_token(type, MAX_SIZE);
    return string_to_type(type);
}

////////////////////////////////////////////////////////////////////////////////
// Additional provided functions
////////////////////////////////////////////////////////////////////////////////

// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

enum ride_type string_to_type(char *type_str) {
    int len = strlen(type_str);

    if (strncasecmp(type_str, "roller_coaster", len) == 0) {
        return ROLLER_COASTER;
    }
    if (strncasecmp(type_str, "CAROUSEL", len) == 0) {
        return CAROUSEL;
    }
    if (strncasecmp(type_str, "FERRIS_WHEEL", len) == 0) {
        return FERRIS_WHEEL;
    }
    if (strncasecmp(type_str, "BUMPER_CARS", len) == 0) {
        return BUMPER_CARS;
    }

    return INVALID;
}

char *type_to_string(enum ride_type type) {
    if (type == ROLLER_COASTER) {
        return "ROLLER_COASTER";
    }
    if (type == CAROUSEL) {
        return "CAROUSEL";
    }
    if (type == FERRIS_WHEEL) {
        return "FERRIS_WHEEL";
    }
    if (type == BUMPER_CARS) {
        return "BUMPER_CARS";
    }
    return "INVALID";
}

int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    scanf(" ");

    while (i < buffer_size - 1 && (num_scanned = scanf("%c", &c)) == 1 &&
           !isspace(c)) {
        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}

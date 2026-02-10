# CS-Amusement-Park
CS Amusement Park is a C command-line simulator that manages a theme park using linked lists: rides are stored in a park list, each ride owns a visitor queue, and roaming visitors live in a separate list. Supports adding/inserting rides, queue ops, operating/merging/splitting/shutdown, plus tick-based scheduled commands. 

## **Overview**
The park maintains:
- A linked list of rides
- A linked list of roaming visitors
- Each ride owns a linked-list queue of visitors
- The simulator enforces real-world constraints (capacity, height limits) and supports both immediate and scheduled actions.
  
## **Core Rules**
- Visitors and rides must have unique names
- Visitors can be either roaming or in one ride queue
- Ride queues enforce minimum height and maximum capacity
- Operating rides boards up to the ride’s rider capacity, returning visitors to roaming

## Command Reference
The simulator is controlled via single-character commands entered at runtime.

| Command | Arguments | Description |
|-------|-----------|-------------|
| `a r` | `ride_name ride_type` | Add a new ride to the park |
| `a v` | `visitor_name visitor_height` | Add a new visitor to the park |
| `i` | `index ride_name ride_type` | Insert a ride at a specific position |
| `j` | `ride_name visitor_name` | Add a visitor to a ride queue |
| `m` | `visitor_name ride_name` | Move a roaming visitor to a ride queue |
| `d` | `visitor_name` | Remove a visitor from a ride queue back to roaming |
| `p` | — | Print the current park state |
| `t` | — | Display total, roaming, and queued visitors |
| `c` | `start_ride end_ride` | Count visitors queued between two rides |
| `l` | `visitor_name` | Remove a visitor from the park |
| `r` | — | Operate all rides and return riders to roaming |
| `M` | `ride_type` | Merge the two smallest rides of a given type |
| `s` | `n ride_name` | Split a ride into `n` smaller rides |
| `q` | — | Quit the program and free all resources |
| `?` | — | Show usage information |

## **Highlighted Functions**
### Command Processing & Control Flow
- command_loop() / second_command_loop()
- execute_schedule()
### Core Data Model
- initialise_park()
- create_ride() / create_visitor()
### Linked List Operations
- Ride management: add_ride(), insert_ride(), delete_shutdown_ride()
- Visitor management: add_visitor(), leave()
### Queue & Visitor State Management
- join_queues() / delete_from_queue()
- move_visitor() / find_park()
### Validation & Constraints
- check_ride_validity() / check_visitor_validity()
- check_queues() / check_move() / check_full()
### Ride Operations
- operate_ride()
- merge() / split()
- shut_down_ride() / move_same_type()
### Scheduling System
- do_schedule() / store_values()
- execute_schedule()
### Memory Management
- end_of_day() / free_park()

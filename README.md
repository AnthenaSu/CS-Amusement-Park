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
| Command | Arguments | Action | Function(s) |
|-------|-----------|--------|-------------|
| `?` | — | Show usage information | `print_usage()` |
| `a r` | `ride_name ride_type` | Add a ride | `add_ride()` |
| `a v` | `visitor_name height` | Add a visitor | `add_visitor()` |
| `i` | `index ride_name ride_type` | Insert ride at position | `insert_ride()` |
| `p` | — | Print park state | `print_park()` |
| `j` | `ride_name visitor_name` | Join ride queue | `join_queues()` |
| `m` | `visitor_name ride_name` | Move visitor | `move_visitor()` |
| `d` | `visitor_name` | Remove from queue | `delete_from_queue()` |
| `l` | `visitor_name` | Visitor leaves park | `leave()` |
| `t` | — | Show visitor counts | `print_total_vis()` |
| `c` | `start end direction` | Count queued visitors | `count()` |
| `r` | — | Operate all rides | `operate_ride()` |
| `S` | `ride_name` | Shut down ride | `shut_down_ride()` |
| `M` | `ride_type` | Merge two smallest rides | `merge()` |
| `s` | `n ride_name` | Split a ride | `split()` |
| `T` | `ticks command` | Schedule a command | `do_schedule()` |
| `~` | `ticks` | Advance scheduled time | `forward_ticks()` |
| `q` | — | Exit and free memory | `end_of_day()` |

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

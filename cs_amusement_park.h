// CS Amusement Park
// cs_amusement_park.h
// Written by <Anthena Su>, <z5640267>
// on <23/04/2025>

////////////////////////////////////////////////////////////////////////////////
// Provided Constants
////////////////////////////////////////////////////////////////////////////////
#define MAX_SIZE 100

////////////////////////////////////////////////////////////////////////////////
// Your constants
////////////////////////////////////////////////////////////////////////////////
#define PRINT_USAGE '?'
#define APPEND 'a'
#define PRINT 'p'
#define JOIN_QUEUES 'j'
#define MOVE_VISITOR 'm'
#define LEAVE 'l'
#define END_OF_DAY 'q'
#define DELETE_FROM_QUEUE 'd'
#define INSERT_RIDE 'i'
#define MERGE 'M'
#define PRINT_TOTAL_VIS 't'
#define COUNT 'c'
#define OPERATE_RIDE 'r'
#define DO_SCHEDULE 'T'
#define FORWARD_TICKS '~'
#define TRUE 1
#define FALSE 0
////////////////////////////////////////////////////////////////////////////////
// Provided Enums
////////////////////////////////////////////////////////////////////////////////

enum ride_type { ROLLER_COASTER, CAROUSEL, FERRIS_WHEEL, BUMPER_CARS, INVALID };

////////////////////////////////////////////////////////////////////////////////
// Provided Structs
////////////////////////////////////////////////////////////////////////////////

struct park {
    // The name of the park
    char name[MAX_SIZE];
    // The total number of visitors in the park
    int total_visitors;
    // The list of rides in the park
    struct ride *rides;
    // The list of visitors in the park
    struct visitor *visitors;
};

struct ride {
    // The name of the ride
    char name[MAX_SIZE];
    // The type of the ride 
    // i.e. ROLLER_COASTER, CAROUSEL, FERRIS_WHEEL, BUMPER_CARS, INVALID
    enum ride_type type;
    // The number of riders the ride can hold
    int rider_capacity;
    // The number of visitors that can be in the queue
    int queue_capacity;
    // The minimum height required to ride
    double min_height;
    // The queue of visitors waiting to ride
    struct visitor *queue;
    // The next ride in the list
    struct ride *next;
};

struct visitor {
    // The name of the visitor
    char name[MAX_SIZE];
    // The height of the visitor
    double height;
    // The next visitor in the list
    struct visitor *next;
};

struct schedule {
    int count_down;
    int num; 
    int done;
    int run;
    double height;
    char command;  
    char append_command;
    char name[MAX_SIZE];
    char visitor_name[MAX_SIZE];
    enum ride_type type;
    struct schedule *next;
};


////////////////////////////////////////////////////////////////////////////////
// Provided function prototypes
////////////////////////////////////////////////////////////////////////////////
void print_usage(void);
void scan_name(char name[MAX_SIZE]);
void print_ride(struct ride *ride);
enum ride_type scan_type(void);
void print_welcome_message(char name[MAX_SIZE]);

// Additional provided function prototypes
// You won't need to use these functions!
// We use just them to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
char *type_to_string(enum ride_type type);
enum ride_type string_to_type(char *type_str);

////////////////////////////////////////////////////////////////////////////////
// Your function prototypes
////////////////////////////////////////////////////////////////////////////////

// Stage 1
struct park *initialise_park(char name[MAX_SIZE]);
struct visitor *create_visitor(char name[MAX_SIZE], double height);
struct ride *create_ride(char name[MAX_SIZE], enum ride_type type);
void command_loop (struct park *park, struct schedule **schedule);
void second_command_loop (char command, struct park *park, struct schedule **schedule);
void append(struct park *park, char command_append);
void add_ride (struct park *park, char name[MAX_SIZE], enum ride_type type);
void add_visitor (struct park *park, char name[MAX_SIZE], double height);
void print_park(struct park *park);
void print_visitor(struct visitor *visitor);
int check_ride_validity(struct park *park, char name[MAX_SIZE], enum ride_type type);
int check_visitor_validity(struct park *park, char name[MAX_SIZE], double height);

// Stage 2
void insert_ride (struct park *park, int position, char name[MAX_SIZE], enum ride_type type);
int insert_error (struct park *park, char name[MAX_SIZE], int position, enum ride_type type);
void join_queues (struct park *park, char ride_name[MAX_SIZE], char visitor_name[MAX_SIZE]);
int check_names_exist(
    struct park *park,
    char ride_name[MAX_SIZE],
    char visitor_name[MAX_SIZE]);
int check_queues(struct park *park, char ride_name[MAX_SIZE], char visitor_name[MAX_SIZE]);
void free_visitor(struct park *park, char visitor_name[MAX_SIZE]);
void delete_from_queue (struct park *park, char name[MAX_SIZE]);
void move_visitor (struct park *park, char visitor_name[MAX_SIZE], char ride_name[MAX_SIZE]);
int check_move (struct park *park, char visitor_name[MAX_SIZE], char ride_name[MAX_SIZE]);
void add_move_vis(struct park *park, char ride_name[MAX_SIZE], char visitor_name[MAX_SIZE], double height);
void print_total_vis(struct park *park);
void count(struct park *park);
int check_start_end(struct park *park, char start_name[MAX_SIZE], char end_name[MAX_SIZE]);
int find_park(struct park *park, char visitor_name[MAX_SIZE], char ride_name[MAX_SIZE]);
void forward (struct park *park, char start[MAX_SIZE], char end[MAX_SIZE]);
int check_full(int num_queue, int max_capacity, char ride_name[MAX_SIZE], char visitor_name[MAX_SIZE]);


// Stage 3
void free_park (struct park *park, struct schedule *schedule);
void end_of_day(struct park *park, struct schedule **schedule);
void leave (struct park *park, char visitor_name[MAX_SIZE]);
void operate_ride(struct park *park);
void shut_down_ride (struct park *park, char ride_name[MAX_SIZE]);
int check_shutdown_name (struct park *park, char ride_name[MAX_SIZE]);
void move_same_type(struct park *park, struct ride *shutdown, int type);
int get_queue_num (struct ride *current_ride);
void back_to_park(struct park *park, struct ride *shutdown);
void delete_shutdown_ride(struct park *park, struct ride *shutdown);

//  Stage 4
void merge(struct park *park, enum ride_type type);
void merge_statement(int type);
void change_rides (struct ride *long_ride, struct ride *short_ride);
void split(struct park *park, int num, char name[MAX_SIZE]);
void merge_them(struct ride *short_ride, struct ride *long_ride, int smallest_num, int second_small_num);
int check_split (struct park *park, char name[MAX_SIZE], int num);
void create_names(struct park *park, char name[MAX_SIZE], int num, enum ride_type type);
void change_names (struct park *park, char name[MAX_SIZE], int num, enum ride_type type);
void split_here(struct park *park, struct ride *target_ride, int initial_count, int num);
void do_schedule (struct park *park, struct schedule **schedule);
int check_schedule (int ticks);
void store_values (struct schedule *schedule, char command);
void second_schedule_command (struct schedule *schedule, char command);
void schedule_initialize();
void count_down(struct park *park, struct schedule *schedule);
void execute_schedule (struct park *park, struct schedule *schedule, struct schedule *target);
void forward_ticks(struct park *park, struct schedule **schedule, int num);
int case_two (struct park *park, int num_vst, char end[MAX_SIZE]);
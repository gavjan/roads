/* header file containing constants */
#ifndef DROGI_DEFINE_H
#define DROGI_DEFINE_H
#define EQUAL 0
#define STARTING_SIZE 1
#define MULTIPLIER 3/2+1
#define MISSING -1
#define NOMEM -2
#define MAX_ROUTES 1000
#define MIN_ROUTE 1
#define UNDEF -1
#define MAX_ROUTEID_DIGITS 3
#define MAX_LENGTH_DIGITS 10
#define MAX_YEAR_DIGITS_WITH_MINUS 11
#define MIN_POSSIBLE_ROUTE 5
#define ROUTEID_AND_FIRST_CITY 2
#define ROUTE_REPEAT_NUM 3
#define FIRST_LETTER_ADDROAD 'a'
#define FIRST_LETTER_REPAIR_OR_REMOVE_ROAD_OR_ROUTE 'r'
#define FIRST_LETTER_GETROUTEDESCRIPTION 'g'
#define FIRST_LETTER_NEWROUTE 'n'
#define FIRST_LETTER_EXTENDROUTE 'e'
#define for_each_neighbor(item, arr) \
    for(struct list* item = arr.head; item != NULL; item = item->next)
#define for_each_node(p) \
    for(; p; p=p->next)
#endif //DROGI_DEFINE_H

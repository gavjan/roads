/** @file
 * Header file that stores the national road map
 *
 * @author Gevorg Chobanyan
 * @date 29.04.2019
 */

#ifndef DROGI_MAP_H
#define DROGI_MAP_H
#define MAX_ROUTES 1000
#include "hash_table.h"
#include <stdbool.h>
/**
 * @brief Structure storing route from given city.
 */
struct routeList {
    struct routeList* next; ///< Next pointer
    struct list* edge; ///< edge pointer
};
/**
 * @brief Structure storing first city in a route.
 */
struct routeArr {
    const char* startCity; ///< Start City
    struct routeList** routeList; ///< routeList double pointer
};
/**
 * @brief Structure storing the map of national roads.
 */
struct Map {
    struct arr* arr; ///< Array of linked lists
    int size; ///< Allocated size for the array
    int count; ///< Used up length of the array
    struct routeArr routes[MAX_ROUTES];	///< Array of linked lists for storing routes
	ht_hash_table* ht; ///< Hast Table Pointer
}; typedef struct Map Map;
/**
 * @brief Structure storing head of neighbors of a vertex.
 */
struct belongNode {
    int id; ///< routeId
    struct belongNode* next; ///< Next pointer
};
struct arr {
    const char* city;
	struct list* head;
	//dijkstra
	unsigned long long dist;
	int prev;
	int oldest;
    bool exists;
    bool multiple;
    struct list* routeEdge;
    bool banned;
};
/**
 * @brief Structure storing neighbors of a vertix.
 */
struct list {
    int from; ///< Index of the city that start is in
    int isIn; ///< Index the end vertex <=> searchCity(m,city);
    unsigned length; ///< Length of the road
    int builtYear; ///< builtYear
    const char* city; ///< city
    struct belongNode** belongs; ///< belongsNode double pointer
    struct list* next; ///< Next pointer
};

/**
 * @brief Get new Map
 * @return Map pointer upon successful memory allocation
 */
Map* newMap(void);

/**
 * @brief deleteMap() according to the specifications of the project
 * @param *m Map pointer
 */
void deleteMap(Map *map);

/**
 * @brief addRoad() according to the specifications of the project
 * @param *m Map pointer
 * @param *city1 City1
 * @param *city2 City2
 * @param length Road length
 * @param builtYear road builtYear
 * @return Whether or not function was executed affirmatively or negatively
 */
bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear);

/**
 * @brief repairRoad() according to the specifications of the project
 * @param *map Map pointer
 * @param *city1 City1
 * @param *city2 City2
 * @param repairYear road builtYear
 * @return Whether or not function was executed affirmatively or negatively
 */
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear);

/**
 * @brief newRoute() according to the specifications of the project
 * @param *map Map pointer
 * @param *routeId routeId
 * @param *city1 City1
 * @param *city2 City2
 * @return Whether or not function was executed affirmatively or negatively
 */
bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2);

/**
 * @brief extendRoad() according to the specifications of the project
 * @param *m Map pointer
 * @param routeId routeId
 * @param *city City
 * @return Whether or not function was executed affirmatively or negatively
 */
bool extendRoute(Map *map, unsigned routeId, const char *city);
/**
 * @brief removeRoad() according to the specifications of the project
 * @param *m Map pointer
 * @param *city1 City1
 * @param *city2 City2
 * @return Whether or not function was executed affirmatively or negatively
 */
bool removeRoad(Map *map, const char *city1, const char *city2);

/**
 * @brief getRouteDescription() according to the specifications of the project
 * @param *m Map pointer
 * @param routeId
 * @return String of result NULL upon getting failed execution
 */
char const* getRouteDescription(Map *map, unsigned routeId);
/**
 * @brief repairRoute() according to the specifications of the project
 * @param *m Map pointer
 * @param routeId routeId
 * @return Whether or not function was executed affirmatively or negatively
 */
bool removeRoute(Map *map, unsigned routeId);
/**
 * @brief Free route list
 * @param *head route list
 */
void freeRouteList(struct routeList* head);
/**
 * @brief Free belong list
 * @param *head head routeList
 */
void freeBelongList(struct belongNode* head);
/**
 * @brief Free list
 * @param *head list pointer
 */
void freeList(struct list* head);

/**
 * @brief connect two existing cities with a road
 * @param *m Map pointer
 * @param C1 City1 int
 * @param C2 City2 int
 * @param length Road length
 * @param builtYear road builtYear
 * @return Whether or not it was possible to connect those two cities
 */
bool connect(struct Map* m, int C1, int C2, unsigned length, int builtYear);
/**
 * @brief Try to prepend into belongs linked list
 * @param data Data to prepend
 * @param **head belongNode double pointer
 * @return Whether or not function was executed affirmatively or negatively
 */
bool prependBelongs(int data, struct belongNode **head);
/**
 * @brief Try to append node to route linked list
 * @param **head head routeList
 * @param *edge appending list pointer
 * @param *lastP last node pointer
 * @return lastP last node pointer
 */
struct routeList* appendRoute(struct routeList **head, struct list* postfix, struct routeList* lastP);
/**
 * @brief Search and return index of a city in arr
 * @param *m Map pointer
 * @param *city City
 * @return Index in arr if exists MISSING otherwise
 */
int searchCity(struct Map* m, const char* city);
/**
 * @brief Check if the city name is correct
 * @param *city String of city name
 * @return Whether the city name is correct or not
 */
bool correctName(const char *city);
/**
 * @brief Check if two citiec have a connecting road
 * @param *m Map pointer
 * @param *city1 City1
 * @param *city2 City2
 * @return Whether or not they have a connecting road
 */
bool haveRoad(struct Map* m, const char* city1, const char* city2);
/**
 * @brief Try to prepend route linked list
 * @param **head head routeList
 * @param *edge appending list pointer
 * @return Whether or not function was executed affirmatively or negatively
 */
bool prependRoute(struct routeList **head, struct list* edge);
/**
 * @brief Get length between u and X
 * @param u useless field for start city, it's here to make the function calls readable
 * @param *X list pointer of end city
 * @return length between cities
 */
unsigned length(int u, struct list* X);
/**
 * @brief Check if routeId is correct
 * @param routeId routeId
 * @return Whether or not routeId is correct
 */
bool correctRouteId(unsigned routeId);
/**
 * @brief Add city to map
 * @param *m Map pointer
 * @param *city City name
 * @return position of taken city
 */
int addCity(struct Map* m, const char* city);
/**
 * @brief Get Adjacent node for C1 and C2
 * @param *m Map pointer
 * @param C1 City1 int
 * @param C2 City2 int
 * @return adjacent node if exists
 */
struct list *getAdjNode(Map* m, int C1, int C2);
/**
 * @brief Extend arr if needed
 * @param *arr arr pointer
 * @param *size size of arr
 * @param *failed whether or not failed to extend
 * @return Pointer for extended arr
 */
struct arr* extend(struct arr* arr, int* size, bool* failed);

/**
 * @brief Get new list
 * @return list pointer upon successful memory allocation
 */
struct list* newList();

#endif //DROGI_MAP_H

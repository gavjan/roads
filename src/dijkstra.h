/** @file
 * Header file for dijkstra
 *
 * @author Gevorg Chobanyan
 * @date 29.04.2019
 */

#include "map.h"

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

/**
 * @brief Dijkstra algorithm
 * @param *m Map pointer
 * @param start Path start
 * @param *ban1 first edge ban
 * @param *ban2 second edge ban
 * @return Whether or not able to allocate space
 */
bool dijkstra(Map* m, int start, struct list* ban1, struct list* ban2);

#endif //DROGI_DIJKSTRA_H

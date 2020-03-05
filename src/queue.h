/** @file
 * Header file for priority queue
 *
 * @author Gevorg Chobanyan
 * @date 29.04.2019
 */
 #ifndef DROGI_QUEUE_H
#define DROGI_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
/**
 * @brief structure for Qnode
 * */
typedef struct Qnode {
	int data; ///< Data of a Qnode
	unsigned long long priority; ///< Priority of a Qnode
	struct Qnode* next; ///< Pointer to the next Qnode

} QNode;
/**
 * @brief Function to Create A New QNode
 * @param d data
 * @param p priority
 * @return QNode Pointer on successful memory allocation
 * */
QNode *newQNode(int d, unsigned long long p);
/**
 * @brief Return the value at head
 * @param **head QNode double pointer
 * @return data
 * */
int peek(QNode **head);
/**
 * @brief Removes the element with the
 * @param **head QNode double pointer
 * */
/**
 * @brief highest priority form the list
 * @param **head QNode double pointer
 * @return data
 * */
int pop(QNode **head);
/**
 * @brief Function to push according to priority
 * @param **head QNode double pointer
 * @param d data
 * @param p priority
 * */
void push(QNode **head, int d, unsigned long long p);
/**
 * @brief Function to check is list is empty
 * @param **head QNode double pointer
 * @return NULL if empty
 * */
int isEmpty(QNode **head);
/**
 * @brief Function to decrease a priority
 * @param **head QNode double pointer
 * @param d data
 * @param p priority
 * */
void decrease(QNode** head, int d, unsigned long long p);
#endif //DROGI_QUEUE_H
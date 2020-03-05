/** @file
 * Header file for Hash Table
 *
 * @author Gevorg Chobanyan
 * @date 29.04.2019
 */
#ifndef DROGI_HASH_TABLE_H
#define DROGI_HASH_TABLE_H


/**
 * @brief Structure for Hash Table item
 */
typedef struct {
  char const* key; ///< key
  int value; ///< value
} ht_item;
/**
 * @brief Structure for Hash Table
 */
typedef struct {
  int size; ///< size
  int count; ///< count
  ht_item** items; ///< Array of ht_item
} ht_hash_table;
/**
 * @brief Make a new Hash Table
 * @return newly created Hash Table upon successful memory allocation
 */
ht_hash_table* ht_new();
/**
 * @brief Delete Hash Table
 * @param *ht Hash Table pointer
 */
void ht_del_hash_table(ht_hash_table* ht);
/**
 * @brief Insert into Hash Table
 * @param *ht Hash Table pointer
 * @param *key key
 * @param *value value
 */
void ht_insert(ht_hash_table* ht, const char* key, int value);
/**
 * @brief Search in Hash Table
 * @param *ht Hash Table Pointer
 * @param *key key
 * @return Search result
 */
int ht_search(ht_hash_table* ht, const char* key);
#endif //DROGI_HASH_TABLE_H
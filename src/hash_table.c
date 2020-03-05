#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#define MISSING -1
#define HT_SIZE 25000

int HT_PRIME_1 = 163;
static ht_item HT_DELETED_ITEM = {NULL, MISSING};
int HT_PRIME_2 = 199;
/**
 * @brief Custom pow() function
 * @param x first int
 * @param y second int
 * @return x^y
 */
long power(int x, int y) {
   if (y < 0) {
	  switch (x) {
		 case 0:
			return 0;
		 case 1:
			return 1;
		 case -1:
			return y % 2 ? -1 : 1;
		 default:
			return 0;
	  }
   }
   long z = 1;
   long base = x;
   while (y) {
	  if (y%2) {
		 z *= base;
	  }
	  y /= 2;
	  base *= base;
   }
   return z;
}
/**
 * @brief Get new Hash Table item
 * @param *k key
 * @param *v value
 * @return newly created Hash Table item upon successful memory allocation
 */
static ht_item* ht_new_item(const char* k, int v) {
  ht_item* i = (ht_item*)malloc(sizeof(ht_item));
  char* new = (char*)malloc(sizeof(char)*(strlen(k)+1));
  strcpy(new, k);
  i->key = (const char*)new;
  i->value = v;
  return i;
}
ht_hash_table* ht_new() {
  ht_hash_table* ht;
  ht = (ht_hash_table*)malloc(sizeof(ht_hash_table));
  if(!ht) {return NULL;}
  ht->size = HT_SIZE;
  ht->count = 0;
  ht->items = (ht_item**)calloc((size_t)ht->size, sizeof(ht_item*));
  if(!ht->items) {return NULL;}
  return ht;
}
/**
 * @brief Delte Hash Table item
 * @param *i index
 */
static void ht_del_item(ht_item* i) {
  free((void*)i->key);
  free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
  if(!ht) {return;}
  int i;
  for (i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL) {
      ht_del_item(item);
    }
  }
  free (ht->items);
  free (ht);
}
/**
 * @brief Hash
 * @param *s String
 * @param *a Number of buckets
 * @param *m attempt
 * @return Hash code
 */
static int ht_hash(const char* s, const int a, const int m) {
  long hash = 0;
  int len_s = strlen(s);
  int i;
  for (i = 0; i < len_s; i++) {
    hash += power(a, len_s - (i+1)) * s[i];
    hash = hash % m;

  }
  return (int)(hash>0 ? hash:-hash) ;
}
/**
 * @brief Get Hash
 * @param *s String
 * @param *a Number of buckets
 * @param *m attempt
 * @return Hash code
 */
static int ht_get_hash(const char*s, const int num_buckets, const int attempt) {
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(ht_hash_table* ht, const char* key, int value) {
  ht_item* item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item* cur_item = ht->items[index];
  int i = 1;
  while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
    if (strcmp(cur_item->key, key) == 0) {
      ht_del_item(cur_item);
      ht->items[index] = item;
      return;
    }
    index = ht_get_hash(item->key, ht->size, i);
    cur_item = ht->items[index];
    i++;
  }
  ht->items[index] = item;
  ht->count++;
}

int ht_search(ht_hash_table* ht, const char* key) {
  int index = ht_get_hash(key, ht->size, 0);
  ht_item* item = ht->items[index];
  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        return item ->value;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  return MISSING;
}
/**
 * @brief Delete Hash
 * @param *ht Hash Table pointer
 * @param *k key
 */
void ht_delete(ht_hash_table* ht, const char* key) {
  int index = ht_get_hash(key, ht->size, 0);
  ht_item* item = ht->items[index];
  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        ht_del_item(item);
        ht->items[index] = &HT_DELETED_ITEM;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  ht->count--;
}

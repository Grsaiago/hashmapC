#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 1) make the insert and remove table pointer into pointer pointer,
 * so that when the table is modified by realloc, it modifies the
 * main pointer.
 *
 * 2) check for size on insert and in delete in order to
 * realloc everything in case of threshhold for reduction or increase.
 *
 * 3) create the linked list api and implement:
 * 3.1) each bucket as a node
 * 3.2) in case of colision, follow the linked list
 * doing a strcmp in order to find the correct node, if it exists.
 */

#include "Hashmap.h"

struct s_bucket *bucket_find_by_key(struct s_bucket *start, char *key) {
	struct s_bucket *itr;

	itr = start;
	while (itr) {
		if (strcmp(key, itr->key) == 0) {
			return itr;
		}
		itr = itr->next;
	}
	return (NULL);
}

void *bucket_insert(struct s_bucket **start, struct s_bucket *new_bucket) {
	struct s_bucket *found;

	if (*start == NULL) {
		*start = new_bucket;
	}
	found = bucket_find_by_key(*start, new_bucket->key);
	if (!found) {
	}
}

struct s_hash_table *malloc_table(unsigned int size,
								  void (*free_func)(void *)) {
	struct s_hash_table *table;

	if (size == 0)
		return (NULL);
	table = malloc(sizeof(struct s_hash_table));
	if (!table)
		return (NULL);
	*table =
		(t_hash_table){.size = 0,
					   .capacity = size,
					   .free_func = free_func,
					   .entries = calloc(size + 1, sizeof(struct s_bucket))};
	if (table->entries == NULL) {
		free(table);
		return (NULL);
	}
	return (table);
}

void free_table(struct s_hash_table *table) {
	unsigned int index;

	index = 0;
	while (index < table->capacity) {
		table->free_func(table->entries[index].value);
		free(table->entries[index].key);
		index++;
	}
	free(table->entries);
	free(table);
	return;
}

static unsigned int hash_function(char *key) {
	unsigned int i;
	unsigned int keyHash;

	if (!key)
		return (0);
	i = 0;
	keyHash = 0;
	while (key[i]) {
		keyHash += key[i] * i;
		i++;
	}
	return (keyHash);
}

void *table_insert(struct s_hash_table **table_ptr, char *key, void *value) {
	unsigned int		 index;
	unsigned int		 key_hash;
	struct s_hash_table *table;
	struct s_bucket		*entry_position;

	if (!key || strcmp("", key) == 0)
		return (NULL);
	table = *table_ptr;
	if (table_access(table, key))
		table_remove(table_ptr, key);
	key_hash = hash_function(key);
	index = key_hash % table->capacity;
	entry_position = &table->entries[index];
	entry_position->key = strdup(key);
	if (!entry_position->key) {
		free_table(table);
		return (NULL);
	}
	entry_position->value = value;
	table->size++;
	printf("table size no insert %d\n", table->size);
	return (value);
}

int table_remove(struct s_hash_table **table_ptr, char *key) {
	unsigned int		 index;
	unsigned int		 key_hash;
	struct s_hash_table *table;
	struct s_bucket		*entry_position;

	if (!key)
		return (0);
	table = *table_ptr;
	key_hash = hash_function(key);
	index = key_hash % table->capacity;
	entry_position = &table->entries[index];
	if (entry_position->key == NULL)
		return (0);
	if (strcmp(key, entry_position->key) == 0) {
		free(entry_position->key);
		table->free_func(entry_position->value);
	}
	// else colisão
	bzero(entry_position, sizeof(struct s_bucket));
	table->size--;
	printf("tableSize no delete %d\n", table->size);
	return (0);
}

void *table_access(struct s_hash_table *table, char *key) {
	unsigned int	 index;
	unsigned int	 key_hash;
	struct s_bucket *entry_position;

	if (!key)
		return (NULL);
	key_hash = hash_function(key);
	index = key_hash % table->capacity;
	entry_position = &table->entries[index];
	if (entry_position->key && strcmp(key, entry_position->key) == 0)
		return (entry_position->value);
	else
		return (NULL);
}

int main(void) {
	struct s_hash_table *table = malloc_table(10, free);
	char				*access;

	if (!table)
		return (0);
	table_insert(&table, "prop0", strdup("string 0"));
	printf("Access na prop0: %s\n", (char *)table_access(table, "prop0"));
	table_insert(&table, "prop0", strdup("Agora sobreescreveu"));
	table_remove(&table, "prop1");
	table_remove(&table, "prop2");
	table_remove(&table, "prop3");
	printf("Access na prop0: %s\n", (char *)table_access(table, "prop0"));
	printf("Access na prop1: %s\n", (char *)table_access(table, "prop1"));
	printf("Access na prop2: %s\n", (char *)table_access(table, "prop2"));
	free_table(table);
	return (0);
}

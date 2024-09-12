#ifndef HASHMAP_H
#define HASHMAP_H

/* Struct definitions */
typedef struct s_bucket {
	char			*key;
	void			*value;
	struct s_bucket *next;
} t_bucket;

typedef struct s_hash_table {
	unsigned int size;
	unsigned int capacity;
	void (*free_func)(void *);
	struct s_bucket *entries;
} t_hash_table;

/* bucket manipulation */

struct s_bucket *bucket_find_by_key(struct s_bucket *start, char *key);
void *bucket_insert(struct s_bucket **start, struct s_bucket *new_bucket);

/* table manipulation */
struct s_hash_table *malloc_table(unsigned int size, void (*free_func)(void *));
void				 free_table(struct s_hash_table *table);
void *table_insert(struct s_hash_table **table_ptr, char *key, void *value);
void *table_access(struct s_hash_table *table, char *key);
int	  table_remove(struct s_hash_table **table_ptr, char *key);
static unsigned int hash_function(char *key);

#endif // !HASHMAP_H

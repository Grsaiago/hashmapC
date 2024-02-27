#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
* make the insert and remove table pointer into pointer pointer,
* so that when the table is modified by realloc, it modifies the
* main pointer.
*/

typedef struct s_table_entry
{
  char          *key;
  void          *value;
} t_table_entry;

typedef struct s_hash_table
{
  unsigned int	      size;
  unsigned int	      capacity;
  void		      (*free_func)(void *);
  struct s_table_entry *entries;
} t_hash_table;

struct s_hash_table    *malloc_table(unsigned int size, void (*free_func)(void *));
void		      free_table(struct s_hash_table *table);
void		      *table_insert(struct s_hash_table *table, char *key, void *value);
void		      *table_access(struct s_hash_table *table, char *key);
int		      table_remove(struct s_hash_table *table, char *key);
static unsigned int   hash_function(char *key);

struct s_hash_table    *malloc_table(unsigned int size, void (*free_func)(void *))
{
  struct s_hash_table  *table;

  if (size == 0)
    return (NULL);
  table = malloc(sizeof(struct s_hash_table));
  if (!table)
    return (NULL);
  *table = (t_hash_table) {
    .size = 0,
    .capacity = size, 
    .free_func = free_func,
    .entries = calloc(size + 1, sizeof(struct s_table_entry))
  };
  if (table->entries== NULL) {
    free(table);
    return (NULL);
  }
  return (table);
}

void  free_table(struct s_hash_table *table)
{
   unsigned int	index;

  index = 0;
  while (index < table->capacity)
  {
    table->free_func(table->entries[index].value);
    free(table->entries[index].key);
    index++;
  }
  free(table->entries);
  free(table);
  return ;
}

static unsigned int  hash_function(char *key)
{
  unsigned int  i;
  unsigned int keyHash;

  if (!key)
    return (0);
  i = 0;
  keyHash = 0;
  while (key[i])
  {
    keyHash += key[i] * i;
    i++;
  }
  return (keyHash);
}

void  *table_insert(struct s_hash_table *table, char *key, void *value)
{
  unsigned int	      index;
  unsigned int	      key_hash;
  struct s_table_entry *entry_position;

  if (!key || strcmp("", key) == 0)
    return (NULL);
  if (table_access(table, key))
	table_remove(table, key);
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

int   table_remove(struct s_hash_table *table, char *key)
{
  unsigned int		index;
  unsigned int		key_hash;
  struct s_table_entry	*entry_position;

  if (!key)
    return (0);
  key_hash = hash_function(key);
  index = key_hash % table->capacity;
  entry_position = &table->entries[index];
  if (entry_position->key == NULL)
    return (0);
  if (strcmp(key, entry_position->key) == 0)
  {
    free(entry_position->key);
    table->free_func(entry_position->value);
  }
  // else colisão
  bzero(entry_position, sizeof(struct s_table_entry));
  table->size--;
  printf("tableSize no delete %d\n", table->size);
  return (0);
}

void  *table_access(struct s_hash_table *table, char *key)
{
  unsigned int		index;
  unsigned int		key_hash;
  struct s_table_entry	*entry_position;

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

int main(void)
{
  struct s_hash_table  *table = malloc_table(10, free);
  char		      *access;

  if (!table)
    return (0);
  table_insert(table, "prop0", strdup("string 0"));
  printf("Access na prop0: %s\n", (char *)table_access(table, "prop0"));
  table_insert(table, "prop0", strdup("Agora sobreescreveu"));
  table_remove(table, "prop1");
  table_remove(table, "prop2");
  table_remove(table, "prop3");
  printf("Access na prop0: %s\n", (char *)table_access(table, "prop0"));
  printf("Access na prop1: %s\n", (char *)table_access(table, "prop1"));
  printf("Access na prop2: %s\n", (char *)table_access(table, "prop2"));
  free_table(table);
  return (0);
}

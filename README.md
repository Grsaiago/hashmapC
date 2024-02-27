
# C Hash Table

This is my implementation of a hashmap in C


## Installation
Although this project wasn't made to be used, if you want to, you just have to clone this repo and copy the only file into your project src folder

```bash
cp hashmap.c <path to your C source files>
```
    
## Usage/Examples

```c
int main(void) {
    struct s_hash_table    *table;
    void                    *str;

    table = malloc_table(10); // First you have to allocate the hash table.
    str = strdup("example_value");  // creating a sample value.
    table_insert(table, "example_key", str); // inserting is easy as passing the key and a pointer to the value.
    printf("%s\n", (char *)table_access(table, "key")); // accessing is as easy as passing the key you want to access.
    freeTable(table); // don't forget to free the requested memory, just as in malloc/free api ;)
}
```


## Roadmap

- Colision resolution.

- Make the internal array into a vector to allow dynamic growth of size.

- Implement a decent hashing algorithm

## Documentation

### struct s_hash_table    \*malloc_table(unsigned int size, void (*free_func)(void *))
Description: Creates an instance of the hashTable with initial *size* ammount of buckets.

Parameters:
* size: The initial size of the hash table.
* free_func: A pointer to a function that frees the data structure you're going to store as the value in the table. This is used in the table destruction, to ensure that there is no data leaks.

Return: A hash table instance.


### void    free_table(struct s_hash_table *table)
Description: Frees the *table* instance and calls the *free_func* on every existing bucket.

Parameters:
* table: The hash table to be freed.

* Return: n/a


### struct void    *tableInsert(struct s_hash_table *table, char *key, void *value)
Description: Inserts in the given *table* the value *value* with *key* as the access key to said value.

Parameters:
- table: The hash table to be operated on.
- key: A string that will become the key of the new bucket.
- value: The new value to be associated with the key and inserted in the bucket.

Return: the inserted value


### int     table_remove(void *table, char *key)
Description: Calls the free_func on the value assotiated with *key*, and delete said *key*.

Parameters:
- table: The hash table to be operated on.
- key: A string that has the same name as a key in the hash table.

Return:
0 if the value existed before being deleted, 1 otherwise.


### void    *table_access(struct s_hash_table *table, char *key)
Description: Retrieves from *table* the value associated with *key* - if so exists.

Parameters:
- table: the table to be operated on.
- key: A string that has the same name as a key in the hash table.

Return:
A pointer to the associated value of the key/value pair or NULL if *key* doesn't exist on *table*

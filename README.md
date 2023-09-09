
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
    void    *table;
    void    *str;

    table = mallocTable(10);
    str = ft_strdup("example_value");
    tableInsert(table, "example_key", str);
    printf("%s\n", (char *)tableAccess(table, "key"));

    freeTable(table);
}
```


## Roadmap

- Colision resolution.

- Make the internal array into a vector to allow dynamic growth of size.

- Enhance the hashing algorithm
## Documentation

#### void    \*mallocTable(unsigned int size, void (*freeFunc)(void *))
Description: Creates an instance of the hashTable with initial *size* initial ammount of buckets.

Parameters:
* size: The initial size of the hash table.
* freeFunc: A pointer to a function that frees the data structure you're going to store as the value in the table. This is used in the table destruction, to ensure that there is no data leaks.

Return: A hash table instance.


#### void    freeTable(void *table)
Description: Frees the *table* instance and calls the *freeFunc* on every existing bucket.

Parameters:
* table: The hash table to be freed.

#### void    *tableInsert(void *table, char *key, void *value)
Description: Inserts in the given *table* the value *value* with *key* as the access key to said value.

Parameters:
- table: The hash table to be operated on.
- key: A string that will become the key of the new bucket.
- value: The new value to be associated with the key and inserted in the bucket.

Return: n/a


#### int     tableDelete(void *table, char *key)
Description: Calls the freeFunc on the value assotiated with *key*.

Parameters:
- table: The hash table to be operated on.
- key: A string that has the same name as a key in the hash table.

Return:
0 if the value existed before being deleted, 1 otherwise.


#### void    *tableAccess(void *table, char *key)
Description: Retrieves from *table* the value associated with *key* - if so exists.

Parameters:
- table: the table to be operated on.
- key: A string that has the same name as a key in the hash table.

Return:
A pointer to the associated value of the key/value pair or NULL if *key* doesn't exist on *table*

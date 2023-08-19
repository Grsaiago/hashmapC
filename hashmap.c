#include <stdio.h>
#include "./libft/libft.h"

typedef struct s_tableMetaData
{
  unsigned int  tableSize;
  int           (*compareFunction)(void *);
  void          (*freeFunction)(void *);
} t_tableMetaData;

typedef struct s_tableEntry
{
  unsigned int  keyHash;
  char          *keyName;
  void          *value;
} t_tableEntry;

void  *mallocTable(unsigned int size, int (*compareFunc)(void *), void (*freeFunc)(void *))
{
  void  *table;
  t_tableMetaData *metadata;

  table = ft_calloc((sizeof(t_tableEntry) * size) + sizeof(t_tableMetaData), 1);
  if (!table)
    return (NULL);
  metadata = table;
  *metadata = (t_tableMetaData){
    .tableSize = size, 
    .compareFunction = compareFunc,
    .freeFunction = freeFunc
  };
  printf("O size é: %d\n", *(unsigned int *)table);
  metadata->compareFunction(table);
  return (table + sizeof(t_tableMetaData));
}

void  freeTable(t_tableMetaData *table)
{
  t_tableEntry    *contentRef;
  unsigned int    index;

  if (!table)
    return ;
  contentRef = (t_tableEntry *)table;
  table = (void *)table - sizeof(t_tableMetaData);
  index = 0;
  while (index < table->tableSize)
  {
    if (contentRef[index].value)
    {
      table->freeFunction(contentRef[index].value);
      printf("De dentro da free table a chave é: %s\n", contentRef[index].keyName);
      free(contentRef[index].keyName);
    }
    index++;
  }
  free(table);
}

static unsigned int  hashFunction(char *key)
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

void  *tableInsert(t_tableMetaData *table, char *key, void *value)
{
  unsigned int  tableindex;
  unsigned int  keyHash;
  t_tableEntry  *entryposition;

  if (!key)
  {
    printf("Invalid insertion of NULL key\n");
    return (NULL);
  }
  keyHash = hashFunction(key);
  tableindex = keyHash % *(unsigned int *)((void *)table - sizeof(t_tableMetaData));
  entryposition = (t_tableEntry *)table + tableindex;
  entryposition->keyName = ft_strdup(key, 0);
  if (!entryposition->keyName)
    return (NULL);
  entryposition->keyHash = keyHash;
  entryposition->value = value;
  printf("---- insertion of key %s start ----\n", key);
  printf("keyHash = %d\n", entryposition->keyHash);
  printf("tableindex: %d\n", tableindex);
  printf("keyName = %s\n", entryposition->keyName);
  printf("keyvalue = %s\n", entryposition->value);
  printf("---- insertion of key %s end ----\n", key);
  return (&table[tableindex]);
}

int tableDelete(void *table, char *key)
{
  unsigned int  tableIndex;
  unsigned int  keyHash;
  t_tableEntry *entryPosition;
  t_tableMetaData *tableMeta;

  printf("---- Deletion of key %s ----\n", key);
  if (!key)
  {
    printf("Invalid deletion of NULL key\n");
    printf("---- Deletion of key %s end ----\n", key);
    return (0);
  }
  keyHash = hashFunction(key);
  tableIndex = keyHash % *(unsigned int *)((void *)table - sizeof(t_tableMetaData));
  entryPosition = (t_tableEntry *)table + tableIndex;
  if (!entryPosition || entryPosition->keyHash != keyHash)
  {
    printf("Key %s doesn't exist on table\n", key);
    printf("---- Deletion of key %s end ----\n", key);
    return (0);

  }
  tableMeta = table - sizeof(t_tableMetaData);
  if (ft_strncmp(key, entryPosition->keyName, ft_strlen(entryPosition->keyName)) == 0)
    tableMeta->freeFunction(entryPosition->value);
  // else colisão
  printf("Deletion happened with the following parameters:\nKey passed: %s\nKey Deleted: %s\n", key, entryPosition->keyName);
  ft_bzero(entryPosition, sizeof(t_tableEntry));
  printf("---- Deletion of key %s end ----\n", key);
  return (0);
}

void  *tableAcess(void *table, char *key)
{
  unsigned int  keyHash;
  unsigned int  tableIndex;

  if (!key)
    return (NULL);
  keyHash = hashFunction(key);
  tableIndex = keyHash % *(unsigned int *)((void *)table - sizeof(t_tableMetaData));
  if (((t_tableEntry *)(table))[tableIndex].keyHash == keyHash)
    return (((t_tableEntry *)(table))[tableIndex].value);
  else 
    return (NULL);
}

int testF(void *ptr)
{
  printf("Funcionou a chamada de função interna de comparação\n");
  return (0);
}

int main(void)
{
  void  *table = mallocTable(10, testF, free);
  char  *access;

  if (!table)
    return (0);
  tableInsert(table, "prop0", "string 0");
  tableInsert(table, "prop1", "string 1");
  tableInsert(table, "prop2", "string 2");
  access = tableAcess(table, "prop0");
  printf("Access na prop0: %s\n", access);
  access = tableAcess(table, "prop1");
  printf("Access na prop1: %s\n", access);
  access = tableAcess(table, "prop2");
  printf("Access na prop2: %s\n", access);
  tableDelete(table, "prop0");
  tableDelete(table, "prop3");
  freeTable(table);
  return (0);
}

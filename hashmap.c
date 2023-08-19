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

  table = calloc((sizeof(t_tableEntry) * size) + sizeof(t_tableMetaData), 1);
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

void  *insertOnTable(t_tableMetaData *table, char *key, void *value)
{
  int           i;
  unsigned int  tableIndex;
  unsigned int  keyHash;
  t_tableEntry  *entryPosition;

  if (!key)
    return (NULL);
  keyHash = hashFunction(key);
  tableIndex = keyHash % *(unsigned int *)((void *)table - sizeof(t_tableMetaData));
  entryPosition = (t_tableEntry *)table + tableIndex;
  entryPosition->keyName = ft_strdup(key, 0);
  entryPosition->keyHash = keyHash;
  entryPosition->value = value;
  printf("---- Insertion Start ----\n");
  printf("KeyHash = %d\n", entryPosition->keyHash);
  printf("tableIndex: %d\n", tableIndex);
  printf("KeyName = %s\n", entryPosition->keyName);
  printf("KeyValue = %s\n", entryPosition->value);
  printf("---- Insertion End ----\n");
  return (&table[tableIndex]);
}

void  *accessTable(void *table, char *key)
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
  void  *content = ft_strdup("string 0", 0);
  char  *access;

  if (!table)
    return (0);
  insertOnTable(table, "prop0", content);
  content = ft_strdup("string 1", 0);
  insertOnTable(table, "prop1", content);
  content = ft_strdup("string 2", 0);
  insertOnTable(table, "prop2", content);
  access = accessTable(table, "prop0");
  printf("Access na prop0: %s\n", access);
  access = accessTable(table, "prop1");
  printf("Access na prop1: %s\n", access);
  access = accessTable(table, "prop2");
  printf("Access na prop2: %s\n", access);
  freeTable(table);
  return (0);
}

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

void  freeTable(t_tableMetaData *table) // tem que colocar o parametro extra pra ser uma função que vai limpar os valores das chaves
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

void  *insertOnTable(t_tableMetaData *table, char *key, void *value)
{
  int           i;
  unsigned int  tableIndex;
  unsigned int  keyHash;
  t_tableEntry  *entryPosition;

  if (!key)
    return (NULL);
  i = 0;
  while (key[i])
  {
    keyHash += key[i] * i;
    i++;
  }
  tableIndex = keyHash % *(int *)((void *)table - sizeof(t_tableMetaData));
  entryPosition = (t_tableEntry *)table + tableIndex;
  entryPosition->keyName = ft_strdup(key, 0);
  entryPosition->keyHash = keyHash;
  entryPosition->value = value;
  printf("tableIndex: %d\n", tableIndex);
  printf("KeyName = %s\n", entryPosition->keyName);
  printf("KeyHash = %d\n", entryPosition->keyHash);
  return (&table[tableIndex]);
}

int testF(void *ptr)
{
  printf("Funcionou a chamada de função interna de comparação\n");
  return (0);
}

int main(void)
{
  void  *table = mallocTable(10, testF, free);
  void  *content = malloc(10);
  if (!table)
    return (0);
  insertOnTable(table, "prop0", content);
  content = malloc(10);
  insertOnTable(table, "prop1", content);
  content = malloc(10);
  insertOnTable(table, "prop2", content);
  freeTable(table);
  return (0);
}

#include <stdio.h>
#include "./libft/libft.h"

typedef struct s_tableMetaData
{
  unsigned int  tableSize;
  unsigned int	tableCapacity;
  void          (*freeFunction)(void *);
} t_tableMetaData;

typedef struct s_tableEntry
{
  char          *keyName;
  void          *value;
} t_tableEntry;

void	*mallocTable(unsigned int size, void (*freeFunc)(void *));
void	freeTable(void *table);
static unsigned int		hashFunction(char *key);
void	*tableInsert(void *table, char *key, void *value);
int		tableDelete(void *table, char *key);
void  	*tableAccess(void *table, char *key);

void  *mallocTable(unsigned int size, void (*freeFunc)(void *))
{
  void  *table;
  t_tableMetaData *metadata;

  table = ft_calloc((sizeof(t_tableEntry) * size) + sizeof(t_tableMetaData), 1);
  if (!table)
    return (NULL);
  metadata = table;
  *metadata = (t_tableMetaData){
    .tableCapacity = size, 
    .freeFunction = freeFunc
  };
  return (table);
}

void  freeTable(void *table)
{
  t_tableEntry    *contentRef;
  unsigned int    index;

  if (!table)
    return ;
  contentRef = (t_tableEntry *)(table + sizeof(t_tableMetaData));
  index = 0;
  while (index < ((t_tableMetaData *)table)->tableCapacity)
  {
    if (contentRef[index].value)
    {
      ((t_tableMetaData *)table)->freeFunction(contentRef[index].value);
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

void  *tableInsert(void *table, char *key, void *value)
{
  unsigned int  tableindex;
  unsigned int  keyHash;
  t_tableEntry  *entryPosition;
  char			newEntry;

  if (!key)
    return (NULL);
  if (tableAccess(table, key))
	tableDelete(table, key);
  keyHash = hashFunction(key);
  tableindex = keyHash % ((t_tableMetaData *)table)->tableCapacity;
  entryPosition = (t_tableEntry *)(table + sizeof(t_tableMetaData)) + tableindex;
  entryPosition->keyName = ft_strdup(key, 0);
  if (!entryPosition->keyName)
    return (NULL);
  entryPosition->value = value;
  ((t_tableMetaData *)table)->tableSize++;
  printf("tableSize no insert %d\n", ((t_tableMetaData *)table)->tableSize);
  return (entryPosition);
}

int tableDelete(void *table, char *key)
{
  unsigned int		tableIndex;
  unsigned int		keyHash;
  t_tableEntry		*entryPosition;

  if (!key)
    return (0);
  keyHash = hashFunction(key);
  tableIndex = keyHash % ((t_tableMetaData *)table)->tableCapacity;
  entryPosition = (t_tableEntry *)(table + sizeof(t_tableMetaData)) + tableIndex;
  if (entryPosition->value == NULL)
    return (0);
  if (ft_strncmp(key, entryPosition->keyName, ft_strlen(entryPosition->keyName)) == 0)
  {
    free(entryPosition->keyName);
    ((t_tableMetaData *)table)->freeFunction(entryPosition->value);
  }
  // else colisão
  ft_bzero(entryPosition, sizeof(t_tableEntry));
  ((t_tableMetaData *)table)->tableSize--;
  printf("tableSize no delete %d\n", ((t_tableMetaData *)table)->tableSize);
  return (0);
}

void  *tableAccess(void *table, char *key)
{
  unsigned int  keyHash;
  unsigned int  tableIndex;
  t_tableEntry *entryPosition;

  if (!key)
    return (NULL);
  keyHash = hashFunction(key);
  tableIndex = keyHash % ((t_tableMetaData *)table)->tableCapacity;
  entryPosition = (t_tableEntry *)(table + sizeof(t_tableMetaData)) + tableIndex;
  if (ft_strncmp(key, entryPosition->keyName, ft_strlen(entryPosition->keyName)) == 0)
    return (entryPosition->value);
  else 
    return (NULL);
}

int main(void)
{
  void  *table = mallocTable(10, free);
  char  *access;

  if (!table)
    return (0);
  tableInsert(table, "prop0", ft_strdup("string 0", 0));
  printf("Access na prop0: %s\n", (char *)tableAccess(table, "prop0"));
  tableInsert(table, "prop0", ft_strdup("string 0 sobrescrevendo", 0));
  tableDelete(table, "prop1");
  tableDelete(table, "prop1");
  tableDelete(table, "prop1");
  printf("Access na prop0: %s\n", (char *)tableAccess(table, "prop0"));
  printf("Access na prop1: %s\n", (char *)tableAccess(table, "prop1"));
  printf("Access na prop2: %s\n", (char *)tableAccess(table, "prop2"));
  freeTable(table);
  return (0);
}

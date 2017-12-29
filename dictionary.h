#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
typedef struct _record
{ 
    char *word;
    char *meaning; 
    struct _record *next; 
    struct _record *prev;
} record;  

typedef struct _dictionary
{ 
    size_t size;
    record *head; 
    record *tail;     
} dictionary;

/* create dictionary
 * RETURN VALUE: pointer on struct dictionary -  if success
 *               NULL - if memory allocation error
 */
dictionary *createDictionary();

/** delete dictionary
 * RETURN VALUE: none
 */
void deleteDictionary(dictionary *glossary);

/* addRecord   
 * RETURN VALUE: 0 - success
 *               1 - memory allocation error
 */
int addRecord(char *word, char *meaning, dictionary *glossary);

/*search record
 * RETURN VALUE: pointer on record - if record was found
 *               NULL - if record not found
 */
record *searchRecord(char *searchWord, dictionary *glossary);

/* delete record
 * RETURN VALUE: 0 - success
 *               1 - record doesn't exist
 */
int deleteRecord(char *deleteWord, dictionary *glossary);

/** list all words from dictionary
 * RETURN VALUE: none
 */
void listAllWords(dictionary *glossary);

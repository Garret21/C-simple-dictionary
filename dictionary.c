#include "dictionary.h"

/** create dictionary
 * RETURN VALUE: pointer on struct dictionary -  if success
 *               NULL - if memory allocation error
 */
dictionary *createDictionary()
{
    dictionary *tmp = (dictionary*) malloc(sizeof(dictionary));
    if (tmp != NULL)
    {   
        tmp->size = 0;
        tmp->head = tmp->tail = NULL;
    }
    return tmp;
}


/** delete dictionary
 * RETURN VALUE: none
 */
void deleteDictionary(dictionary *glossary)
{
    record *tmp = glossary->head;
    record *next = NULL;
    while(tmp)
    {
        next = tmp->next;
        free(tmp->word);
        free(tmp->meaning);
        free(tmp);
        tmp = next;
    }
    free(glossary);
}


/** add record   
 * RETURN VALUE: 0 - success
 *               1 - memory allocation error
 */
int addRecord(char *word, char *meaning, dictionary *glossary)
{
    int retcode = 0;
    record *tmp = (record*) malloc(sizeof(record));
    if (tmp != NULL) 
    {   
        tmp->word = word;
        tmp->meaning = meaning;
        tmp->prev = glossary->tail;
        tmp->next = NULL;
        if ( glossary->tail != NULL )
            glossary->tail->next = tmp;
        glossary->tail = tmp;
        if ( glossary->head == NULL )
            glossary->head = tmp;
        glossary->size++;
    }
    else
    {               //memory allocation error
        retcode = 1;
        printf("memory allocation error\n");
    }
    return retcode;
}


/** search record
 * RETURN VALUE: pointer on record - if record was found
 *               NULL - if record not found
 */
record *searchRecord(char *searchWord, dictionary *glossary)
{
    record *result=NULL;
    record *tmp = glossary->head;
    int counter = 0;
    while ( counter<glossary->size && strcmp(searchWord,tmp->word) != 0)
    {
        tmp = tmp->next;
        counter++;
    }
    if ( counter < glossary->size )
        result = tmp;   //record found
    return result;
}


/** delete record
 * RETURN VALUE: 0 - success
 *               1 - record not found
 */
int deleteRecord(char *deleteWord, dictionary *glossary)
{
    int retcode = 0;
    record *tmp = searchRecord(deleteWord, glossary);
    if ( tmp != NULL )
    {
        if ( tmp == glossary->head )
            glossary->head = tmp->next;
        if ( tmp == glossary->tail )
            glossary->tail = tmp->prev;
        if ( tmp->next != NULL)
            tmp->next->prev = tmp->prev;
        if ( tmp->prev != NULL )
            tmp->prev->next = tmp->next;
        glossary->size--;
        free(tmp);
    }
    else retcode = 1;    //record not found
    return retcode;
}


/** list all words from dictionary
 * RETURN VALUE: none
 */
void listAllWords(dictionary *glossary)
{
    record *tmp = glossary->head;
    int counter = 0;
    while ( tmp != NULL )
    {   
        printf("%d| %s\n", ++counter, tmp->word);
        tmp = tmp->next;
    }
}


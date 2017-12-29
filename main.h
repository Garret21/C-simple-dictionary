#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

#define DEFAULT_PATH "dictionary.txt"
#define SEPARATOR '-'
#define WORDS_LENGTH 4
#define MEANINGS_LENGTH 4
#define BUFFERS_LENGTH 4


/** read line from stream 
  * RETURN VALUE: 0 - success
  *               1 - memory allocation error
  *               5 - empty stream error
  */
int readLine(FILE *stream, char **line);

/** parsing file
  * RETURN VALUE: 0 - success   two lines with word and meaning  
  */
int parseRecord(char *source, char **word, char **meaning);

/** load dictionary - read file and load dictionary into memory
 * RETURN VALUE: 0 - success
 *               1 - file doesn't exist
 *               2 - can't open file
 *               3 - closing file error
 *               4 - reading error
 */
int loadDictionary(char *path, dictionary *glossary);

/** write record
 * RETURN VALUE: 0 - success
 *               4 - writing error
 */
int writeRecord(FILE *stream, record *_record, char separator);

/* unload dictionary - write dictionary to file 
 * RETURN VALUE: 0 - success
 *               2 - can't open file
 *               3 - writing error
 *               4 - closing file error
 */
int unloadDictionary(char *path, dictionary *glossary);

/* read word and meaning from console and add new record in dictionary
 * RETURN VALUE: 0 - success
 *               1 - error
 */
int addNewRecord(dictionary *glossary);


/* read word, find record and delete it from dictionary
 * RETURN VALUE: 0 - success
 *               1 - error
 */
int delRecord(dictionary *glossary);


/*read word, find record and print it
 * RETURN VALUE: 0 - success
 *               1 - error
 */
int findRecord(dictionary *glossary);


/** work with dictionary - search, add and delete record
 * RETUEN VALUE: none
 */
void workWithDictionary(dictionary *glossary);

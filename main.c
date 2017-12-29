#include "main.h"


/** read line from stream 
 * RETURN VALUE: 0 - success
 *               1 - memory error
 *               5 - empty stream error
 */
int readLine(FILE *stream, char** line)
{
    int retcode = 0;
    char *buffer = NULL;
    if ( stream != NULL )
    {
        buffer = (char*) malloc(sizeof(char)*BUFFERS_LENGTH+1);
        if ( buffer != NULL )
        {
            unsigned int i=0;
            unsigned int N = BUFFERS_LENGTH; //buffer's start length
            while ( (buffer[i] = getc(stream)) != '\n' )
            {
                if (++i >= N)
                {   //extension buffer
                    N = N+N;
                    char *savebuffer = NULL;
                    savebuffer = (char*) realloc (buffer, sizeof(char)*N+1);
                    if (savebuffer != NULL) 
                        buffer = savebuffer;
                    else
                    {
                        printf("memory reallocation error\n");
                        retcode = 1;
                    }
                }
            }
            buffer[i] = '\0';
            *line = buffer;
        }
        else
        {
            printf("memory allocation error\n");
            retcode = 1;    //memory allocation error
        }
    }
    else
    {
       printf("stream is empty\n");
       retcode = 5;    //empty stream error
    }
    return retcode;
}


/** parsing file
 * RETURN VALUE: 0 - success two lines with word and meaning  
 *               1 - memory error 
 */
int parseRecord(char *source, char **word, char **meaning)
{
    int retcode = 0;
    unsigned int i=0;
    unsigned int Wl = WORDS_LENGTH; //word's buffer string start value
    unsigned int Ml = MEANINGS_LENGTH; //meaning's buffer string start value
    char *tmpWord  = (char*) malloc(sizeof(char)*Wl+1);
    if (tmpWord != NULL)
    {   //parse word
        while ( source[i] != SEPARATOR )
        {
            tmpWord[i] = source[i];
            if (++i >= Wl)
            {   //extension buffer
                Wl = Wl+Wl;
                char *savebuffer = NULL;
                savebuffer = (char*) realloc (tmpWord, sizeof(char)*Wl+1);
                if (savebuffer != NULL) 
                    tmpWord = savebuffer;
                else
                {
                    printf("memory reallocation error\n");
                    retcode = 1;
                }
            }
        }
        tmpWord[i] = '\0'; // close word's string;
        *word = (char*) realloc(tmpWord, sizeof(char)*i); //clip the length of the buffer
       
        unsigned int nextWord = i+1; //offset the meaning from the sring's start
        i++;    //put pointer on the begin of the meaning
               
        char *tmpMeaning  = (char*) malloc(sizeof(char)*Ml+1);
        if (tmpMeaning != NULL)
        {   //parse meaning
            while ( source[i] != '\0' )
            {
                tmpMeaning[i-nextWord] = source[i];
                if (++i-nextWord >= Ml)
                {   //extension meaning's buffer
                    Ml = Ml+Ml;
                    char *savebuffer = NULL;
                    savebuffer = (char*) realloc (tmpMeaning, sizeof(char)*Ml+1);
                    if (savebuffer != NULL) 
                        tmpMeaning = savebuffer;
                    else
                    {
                        printf("memory reallocation error\n");
                        retcode = 6;
                    }
                }
            }
            tmpMeaning[i-nextWord] = '\0';
            *meaning = (char*) realloc(tmpMeaning, sizeof(char)*(i-nextWord));
        }
        else
        {
            printf("memory allocation error\n");
            retcode = 1;
        }
    }
    else
    {
        printf("memory allocation error\n");
        retcode = 1;
    }
    free(source);
    return retcode;    
}


/** load dictionary - read file and load dictionary into memory
 * RETURN VALUE: 0 - success
 *               1 - file doesn't exist
 *               2 - can't open file
 *               3 - closing file error
 *               4 - reading error
 */
int loadDictionary(char *path, dictionary *glossary)
{
    int retcode = 0;
    struct stat fail;
    if (!stat(path,&fail) )
    {
        FILE *source;
        if ( (source = fopen(path, "r")) != NULL )
        {
            char *buffer = NULL;             //readline buffer
            char *word = NULL;              //word buffer;
            char *meaning = NULL;          //meaning buffer
            int recordsCount, i;                    
            fscanf(source, "%d\n", &recordsCount);    
            printf("count of records - %d\n", recordsCount);
            for( i = 0; i<recordsCount; i++ )
            {   
                if ( readLine(source, &buffer) == 0 )        //if read the line successful
                {   
                    parseRecord(buffer, &word, &meaning);
                    addRecord(word, meaning, glossary);
                }
                else 
                {
                    printf("readfile error\n");
                    retcode = 4; //reading error
                    break;
                }
            }
            if ( fclose(source) != 0)       
            {
                printf("file closing error\n");
                retcode = 3;    //closing error
            }
        }
        else
        {
            printf("can`t open file\n"); 
            retcode = 2;          //can't open file
        }
    }
    else
    {
        printf("file doesn`t not exist\n");
        retcode = 1;            //file doesn't exist
    }
    return retcode;
}


/** write record
 * RETURN VALUE: 0 - success
 *               4 - writing error
 */
int writeRecord(FILE *stream, record *_record, char separator)
{
    int retcode = 0;
    if ( fputs(_record->word, stream) != EOF )
        if ( fputc((int) separator, stream) != EOF )
            if ( fputs(_record->meaning, stream) != EOF )
                if ( fputc('\n', stream) != EOF );
                else goto error;
            else goto error;
        else goto error;
    else
    {
        error:
        retcode = 4;
        printf("writing error\n");
    }
    return retcode;
}


/** unload dictionary - write dictionary to file 
 * RETURN VALUE: 0 - success
 *               2 - can't open file
 *               3 - closing file error
 *               4 - writing error
 */
int unloadDictionary(char *path, dictionary *glossary)
{
    int retcode = 0;
    FILE *destination;
    if ( (destination = fopen(path, "w")) != NULL )
    {
        int i;
        fprintf(destination, "%d\n", (int) glossary->size);
        record *tmp = glossary->head;
        for( i=0; i<(int) (glossary->size); i++)
        {
            if ( writeRecord(destination, tmp, SEPARATOR) == 0 )
            {
                tmp = tmp->next;
            }
            else
            {
                retcode =4;     //writefile error
                printf("writefile error\n");
                break;
            }
        }
        if ( fclose(destination) != 0)
        {
            retcode = 3;    //closing error
            printf("file closing error\n");
        }
    }
    else
    {
        printf("can't open file\n");
        retcode = 2;    //can't open file
    }
    return retcode;
}


/* read word and meaning from console and add new record in dictionary
 * RETURN VALUE: 0 - success
 *               1 - error
 */ 
int addNewRecord(dictionary *glossary)
{
    int retcode = 0;
    printf("input new word: ");
    char *newWord = NULL;
    readLine(stdin, &newWord);
    printf("input meaning for word: %s\n>", newWord); 
    char *newMeaning = NULL;
    readLine(stdin, &newMeaning);
    if ( addRecord(newWord, newMeaning, glossary) != 0)
        retcode = 1;
    return retcode;
}


/* read word, find record and delete it from dictionary
 * RETURN VALUE: 0 - success
 *               1 - error
 */ 
int delRecord(dictionary *glossary)
{
    int retcode =0;
    printf("input word for delete: ");
    char *delWord = NULL;
    readLine(stdin,&delWord);
    if ( deleteRecord(delWord, glossary) != 0 )
        retcode = 1;
    free(delWord);
    return retcode;
}


/*read word, find record and print it
 * RETURN VALUE: 0 - success
 *               1 - error
 */
int findRecord(dictionary *glossary)
{
    printf("input word for search: ");
    int retcode = 0;
    char *word = NULL;
    readLine(stdin,&word);
    record *tmp = searchRecord(word, glossary);
    if ( tmp != NULL )
        writeRecord(stdout, tmp, SEPARATOR);
    else 
    {
        printf("record not found\n");
        retcode = 1;
    }
    free(word);
    return retcode;
}


/** work with dictionary - search, add and delete record
 * RETUEN VALUE: none
 */
void workWithDictionary(dictionary *glossary)
{
    int workflag = 1;
    while ( workflag )
    {
        printf("commands for dictionary:\n");
        printf("l - list all words\n");
        printf("s - search record\n");
        printf("a - add new record\n");
        printf("d - delete record\n");
        printf("q - quit\n");
        printf("What do you want to do? >");
        char *man = NULL;
        readLine(stdin, &man);
        switch(man[0])
        {
            case 'l':
                listAllWords(glossary);
                break;
            case 's':
                findRecord(glossary);
                break;
            case 'a':
                addNewRecord(glossary);
                break;
            case 'd':
                delRecord(glossary);
                break;
            case 'q':
                printf("quit\n");
                workflag = 0;
                break;
            default:
                printf("command not found\n");
                break;
        }
        free(man);
    }
}


int main(int argcount, char **argvalue)
{
    dictionary *glossary = createDictionary(); 
    if ( argcount > 1 ) loadDictionary(argvalue[1], glossary);
        else printf("file doesn't selected\n");
    workWithDictionary(glossary);
    if ( argcount > 2 ) unloadDictionary(argvalue[2], glossary);
    else 
    {
        printf("default path: %s\n", DEFAULT_PATH);
        unloadDictionary(DEFAULT_PATH, glossary);
    }
    deleteDictionary(glossary);
    return 0;
}

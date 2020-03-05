#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "hash_table.h"
#include "map.h"
#include "dijkstra.h"
#include "queue.h"
#include <assert.h>

#undef NDEBUG
/**
 * @brief Custom written strdup() function
 * @param *src String pointer
 * @return String pointer on successful memory allocation NULL otherwise
 */
char *my_strdup(char *src) {
    char *str;
    char *p;
    int len = 0;

    while (src[len])
        len++;
    str = (char*)malloc(sizeof(char)*(len + 1));
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}
/**
 * @brief Function that splits a_str into String array with delimiter a_delim
 * @param *a_str String for splitting
 * @param a_delim delimiter for splitting
 * @return String array pointer of split resilt on successful memory allocation NULL otherwise
 */
char** str_split(char* a_str, const char a_delim) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str + strlen(a_str) - 1);

    count++;

    result = malloc(sizeof(char*) * count);

    if (result) {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = my_strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}
/**
 * @brief Insert value into arr[i] dynamic array
 * @param i Position in array
 * @param value Value to insert
 * @param *arr Array pointer
 * @param *capacity Maximum capacity of the array
 * @return Array pointer back on successful memory allocation NULL otherwise
 */
char *pushArr(int i, char value, char *arr, int *capacity) {
    if (i > (*capacity) - 1) {
        *capacity *= 2;
        arr = (char *) realloc(arr, sizeof(char) * (*capacity));
    }
    if (arr)
        arr[i] = value;
    return arr;
}
/**
 * @brief Reach the end of line
 */
void reachEnd() {
    while ((getchar()) != '\n') {}
}
/**
 * @brief Have a peek at the first character of current line
 * @return First charater
 */
int fpeek() {
    int c;
    c=getc(stdin);
    ungetc(c, stdin);
    return c;
}
/**
 * @brief Print ERROR to .err file
 * @param lineNum Line number
 */
void err(int lineNum) {
    fprintf(stderr, "ERROR %d\n", lineNum);
}
/**
 * @brief Write a line into a new string
 * @param *line String pointer to line
 * @param *valid Boolean variable for reporting
 * @return String pointer on successful memory allocation NULL otherwise
 */
char *takeLine(char *line, bool* valid) {
    int i = 0;
    int cap = STARTING_SIZE;
    char letter;
    char prev='\0';
    while ((letter = (char) getchar()) != '\n') {
        line = pushArr(i, letter, line, &cap);
        if(prev==';' && letter==';')
            *valid=false;
        prev=letter;
        i++;
    }
    if(prev==';') {*valid=false;}
    line = pushArr(i, '\0', line, &cap);
    return line;
}
/**
 * @brief Free array of strings
 * @param **Sarr array of strings
 */
void freeSarr(char** Sarr) {
    int i;
    for (i = 0; Sarr[i]; i++)
        free(*(Sarr + i));
    free(Sarr);
}
/**
 * @brief Free everything and return false
 * @param *line line String
 * @param **Sarr array of strings
 * @param *ht hash table pointer
 * @return false for later return
 */
bool fail(char* line, char** Sarr, ht_hash_table* ht) {
    if(line) {free(line);}
    if(Sarr) {freeSarr(Sarr);}
    if(ht) {ht_del_hash_table(ht);}
    return false;
}
/**
 * @brief Find the length of array of strings
 * @param **s array of strings
 * @return Length
 */
int strstrlen(char** s) {
    int i;
    for(i = 0; s[i]; i++)
        ;
    return i;
}
/**
 * @brief Find out whether or not s is a number
 * @param *s String
 * @return Whether or not s is numeric
 */
bool isNumeric(char* s) {
    int length = strlen (s);
    int i;
    for (i=0;i<=length-1; i++)
        if (!('0'<=s[i] && s[i]<='9'))
            return false;
    return true;
}
/**
 * @brief Correctly convert lengthString to number, evaluate and return
 * @param *lengthString String for length
 * @param *length Int pointer where length will be written
 * @return Whether or not got correct number
 */
bool lengthIntConverter(char* lengthString, unsigned int* length) {
    if(!isNumeric(lengthString)) {return false;}
    long int temp=atol(lengthString);
    if(strlen(lengthString)>MAX_LENGTH_DIGITS || temp>UINT_MAX || temp==0)
        return false;
    *length=(unsigned)temp;
    return true;
}
/**
 * @brief Correctly convert builtYearString to number, evaluate and return
 * @param *builtYearString String for length
 * @param *builtYear Int pointer where length will be written
 * @return Whether or not got correct number
 */
bool builtYearIntConverter(char* builtYearString, int* builtYear) {
    if(!isNumeric(builtYearString)) {return false;}
    long int temp=atol(builtYearString);
    if(strlen(builtYearString)>MAX_YEAR_DIGITS_WITH_MINUS || temp>INT_MAX || temp<INT_MIN || temp==0)
        return false;
    *builtYear=(int)temp;
    return true;
}
/**
 * @brief Try to make a route from description
 * @param *m Map pointer
 * @return Whether or not function was executed affirmatively or negatively
 */
bool doMakeRoute(Map* m) {
    char *line = (char *) malloc(sizeof(char));
    bool valid=true;
    line = takeLine(line, &valid);
    if(!valid)
        return fail(line,NULL,NULL);
    char** Sarr=str_split(line,';');
    if(!Sarr)
        return fail(line,NULL,NULL);
    int n=strstrlen(Sarr);
    if(n<MIN_POSSIBLE_ROUTE || (n-ROUTEID_AND_FIRST_CITY)%ROUTE_REPEAT_NUM!=0 )
        return fail(line,Sarr, NULL);
    int routeId;
    char* Idstr=Sarr[0];
    if(!isNumeric(Idstr))
        return fail(line,Sarr,NULL);
    routeId=(unsigned)atoi(Idstr);
    if(strlen(Idstr)>MAX_ROUTEID_DIGITS || !correctRouteId(routeId) || m->routes[routeId].startCity)
        return fail(line, Sarr, NULL);
    char* head=Sarr[1];
    if(!correctName(head))
        return fail(line, Sarr, NULL);
    unsigned length;
    char* prev=head;
    int builtYear;
    struct list* edge;
    long int temp;
    ht_hash_table* ht=ht_new();
    ht_insert(ht,head,1);
    int c,cprev;

    int i;
    for(i=2;i<=n-1;i+=3) {
        if(!lengthIntConverter(Sarr[i], &length)) {return fail(line,Sarr,ht);}
        if(!builtYearIntConverter(Sarr[i+1], &builtYear)) {return fail(line,Sarr,ht);}
        if(!correctName(Sarr[i+2]))
            return fail(line,Sarr,ht);
        if(ht_search(ht,Sarr[i+2])!=MISSING) // If visited
            return fail(line,Sarr,ht);
        ht_insert(ht,Sarr[i+2],i+2);
        if((c=searchCity(m,Sarr[i+2]))!=MISSING && (cprev=searchCity(m,prev))!=MISSING) {
            edge=getAdjNode(m,cprev,c);
            if(edge) {
                if(edge->length!=length || edge->builtYear>builtYear)
                    return fail(line,Sarr,ht);
            }
        }
        prev=Sarr[i+2];
    }
    if(ht) {ht_del_hash_table(ht);}
    if(searchCity(m,head)==MISSING)
        addCity(m,head);
    const char* headConst=m->arr[searchCity(m,head)].city;
    m->routes[routeId].startCity=headConst;
    prev=head;
    struct routeList** path=m->routes[routeId].routeList;
    struct routeList* lastP=NULL;
    for(i=2;i<=n-1;i+=3) {
        temp=atol(Sarr[i]);
        length=(unsigned)temp;
        temp=atol(Sarr[i+1]);
        builtYear=(int)temp;
        addRoad(m,prev,Sarr[i+2],length,builtYear);
        c=searchCity(m,Sarr[i+2]);
        cprev=searchCity(m,prev);
        edge=getAdjNode(m,cprev,c);
        edge->builtYear=builtYear;
        lastP=appendRoute(path, edge,lastP);
        prependBelongs(routeId,edge->belongs);
        prev=Sarr[i+2];
    }
    if(line) {free(line); line=NULL;}
    if(Sarr) {freeSarr(Sarr);}
    return true;
}
/**
 * @brief Correctly convert Idstr to number, evaluate and return
 * @param *Idstr routeId String
 * @return Whether or not got correct number
 */
bool routeIdIntConverter(char* Idstr, unsigned* routeId) {
    if(!isNumeric(Idstr) || strlen(Idstr)>MAX_ROUTEID_DIGITS) {return false;}
    *routeId=(unsigned)atoi(Idstr);
    return true;
}
/**
 * @brief Try to take and execute repairRoad()
 * @param *m Map pointer
 * @param **Sarr Array of strings
 * @return Whether or not function was executed affirmatively or negatively
 */
bool doRepairRoad(struct Map* m, char** Sarr) {
    if(strstrlen(Sarr)!=4) {return false;}
    char *city1=Sarr[1];
    char *city2=Sarr[2];
    int builtYear;
    if(!builtYearIntConverter(Sarr[3], &builtYear)) {return false;}
    return repairRoad(m,city1,city2,builtYear);
}
/**
 * @brief Try to take and execute getRouteDescription()
 * @param *m Map pointer
 * @param **Sarr Array of strings
 * @return Whether or not function was executed affirmatively or negatively
 */
const char* doGetRouteDescription(struct Map* m, char** Sarr) {
    if(strstrlen(Sarr)!=2) {return NULL;}
    unsigned routeId;
    if(!routeIdIntConverter(Sarr[1], &routeId)) {return NULL;}
    return getRouteDescription(m,routeId);
}
/**
 * @brief Try to take and execute addRoad()
 * @param *m Map pointer
 * @param **Sarr Array of strings
 * @return Whether or not function was executed affirmatively or negatively
 */
bool doAddRoad(struct Map* m, char** Sarr) {
    if(strstrlen(Sarr)!=5) {return false;}
    char *city1=Sarr[1];
    char *city2=Sarr[2];
    unsigned length;
    int builtYear;
    if(!lengthIntConverter(Sarr[3], &length)) {return false;}
    if(!builtYearIntConverter(Sarr[4], &builtYear)) {return false;}
    return addRoad(m,city1,city2,length,builtYear);
}
/**
 * @brief Try to take and execute newRoute()
 * @param *m Map pointer
 * @param **Sarr Array of strings
 * @return Whether or not function was executed affirmatively or negatively
 */
bool doNewRoute(struct Map* m, char** Sarr) {
    if(strstrlen(Sarr)!=4) {return false;}
    unsigned routeId;
    if(!routeIdIntConverter(Sarr[1], &routeId)) {return false;}
    char *city1=Sarr[2];
    char *city2=Sarr[3];
    return newRoute(m,routeId,city1,city2);
}
/**
 * @brief Try to take and execute extendRoute()
 * @param *m Map pointer
 * @param **Sarr Array of strings
 * @return Whether or not function was executed affirmatively or negatively
 */
bool doExtendRoute(struct Map* m, char** Sarr) {
    if(strstrlen(Sarr)!=3) {return false;}
    unsigned routeId;
    if(!routeIdIntConverter(Sarr[1], &routeId)) {return false;}
    char *city=Sarr[2];
    return extendRoute(m,routeId,city);
}
/**
 * @brief Try to take and execute removeRoad()
 * @param *m Map pointer
 * @param **Sarr Array of strings
 * @return Whether or not function was executed affirmatively or negatively
 */
bool doRemoveRoad(struct Map* m, char** Sarr) {
    if(strstrlen(Sarr)!=3) {return false;}
    char *city1=Sarr[1];
    char *city2=Sarr[2];
    return removeRoad(m,city1,city2);
}
/**
 * @brief Try to take and execute removeRoute()
 * @param *m Map pointer
 * @param **Sarr Array of strings
 * @return Whether or not function was executed affirmatively or negatively
 */
bool doRemoveRoute(struct Map* m, char** Sarr) {
    if(strstrlen(Sarr)!=2) {return false;}
    unsigned routeId;
    if(!routeIdIntConverter(Sarr[1], &routeId)) {return false;}
    return removeRoute(m,routeId);
}
/**
 * @brief Try to take command if so execute it
 * @param *m Map pointer
 * @return Whether or not function was executed affirmatively or negatively
 */
bool doCommand(struct Map* m) {
    const char* description=NULL;
    char *line = (char *) malloc(sizeof(char));
    bool valid=true;
    line = takeLine(line, &valid);
    if(!valid)
        return fail(line,NULL,NULL);
    char** Sarr=str_split(line,';');
    if(!Sarr)
        return fail(line,NULL,NULL);
    if(strcmp(Sarr[0],"getRouteDescription")==EQUAL) {
        description=doGetRouteDescription(m, Sarr);
        if(!description) {valid=false;}
        if(valid) {printf("%s\n", description);}
        free((void*)description);
    }
    else if(strcmp(Sarr[0],"addRoad")==EQUAL) {valid=doAddRoad(m,Sarr);}
    else if(strcmp(Sarr[0],"repairRoad")==EQUAL) {valid=doRepairRoad(m,Sarr);}
    else if(strcmp(Sarr[0],"newRoute")==EQUAL) {valid=doNewRoute(m,Sarr);}
    else if(strcmp(Sarr[0],"extendRoute")==EQUAL) {valid=doExtendRoute(m,Sarr);}
    else if(strcmp(Sarr[0],"removeRoad")==EQUAL) {valid=doRemoveRoad(m,Sarr);}
    else if(strcmp(Sarr[0],"removeRoute")==EQUAL) {valid=doRemoveRoute(m,Sarr);}
    else {return fail(line,Sarr, NULL);}
    if(line) {free(line);}
    freeSarr(Sarr);
    return valid;
}
int main() {
    struct Map* m=newMap();
    int lineNum=1;
    bool valid;
    int firstChar;
    while(1<2) {
        firstChar=fpeek();
        if(firstChar==EOF) {break;}
        if(firstChar=='\n' || firstChar=='#') {
            lineNum++;
            reachEnd();
        }
        else if('0'<firstChar && firstChar<='9') {
            valid=doMakeRoute(m);
            if(!valid)
                err(lineNum);
            lineNum++;
        }
        else if(
                firstChar==FIRST_LETTER_ADDROAD ||
                firstChar==FIRST_LETTER_REPAIR_OR_REMOVE_ROAD_OR_ROUTE ||
                firstChar==FIRST_LETTER_GETROUTEDESCRIPTION ||
                firstChar==FIRST_LETTER_NEWROUTE ||
                firstChar==FIRST_LETTER_EXTENDROUTE) {
            valid=doCommand(m);
            if(!valid)
                err(lineNum);
            lineNum++;
        }
        else {
            err(lineNum);
            lineNum++;
            reachEnd();
        }
    }
    deleteMap(m);
    return 0;
}


#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "map.h"
#include "dijkstra.h"
bool prependBelongs(int data, struct belongNode **head) {
    struct belongNode* new;
    new=(struct belongNode*)malloc(sizeof(struct belongNode));
    if(new==NULL){return false;}
    new->id=data;
    new->next=*head;
    *head=new;
    return true;
}
/**
 * @brief Try to prepend route linked list to another route linked list
 * @param **head head routeList
 * @param *prefix prepending routeList
 * @return Whether or not function was executed affirmatively or negatively
 */
bool prependRouteList(struct routeList **head, struct routeList* prefix) {
    if(!prefix || !head || !*head) return false;
    struct routeList* tmp=prefix;
    while(tmp->next) {tmp=tmp->next;}
    tmp->next=*head;
    *head=prefix;
    return true;
}
bool prependRoute(struct routeList **head, struct list* edge) {
    struct routeList* new;
    new=(struct routeList*)malloc(sizeof(struct routeList));
    if(new==NULL){return false;}
    new->edge=edge;
    new->next=*head;
    *head=new;
    return true;
}
/**
 * @brief Try to append to route linked list
 * @param **head head routeList
 * @param *postfix appending routeList
 * @return Whether or not function was executed affirmatively or negatively
 */
bool appendRouteList(struct routeList **head, struct routeList* postfix) {
    if(!postfix || !head || !*head) return false;
    struct routeList* tmp=*head;
    while(tmp->next) {tmp=tmp->next;}
    tmp->next=postfix;
    return true;
}
struct routeList* appendRoute(struct routeList **head, struct list* edge, struct routeList* lastP) {
    if(!edge || !head) return lastP;
    if(*head==NULL) {prependRoute(head,edge); return *head;}
    lastP->next=(struct routeList*)malloc(sizeof(struct routeList));
    lastP->next->edge=edge;
    lastP->next->next=NULL;
    lastP=lastP->next;
    return lastP;
}
void freeBelongList(struct belongNode* head) {
    struct belongNode *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}
struct list* newList() {
    struct list* l;
    l=(struct list*)malloc(sizeof(struct list));
    l->next=NULL;
    return l;
}

struct Map* newMap() {
    struct Map* m;
    m=(struct Map*)malloc(sizeof(struct Map));
    if(!m){return NULL;}
    m->count=0;
    m->size=1;
    m->arr=NULL;
    m->ht=NULL;
    int i;
    for(i=0;i<=MAX_ROUTES-1;i++) {
        m->routes[i].routeList=(struct routeList**)malloc(sizeof(struct routeList*));
        if(!m->routes[i].routeList) {deleteMap(m); return NULL;}
        *m->routes[i].routeList=NULL;
        m->routes[i].startCity=NULL;
    }
    m->arr=(struct arr*)realloc(m->arr,sizeof(struct arr));
    if(!m->arr) {
        deleteMap(m);
        return NULL;
    }
    m->arr->head=NULL;
    m->arr->city=NULL;

    m->ht = ht_new();
    if(!m->ht) {
        deleteMap(m);
        return NULL;
    }
    return m;
}

bool correctName(const char *city) {
    if(!city) {return false;}
    int length = strlen(city);
    if(length==0) {return false;}
    int i;
    for (i=0; i<=length-1; i++) {
        if (('\0' <= city[i] && city[i] < ' ') || city[i]==';')
            return false;
    }
    return true;
}

struct arr* extend(struct arr* arr, int* size, bool* failed) {
    struct arr* tmp_arr;
    if((tmp_arr=realloc(arr, sizeof(struct arr)*(*size)*(MULTIPLIER)))) {
        arr=tmp_arr;
        *failed=false;
    }
    if(tmp_arr)
        *size=(*size)*(MULTIPLIER);
    return arr;
}

int addCity(struct Map* m, const char* city) {
    struct arr* arr=m->arr;
    int count=m->count;
    int size=m->size;
    //int tmp_size=size;
    if(count>=size) {
        bool no_mem=true;
        m->arr=extend(arr,&size, &no_mem);
        if(no_mem) {return NOMEM;}
    }
    arr=m->arr;
    if(!arr) {return NOMEM;}

    arr[count].city=(char*)malloc(strlen(city)*sizeof(char)+1);
    if(!arr[count].city) {return NOMEM;}
    strcpy((char*)arr[count].city,city);
    city=arr[count].city;
    ht_insert(m->ht, city, count);

    arr[count].head=NULL;


    m->count=(m->count)+1;
    m->size=size;
    return count;
}

struct list *getAdjNode(Map* m, int C1, int C2) {
   struct arr* arr=m->arr;
   if(!arr[C1].head || !arr[C2].head) {return NULL;}
   struct list* l=arr[C1].head;
   while(l) {
	  if(C2==l->isIn) {return l;}
	  l=l->next;
   }
   return NULL;
}

bool haveRoad(struct Map* m, const char* city1, const char* city2) {
   int C1= searchCity(m,city1);
   int C2= searchCity(m,city2);
   struct arr* arr=m->arr;
    if(!arr[C1].head || !arr[C2].head) {return false;}
    struct list* l=arr[C1].head;
    while(l) {
        if(C2==l->isIn) {return true;}
        l=l->next;
    }
    return false;
}

int searchCity(struct Map* m, const char* city) {
   return ht_search(m->ht,city);
}

bool connect(struct Map* m, int C1, int C2,unsigned length, int builtYear) {
    struct belongNode** belong1;
    struct belongNode** belong2;
    const char* city1=m->arr[C1].city;
    const char* city2=m->arr[C2].city;

    belong1=(struct belongNode**)malloc(sizeof(struct belongNode*));
    belong2=(struct belongNode**)malloc(sizeof(struct belongNode*));
    if(!belong1 || !belong2) {return false;}
    *belong1=NULL;
    *belong2=NULL;

    struct arr* arr=m->arr;
    struct list* l1=arr[C1].head;
    struct list* l2=arr[C2].head;
    if(l1==NULL) {
        arr[C1].head=newList();
        l1=arr[C1].head; if(!l1) {return false;}
    }else {
        while(l1->next) {l1=l1->next;}
        l1->next=newList(); if(!(l1->next)) {return false;}
        l1=l1->next;
    }
    l1->city=city2;
    l1->from=C1;
    l1->isIn=C2;
    l1->length=length;
    l1->builtYear=builtYear;
    l1->belongs=belong1;
    if(l2==NULL) {
        arr[C2].head=newList();
        l2=arr[C2].head; if(!l2) {return false;}
    }else {
        while(l2->next) {l2=l2->next;}
        l2->next=newList(); if(!(l2->next)) {return false;}
        l2=l2->next;
    }
    l2->city=city1;
    l2->from=C2;
    l2->isIn=C1;
    l2->length=length;
    l2->builtYear=builtYear;
    l2->belongs=belong2;
    return true;
}

void freeList(struct list* head) {										// Frees a linked list
    if(!head){return;}
    struct list* tmp;
    while (head) {
        tmp=head;
        head=head->next;
        freeBelongList(*(tmp->belongs));
        free(tmp->belongs);
        free(tmp);
    }
}

void freeRouteList(struct routeList* head) {										// Frees a linked list
    if(!head){return;}
    struct routeList* tmp;
    while (head) {
        tmp=head;
        head=head->next;
        free(tmp);
    }
}

bool addRoad(struct Map* map, const char* city1, const char* city2, unsigned length, int builtYear) {
    if(!map || !city1 || !city2 || builtYear==0 || length==0) {return false;}
    if(!correctName(city1) || !correctName(city2)) {return false;}
    if(strcmp(city1,city2)==EQUAL){return false;}
    int C1=searchCity(map, city1);
    int C2=searchCity(map, city2);
    if(C1==MISSING) {C1=addCity(map, city1);}
    if(C2==MISSING) {C2=addCity(map, city2);}
    if(C1==NOMEM || C2==NOMEM) {return false;}
    if(haveRoad(map,city1,city2)) {return false;}
    return connect(map,C1,C2,length,builtYear);
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    if(!map || !city1 || !city2 || repairYear==0) {return false;}
    if(!correctName(city1) || !correctName(city2)) {return false;}
    if(strcmp(city1,city2)==EQUAL){return false;}
    int C1= searchCity(map,city1);
    int C2= searchCity(map,city2);
    if(C1==MISSING || C2==MISSING ) {return false;}

    if(!haveRoad(map,city1,city2)) {return false;}
    struct list* l1=getAdjNode(map,C1,C2);
    struct list* l2=getAdjNode(map,C2,C1);
    if(l1->builtYear>repairYear) {return false;}
    l1->builtYear=repairYear;
    l2->builtYear=repairYear;
    return true;
}
/**
 * @brief Unban all cities
 * @param *map Map pointer
 */
void unbanAll(Map* m) {
    int i;
    for(i=0;i<=(m->count)-1;i++)
        m->arr[i].banned=false;
}
/**
 * @brief Search for a path
 * @param *m Map pointer
 * @param **sequence routeList double pointer
 * @param routeId routeId of route
 * @param *city1 City1
 * @param *city2 City2
 * @return Whether or not function was executed affirmatively or negatively
 */
bool getPath(Map* m, struct routeList** sequence, unsigned routeId, const char* city1,const char* city2) {
    struct arr* arr=m->arr;
    int C1=searchCity(m,city1);
    int C2=searchCity(m,city2);
    unbanAll(m);
    if(!dijkstra(m,C1,NULL,NULL)) {return false;}
    if(!arr[C2].exists || arr[C2].prev==UNDEF ) {return false;}
    bool exists=true;
    int u=C2;
    while(arr[u].prev!=UNDEF) {
        if(!arr[u].exists) {exists=false; break;}
        if(!prependRoute(sequence, arr[u].routeEdge)) {exists=false; break;}

        u=arr[u].prev;
    }
    u=C2;
    while(arr[u].prev!=UNDEF) {
        if(!prependBelongs(routeId,arr[u].routeEdge->belongs)) {exists=false; break;}
        arr[u].routeEdge=NULL;
        u=arr[u].prev;
    }
    if(!exists){
        freeRouteList(*sequence);
        *sequence=NULL;
        return false;
    }
    return true;
}

bool correctRouteId(unsigned routeId) {
    return MIN_ROUTE <= routeId && routeId < MAX_ROUTES;
}

bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
    if(!map || !city1 || !city2) {return false;}
    if(!correctName(city1) || !correctName(city2)) {return false;}
    if(strcmp(city1,city2)==EQUAL){return false;}
    int C1= searchCity(map,city1);
    int C2= searchCity(map,city2);
    if(C1==MISSING || C2==MISSING ) {return false;}

    if (!correctRouteId(routeId)) {return false;}
    struct routeArr* routes  = map->routes;
    if(*(routes[routeId].routeList)) {return false;}
    routes[routeId].startCity=map->arr[C1].city;
    struct routeList** path=routes[routeId].routeList;
    if(!getPath(map,path,routeId, city1,city2)) {return false;}
    return true;
}

unsigned length(int u, struct list* X) {
    (void)u;
    return X->length;
}
/**
 * @brief Count number of digits of a number
 * @param n Number
 * @return Number of digits
 */
int digs(int n) {
    int r=1;
    if(n<0) {n=(n==INT_MIN) ? INT_MAX : -n;}
    while(n>9) {
        n/=10;
        r++;
    }
    return r;
}
/**
 * @brief repairRoad() according to the specifications of the project
 * @param *m Map pointer
 * @param end endpoint
 * @param *oldest int pointer to set oldest
 * @param *length int pointer to set overall length
 * @return routeList pointer upon successful memory allocation
 */
struct routeList* constructRoute(Map* m, int end, int* oldest, unsigned long long* length) {
    struct arr* arr=m->arr;
    if(length) {*length=arr[end].dist;}
    if(oldest) {*oldest=arr[end].oldest;}

    if(!arr[end].exists || arr[end].prev==UNDEF ) {return false;}
    bool exists=true;
    struct routeList* sequence=NULL;
    int u=end;
    while(arr[u].prev!=UNDEF) {
        if(!arr[u].exists) {exists=false; break;}
        if(!prependRoute(&sequence, arr[u].routeEdge)) {exists=false; break;}
        arr[u].routeEdge=NULL;
        u=arr[u].prev;
    }
    if(!exists){
        freeRouteList(sequence);
        return false;
    }
    return sequence;
}
/**
 * @brief Delete first Node of belong Node
 * @param **p belongNode double pointer
 */
void deleteFirstBelongNode(struct belongNode** p) {
    struct belongNode* delete=*p;
    *p=delete->next;
    free(delete);
}
/**
 * @brief Add route to belong node
 * @param *p routeList pointer
 * @param routeId routeId
 * @return Whether or not function was executed affirmatively or negatively
 */
bool addRouteId(struct routeList* p, int routeId ) {
    if(!p) {return false;}
    struct routeList* tmp=p;
    while(tmp) {
        if(!prependBelongs(routeId,tmp->edge->belongs)) {
            while(p!=tmp) {
                deleteFirstBelongNode(tmp->edge->belongs);
                p=p->next;
            }
            return false;
        }
        tmp=tmp->next;
    }
    return true;
}
/**
 * @brief search city in route
 * @param *m Map pointer
 * @param routeId routeId
 * @param extension extension city
 * @return Whether or not city is in route
 */
bool searchInRoute(Map* m, unsigned routeId, int extension) {
    if(searchCity(m,m->routes[routeId].startCity)==extension) return true;
    struct routeList* p=*m->routes[routeId].routeList;
    while(p) {
        if(p->edge->isIn==extension){return true;}
        p=p->next;
    }
    return false;
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {
    if (!map || !correctName(city) || !correctRouteId(routeId)) {return false;}
    int extension=searchCity(map,city);
    if(extension==MISSING) {return false;}
    struct arr* arr =map->arr;
    if(searchInRoute(map,routeId,extension)) {return false;}
    const char* startCity=map->routes[routeId].startCity;
    int C1 = searchCity(map,startCity);
    unsigned long long length1;
    unsigned long long length2;
    int oldest1;
    int oldest2;
    unbanAll(map);
    // Ban Route without endpoints
    struct routeList* p =*map->routes[routeId].routeList;
    if(!p) {return false;}
    while(p->next) {
        arr[p->edge->isIn].banned=true;
        p=p->next;
    }
    int C2=p->edge->isIn;
    arr[C1].banned=false;
    arr[C2].banned=true;
    if(!dijkstra(map,extension,NULL,NULL)) {return false;}
    struct routeList* R1 = constructRoute(map,C1,&oldest1, &length1);
    arr[C1].banned=true;
    arr[C2].banned=false;
    if(!dijkstra(map,C2,NULL,NULL)) {return false;}
    struct routeList* R2 = constructRoute(map,extension,&oldest2, &length2);
    struct routeList* ans;
    if(R1 && R2) {
        if(length1==length2) {
            if(oldest1==oldest2) {
                freeRouteList(R1);
                freeRouteList(R2);
                return false;
            }

            ans=(oldest1>oldest2) ? R1 : R2;
            freeRouteList((oldest1>oldest2) ? R1 : R2);
        }
        else{
            ans=(length1<length2) ? R1:R2;
            freeRouteList((length1<length2) ? R2 : R1);
        }
    }
    else if (R1) {
        ans=R1;
    }
    else if (R2) {
        ans=R2;
    }
    else{
        return false;
    }


    addRouteId(ans, routeId);
    if(ans==R1) {
        prependRouteList(map->routes[routeId].routeList,ans);
        map->routes[routeId].startCity=city;
    }
    else if (ans==R2) {
        appendRouteList(map->routes[routeId].routeList,ans);
    }
    return true;

}
/**
 * @brief Toss every Id of routes that road C1;C2 is in
 * @param *m Map pointer
 * @param C1 City1 int
 * @param C2 City2 int
 * @param idArr[MAX_ROUTES] array to toss in
 * @param *isInExistingRoute boolean pointer to set to true if is in at
 * least one route
 */
void TossEveryIdForEdgeIntoArr(Map* m, int C1, int C2, bool idArr[MAX_ROUTES], bool* isInExistingRoute) {
    struct list* edge1=getAdjNode(m, C1, C2);
    struct list* edge2=getAdjNode(m, C2, C1);
    struct belongNode** belongs1=edge1->belongs;
    struct belongNode** belongs2=edge2->belongs;
    *isInExistingRoute= (*belongs1 || *belongs2) ? true:false;
    struct belongNode* p;
    p=*belongs1;
    for_each_node(p)
        idArr[p->id]=true;
    p=*belongs2;
    for_each_node(p)
        idArr[p->id]=true;
}
/**
 * @brief Delete Node of adjacency
 * @param *m Map pointer
 * @param C1 City int
 * @param *edge list pointer
 */
void deleteNode(Map* m, int C1, struct list* edge) {
    if(m->arr[C1].head==edge) {
        struct list* delete=m->arr[C1].head;
        m->arr[C1].head=delete->next;
        free(delete);
    }
    struct list* p=m->arr[C1].head;
    if(p)
    while(p->next) {
        if(p->next==edge) {
            struct list* delete=p->next;
            p->next=p->next->next;
            free(delete);
            break;
        }
        p=p->next;
    }
}
/**
 * @brief Remove edge between two citiec
 * @param *m Map pointer
 * @param C1 City1
 * @param C2 City2
 */
void removeEdge(Map* m, int C1, int C2) {
    struct list* edge1=getAdjNode(m, C1, C2);
    struct list* edge2=getAdjNode(m, C2, C1);
    struct belongNode** belongs1=edge1->belongs;
    struct belongNode** belongs2=edge2->belongs;
    freeBelongList(*belongs1);
    free(belongs1);
    freeBelongList(*belongs2);
    free(belongs2);
    deleteNode(m,C1,edge1);
    deleteNode(m,C2,edge2);

}
/**
 * @brief repairRoad() according to the specifications of the project
 * @param *start1 routeList pointer
 * @return Copy upon successful memory allocation
 */
struct routeList* copyRoute(struct routeList* start1) {
    struct routeList *start2=NULL,*previous=NULL;
    struct routeList *temp;
    while(start1!=NULL)
    {
        temp = (struct routeList*) malloc (sizeof(struct routeList));
        temp->edge=start1->edge;
        temp->next=NULL;

        if(start2==NULL)
        {
            start2=temp;
            previous=temp;
        }
        else
        {
            previous->next=temp;
            previous=temp;
        }
        start1=start1->next;
    }
    return start2;
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
    if(!map || !city1 || !city2) {return false;}
    if(!correctName(city1) || !correctName(city2)) {return false;}
    if(strcmp(city1,city2)==EQUAL){return false;}
    int C1= searchCity(map,city1);
    int C2= searchCity(map,city2);
    if(C1==MISSING || C2==MISSING ) {return false;}

    if(!haveRoad(map,city1,city2)) {return false;}
    struct arr* arr=map->arr;
    bool IdArr[MAX_ROUTES];
    int i;
    for(i=1;i<=MAX_ROUTES-1;i++) {IdArr[i]=false;}
    bool isInExistingRoute=false;
    TossEveryIdForEdgeIntoArr(map, C1, C2, IdArr, &isInExistingRoute);
    if(!isInExistingRoute){
        removeEdge(map,C1,C2);
        return true;
    }
    struct routeList* p;
    for(i=1;i<=MAX_ROUTES-1;i++) {
        if(!IdArr[i]) {continue;}
        arr[searchCity(map,map->routes[i].startCity)].banned=true;
        p=*map->routes[i].routeList;
        if(!p) {return false;}
        while(p) {
            arr[p->edge->isIn].banned=true;
            p=p->next;
        }
    }
    arr[C1].banned=false;
    arr[C2].banned=false;
    struct list* edge1=getAdjNode(map, C1, C2);
    struct list* edge2=getAdjNode(map, C2, C1);
    if(!dijkstra(map,C1,edge1,edge2)) {return false;}
    struct routeList* c1Toc2=constructRoute(map, C2, NULL, NULL);
    if(!dijkstra(map,C2,edge1,edge2)) {return false;}
    struct routeList* c2Toc1=constructRoute(map, C1, NULL, NULL);
    if(!c1Toc2 || !c2Toc1) {return false;}
    struct routeList* tmp;
    struct routeList* ans;
    struct routeList* delete;
    struct routeList* copy;
    for(i=1;i<=MAX_ROUTES-1;i++) {
        if(!IdArr[i]) {continue;}
        p=*map->routes[i].routeList;
        if(p->edge->from==C1 || p->edge->from==C2) {
            ans=(p->edge->from==C1) ? c1Toc2 : c2Toc1;
            delete=p;
            *map->routes[i].routeList=p->next;
            free(delete);
            copy=copyRoute(ans);
            if(!copy) {return false;}
            prependRouteList(map->routes[i].routeList,copy);
        }
        else while(p->next) {
            if(p->next->edge->from==C1 || p->next->edge->from==C2) {
                ans=(p->next->edge->from==C1) ? c1Toc2 : c2Toc1;
                delete=p->next;
                p->next=p->next->next;
                free(delete);
                copy=copyRoute(ans);
                if(!copy) {return false;}

                tmp=copy;
                while(tmp->next) {tmp=tmp->next;}
                tmp->next=p->next;
                p->next=copy;
                break;
            }
            p=p->next;
        }

    }

    freeRouteList(c1Toc2);
    freeRouteList(c2Toc1);
    removeEdge(map,C1,C2);
    return true;
}

char const* getRouteDescription(Map *map, unsigned routeId) {
    if (!map) {return NULL;}
    if (!correctRouteId(routeId) || !map->routes[routeId].startCity) {
        char* s=(char*)malloc(sizeof(char));
        *s='\0';
        return s;
    }
    int counter=0;
    counter+=digs(routeId); // routeId
    counter++; // ;
    counter+=strlen(map->routes[routeId].startCity); // First City

    struct routeList* p=*map->routes[routeId].routeList;
    while(p) {
        counter++; // ;
        counter+= digs(p->edge->length); // length
        counter++; // ;
        counter+= (p->edge->builtYear<0) ? 1:0; // -
        counter+= digs(p->edge->builtYear); // builtYear
        counter++; // ;
        counter+=strlen(p->edge->city); // City name
        p=p->next;
    }
    counter++; // '\0'

    char* s=(char*)malloc(sizeof(char)*counter);
    if(!s) {return NULL;}

    char routeChar[MAX_ROUTEID_DIGITS+1];
    char lengthChar[MAX_LENGTH_DIGITS+1];
    char yearChar[MAX_YEAR_DIGITS_WITH_MINUS+1];
    sprintf(routeChar, "%d", routeId);
    *s=0;
    strcat(s, routeChar);
    strcat(s, ";");
    strcat(s,map->routes[routeId].startCity); // First City

    p=*map->routes[routeId].routeList;
    while(p) {
        strcat(s, ";");
        sprintf(lengthChar, "%d", p->edge->length);
        strcat(s, lengthChar);
        strcat(s, ";");
        sprintf(yearChar, "%d", p->edge->builtYear);
        strcat(s, yearChar);
        strcat(s, ";");
        strcat(s, p->edge->city); // City name
        p=p->next;
    }
    strcat(s,"\0");
    return s;
}
/**
 * @brief Delete a Belong Node
 * @param **b belongNode double pointer
 * @param reouteId routeId
 * @return Whether or not function was executed affirmatively or negatively
 */
bool deleteBelongNode(struct belongNode** b, unsigned routeId) {
    if(!b || !*b) {return false;}
    if(routeId==(unsigned)(*b)->id) {deleteFirstBelongNode(b); return true;}
    struct belongNode* ptr=*b;
    if(!ptr) {return false;}
    while(ptr->next) {
        if(routeId==(unsigned)ptr->next->id) {
            struct belongNode* delete=ptr->next;
            ptr->next=delete->next;
            free(delete);
        }
        ptr=ptr->next;
    }
    return true;
}

bool removeRoute(Map *map, unsigned routeId) {
    if(!map) {return false;}
    if (!correctRouteId(routeId) || !map->routes[routeId].startCity)
        return false;


    struct routeList* p=*map->routes[routeId].routeList;
    for_each_node(p)
        deleteBelongNode(p->edge->belongs, routeId);
    freeRouteList(*(map->routes[routeId].routeList));
    *map->routes[routeId].routeList=NULL;
    map->routes[routeId].startCity=NULL;
    return true;
}

void deleteMap(struct Map* m) {
    int count=m->count;
    int i;
    struct arr* arr=m->arr;
    for(i=0;i<=count-1;i++) {
        if(arr){
            free((void*)arr[i].city);
            freeList(arr[i].head);
            arr[i].head=NULL;
        }
    }
    for(i=0;i<=MAX_ROUTES-1;i++) {
        freeRouteList(*(m->routes[i].routeList));
        free(m->routes[i].routeList);
    }
    if(arr) {free(arr);}
    ht_del_hash_table(m->ht);
    free(m);
}



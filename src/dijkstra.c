#include "dijkstra.h"

#include "queue.h"
#include "define.h"
#include <limits.h>
/**
 * @brief min() function
 * @param a First number
 * @param b Second number
 * @return Smaller of two numbers
 */
int min(int a, int b) {
    return (a<b) ? a:b;
}



bool dijkstra(Map* m, int start,struct list* ban1,  struct list* ban2) {
    struct arr* arr=m->arr;
    arr[start].dist=0;
    QNode* Q=NULL;
    int i;
    for(i=0;i<=(m->count)-1;i++) {
        if(i!=start){
            arr[i].dist=ULLONG_MAX;
        }
        arr[i].oldest=INT_MAX;
        arr[i].prev=UNDEF;
        arr[i].exists=false;
        arr[i].multiple=false;
        push(&Q, i, arr[i].dist);
        if(!Q) {return false;}
    }
    int oldestWithLast,u,x;
    while(!isEmpty(&Q)) {
        u=pop(&Q);
        if(arr[u].banned) {continue;}
        unsigned long long alt;
        for_each_neighbor(X, arr[u]) { // for each neighbor x of u
            x=X->isIn;
            if(arr[x].banned || arr[u].dist==ULLONG_MAX || X==ban1 || X==ban2) {continue;}
            alt=arr[u].dist + length(u,X);  //X->length; alt = dist[u] + length(u, x)
            if(alt==arr[x].dist) {
                oldestWithLast=min(X->builtYear, arr[u].oldest); // Oldest including the last road
                arr[x].multiple=true;
                if(arr[x].oldest==oldestWithLast)
                    arr[x].exists=false;
                else if(oldestWithLast > arr[x].oldest) {
                    arr[x].prev=u;
                    arr[x].routeEdge=X;
                    //arr[x].oldest=oldestWithLast; // Update oldest if needed
                    arr[x].exists=true;
                }
            }
            else if(alt<arr[x].dist) {
                if(arr[x].multiple && X->builtYear<=arr[u].oldest) {
                    arr[x].exists=false;
                    continue;
                }
                arr[x].dist=alt;
                arr[x].prev=u;
                arr[x].routeEdge=X;
                arr[x].oldest=min(X->builtYear,arr[u].oldest); // Update oldest if needed
                arr[x].exists=true;
                decrease(&Q,x,alt);
            }
        }
    }
    return true;
}

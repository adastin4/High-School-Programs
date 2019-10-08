#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

static size_t N = 5;
static unsigned int MAX = 100;

unsigned int **createGraph(size_t n, unsigned int max);
void printGraph(unsigned int **graph, size_t n, unsigned int max);
size_t *dijkstra(unsigned int **graph, size_t n, size_t source);
unsigned int *getLengths(unsigned int **graph, size_t *paths, size_t n);
size_t *shortestPath(unsigned int **graph, size_t n, size_t source, size_t dest);
unsigned int getDistance(unsigned int **graph, size_t *path, size_t n);
bool isEmpty(bool array[], size_t size);
size_t nodeOfMostEdges(unsigned int **graph, size_t n, size_t source);

int main(void){
    srand(42);
    puts("graph: ");
    unsigned int ** graph = createGraph(N, MAX);
    printGraph(graph, N, MAX);
    puts("\ndijkstra: ");
    size_t *paths = dijkstra(graph, N, 0);
    for(size_t i = 0; i<N; ++i){
        printf("%zu ", paths[i]);
    }
    puts("\nlengths: ");
    unsigned int *lengths = getLengths(graph, paths, N);
    for(size_t i = 0; i < N; ++i){
        printf("%u ", lengths[i]);
    }
    puts("\nshortest path: ");
    size_t *path = shortestPath(graph, N, 0, 1);
    for(size_t i = 0; i < N && path[i] != (size_t)-1;++i){
        printf("%zu ", path[i]);
    }
    puts("\ndistance of path: ");
    printf("%u\n", getDistance(graph,path,N));
    puts("node with the most edges: ");
    size_t edges = nodeOfMostEdges(graph, N, 0);
    printf("%zu\n", edges);
    
    return 0;
}

unsigned int **createGraph(size_t n, unsigned int max){
    double chance = 0.2; //1.0/(max+1);
    unsigned int weight;
    unsigned int **graph = malloc(sizeof(unsigned int *) * n);
    for(size_t i = 0; i < n; ++i){
        graph[i] = calloc(n, sizeof(unsigned int));
        for(size_t j = 0; j < n; ++j){
            if(j != i){
                weight = rand();
                if(weight/(double)RAND_MAX < chance)
                    graph[i][j] = -1;
                else    
                    graph[i][j] = 1 + rand()% max;
            }
        }
    }
    return graph;
}

void printGraph(unsigned int **graph, size_t n, unsigned int max){
    unsigned int width = (unsigned int)log10(max)+1;
    for(size_t i = 0; i < n; ++i){
        for(size_t j = 0; j < n; ++j){
            if(graph[i][j] !=(unsigned int)  -1)
                printf("%*u ",width, graph[i][j]);
            else
                printf("%*c ",width, '~');
        }
        puts("");
    }
}

size_t *dijkstra(unsigned int **graph, size_t n, size_t source){
    size_t *previous = malloc(sizeof(size_t) * n);
    previous[source] = source;    
    
    unsigned int distances[n];
    memset(distances, (unsigned int)-1, n*sizeof(unsigned int));
    distances[source] = 0;

    bool unvisited[n];
    memset(unvisited, true, n*sizeof(bool));

    size_t current = source;
    size_t i;
    unsigned int minDist, currDist, iter = 0;

    while(!isEmpty(unvisited, n) && iter++ < n){
        for(i = 0, minDist = (unsigned int) -1; i<n; ++i){
            if(unvisited[i] && distances[i] < minDist){
                current = i;
                minDist = distances[i];
            }
        }
        for(i = 0; i<n; ++i){
            currDist = graph[current][i];
            if(i != current && currDist < (unsigned int)-1 
            && currDist + distances[current] < distances[i]){
                distances[i] = currDist + distances[current];
                previous[i] = current;
            }
        }
        unvisited[current] = false;    
    }
    for( i = 0; i<n; ++i){
        if(unvisited[i])
            previous[i] = (size_t)(-1);
    }

    return previous;
}

unsigned int *getLengths(unsigned int **graph, size_t *paths, size_t n){
    unsigned int *lengths = calloc(n, sizeof(unsigned int));
    for(size_t i = 0, curr, prev; i < n; ++i){
        curr = i;
        prev = paths[i];
        while(curr != prev){
            lengths[i] += graph[prev][curr];
            curr = prev;
            prev = paths[prev];
        }
    }
    return lengths;
}

bool isEmpty(bool array[], size_t n){
    for(size_t i = 0; i<n; ++i){
        if(array[i] == true)
           return false;
    }
    return true;
}

size_t *shortestPath(unsigned int **graph, size_t n, size_t source, size_t dest){ 
    size_t previous[n];
    previous[source] = source;    
    
    unsigned int distances[n];
    memset(distances, (unsigned int)-1, n*sizeof(unsigned int));
    distances[source] = 0;

    bool unvisited[n];
    memset(unvisited, true, n*sizeof(bool));

    size_t current = source;
    unsigned int minDist, currDist;

    while(unvisited[dest]){
        for(size_t i = 0, minDist = (unsigned int)(-1); i<n; ++i){
            if(unvisited[i] && distances[i] < minDist){
                minDist = distances[i];
                current = i;
            }
        }
        for(size_t i = 0; i<n; ++i){
            currDist = graph[current][i];
            if(i != current && currDist < (unsigned int)(-1) 
            && currDist + distances[current] < distances[i]){
                distances[i] = currDist + distances[current];
                previous[i] = current;
            }
        }
        unvisited[current] = false;    
    }

    size_t *path = malloc(n * sizeof(size_t));
    //memset(path, (size_t)-1, n*sizeof(size_t));

    size_t curr = current, prev = previous[dest], index;
    for(index = n-1; curr != prev; --index){
        path[index] = curr;
        curr = prev;
        prev = previous[prev];
    }
    path[index] = curr;

    for(size_t i = index; i < n; ++i){
        path[i-index] = path[i];
    }

 //   for(i = n-index; i<n; ++i){
   //     path[i] = (size_t)-1;
   // }  
    
    path[n-index] = (size_t) - 1;

    return path;
}

unsigned int getDistance(unsigned int **graph, size_t *path, size_t n){
    unsigned int dist = 0;

    for(size_t i = 1; path[i] != (size_t) -1 && i < n; ++i){
        dist+= graph[path[i-1]][path[i]];
    }

    return dist;
}

size_t nodeOfMostEdges(unsigned int **graph, size_t n, size_t source){
    unsigned int maxEdges = 0, count;
    size_t out;
    size_t* path;
    for(size_t i = 0; i < n; ++i){
        path = shortestPath(graph, n, source, i);
        count = 0;
        for(size_t j = 0; path[j] != (size_t)-1 && j<N; ++j){
            count++;
        }
        if(maxEdges < count){
            maxEdges = count;
            out = i;
        }
    }
    return out;
}

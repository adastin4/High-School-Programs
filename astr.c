#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

enum Type{INT, CHAR};

size_t ROWS = 10;
size_t COL = 10;
int CHANCE = 20;


typedef union data{
    unsigned int i;
    char c;
} Data;

typedef struct tile{
    enum Type t;
    Data d;
} Tile;

typedef struct coord{
    size_t row;
    size_t col;
    unsigned int count;
} Coord;

typedef struct node{
    struct node *next;
    unsigned int f;
    Coord c;
}Node;

Tile** generateGrid(size_t rows, size_t cols, int obChance);
Coord* getPath(Tile ** grid, Coord source, Coord dest, size_t rows, size_t cols);
void printGrid (Tile** grid,  size_t rows, size_t cols);
void resetGrid(Tile** grid, size_t rows, size_t cols);
unsigned int snake(Coord a, Coord b);
void insert (Node **pqtr, Node  *new);
Node* pop (Node **pqptr);
void update (Node **pqptr, unsigned int gNew, size_t row, size_t col, Coord dst);
Coord *Astar(Tile **grid, Coord src, Coord dst, size_t rows, size_t cols);


int main(void){
    srand(42);

    Tile** grid = generateGrid(ROWS, COL,CHANCE);
    printGrid(grid, ROWS, COL);
    
    Coord src = {0, 3, 0};
    Coord dst = {9, 2, 0};
    
    Coord *path = getPath(grid, src, dst, ROWS, COL);
    puts("Grid:");
    printGrid(grid, ROWS, COL);

    puts("Path:");
    for(size_t i = 0; path[i-1].row != dst.row || path[i-1].col != dst.col; ++i){
        printf("%zu, %zu, %u\n", path[i].row, path[i].col, path[i].count);
    }
    puts("");
    path = Astar (grid, src, dst, ROWS, COL);
    puts("AStar grid:");
    printGrid(grid, ROWS, COL);
    return 0;
}

Tile** generateGrid(size_t rows, size_t cols, int obchance){
    Tile** grid = malloc(sizeof(Tile*)*rows);
    Tile* row;
    for(size_t r = 0, c; r < rows; ++r){
       row = malloc(sizeof(Tile) * (cols));
        for(c = 0; c<cols; ++c){
            row[c].t = CHAR;
            if(rand()%100 < obchance){
                row[c].d.c = 'X';
            }
            else{
                row[c].d.c = '-';
            }
        }
        grid[r] = row;
    }
    return grid;
}

Coord* getPath(Tile** grid, Coord source, Coord dest, size_t rows, size_t cols){
    resetGrid(grid, rows, cols);
    grid[source.row][source.col].d.c = 'S';
    grid[dest.row][dest.col].d.c = 'D';
    
    Coord list[rows * cols];
    list[0] = dest;
    unsigned int count;
    
    for(size_t current = 0, tail = 1, r, c; tail <= rows*cols; ++current){
        r = list[current].row;
        c = list[current].col;
        count = list[current].count+1;

        if(r > 0 && grid[r-1][c].t != INT && grid[r-1][c].d.c != 'X'&& grid[r-1][c].d.c!= 'D'){
        if(r > 0 && grid[r-1][c].t != INT && grid[r-1][c].d.c != 'X'&& grid[r-1][c].d.c!= 'D'){
            if(source.row == r-1 && source.col == c){
                break;
            }   
            list[tail++] = (Coord){r-1, c, count}; 
            grid[r-1][c].t = INT;
            grid[r-1][c].d.i = count;
        }
        if(c > 0 && grid[r][c-1].t != INT && grid[r][c-1].d.c != 'X' && grid[r][c-1].d.c!= 'D'){//r c+1 //r c-1
            if(source.row == r && source.col == c-1){
                break;
            }   
            list[tail++] = (Coord) {r, c-1, count};
            grid[r][c-1].t = INT;
            grid[r][c-1].d.i = count;
        }
        if(r < rows-1 && grid[r+1][c].t != INT && grid[r+1][c].d.c != 'X' && grid[r+1][c].d.c!= 'D'){ //r+1 c
            if(source.row == r+1 && source.col == c){
                break;
            }   
            list[tail++] = (Coord){r+1, c, count};
            grid[r+1][c].t = INT;
            grid[r+1][c].d.i = count;
        }
        if(c < cols-1 && grid[r][c+1].t != INT  && grid[r][c+1].d.c != 'X' && grid[r][c+1].d.c!= 'D'){
            if(source.row == r && source.col == c+1){
                break;
            }   
            list[tail++] = (Coord) {r, c+1, count};
            grid[r][c+1].t = INT;
            grid[r][c+1].d.i = count;
        }
            if(source.row == r-1 && source.col == c){
                break;
            }   
            list[tail++] = (Coord){r-1, c, count}; 
            grid[r-1][c].t = INT;
            grid[r-1][c].d.i = count;
        }
        if(c > 0 && grid[r][c-1].t != INT && grid[r][c-1].d.c != 'X' && grid[r][c-1].d.c!= 'D'){//r c+1 //r c-1
            if(source.row == r && source.col == c-1){
                break;
            }   
            list[tail++] = (Coord) {r, c-1, count};
            grid[r][c-1].t = INT;
            grid[r][c-1].d.i = count;
        }
        if(r < rows-1 && grid[r+1][c].t != INT && grid[r+1][c].d.c != 'X' && grid[r+1][c].d.c!= 'D'){ //r+1 c
            if(source.row == r+1 && source.col == c){
                break;
            }   
            list[tail++] = (Coord){r+1, c, count};
            grid[r+1][c].t = INT;
            grid[r+1][c].d.i = count;
        }
        if(c < cols-1 && grid[r][c+1].t != INT  && grid[r][c+1].d.c != 'X' && grid[r][c+1].d.c!= 'D'){
            if(source.row == r && source.col == c+1){
                break;
            }   
            list[tail++] = (Coord) {r, c+1, count};
            grid[r][c+1].t = INT;
            grid[r][c+1].d.i = count;
        }
    }

    Coord *path = malloc(sizeof(Coord)* count+1);
    Coord curr = source;
    path[0] = curr;
    for(size_t i = count-1, r, c; ; --i){
        r = curr.row;
        c = curr.col;
        if(r > 0 && (grid[r-1][c].t == INT || grid[r-1][c].d.c == 'D')){
            if(grid[r-1][c].t == CHAR && grid[r-1][c].d.c == 'D'){
                path[count] = dest;
                break;

            }
            if(grid[r-1][c].d.i == i){
                curr = (Coord) {r-1, c, i};
                path[count-i] = curr;
                continue;
            }
        }

        if(c < cols-1 && (grid[r][c+1].t == INT || grid[r][c+1].d.c == 'D')){
            if(grid[r][c+1].t == CHAR && grid[r][c+1].d.c == 'D'){
                path[count] = dest;
                break;

            }
            if(grid[r][c+1].d.i == i){
                curr = (Coord) {r, c+1, i};
                path[count-i] = curr;
                continue;
            }
        }

        if(r < rows-1 && (grid[r+1][c].t == INT || grid[r+1][c].d.c == 'D')){
            if(grid[r+1][c].t == CHAR && grid[r+1][c].d.c == 'D'){
                path[count] = dest;
                break;

            }
            if(grid[r+1][c].d.i == i){
                curr = (Coord) {r+1, c, i};
                path[count-i] = curr;
                continue;
            }
        }

        if(c > 0 && (grid[r][c-1].t == INT || grid[r][c-1].d.c == 'D')){
            if(grid[r][c-1].t == CHAR && grid[r][c-1].d.c == 'D'){
                path[count] = dest;
                break;

            }
            if(grid[r][c-1].d.i == i){
                curr = (Coord) {r, c-1, i};
                path[count-i] = curr;
                continue;
            }
        }
    }

    return path;
}

void printGrid (Tile** grid, size_t rows, size_t cols){
    unsigned int maxint = 1, maxWidth;
    size_t r, c;
    for(r = 0; r< rows; ++r){
        for(c = 0; c<cols; ++c){
            if(grid[r][c].t == INT && (grid[r][c].d.i)>maxint){
               maxint = grid[r][c].d.i; 
            }
        }
    }
    maxWidth = (unsigned int) log10(maxint) + 2;

    for(size_t r = 0; r < rows; ++r){
        for(size_t c = 0; c< cols; ++c){
            if(grid[r][c].t == CHAR){
                printf("%*c ", maxWidth, grid[r][c].d.c);
            }
            else{
                printf("%*u ", maxWidth, grid[r][c].d.i);
            }
        }
        puts("");
    }
}

void resetGrid(Tile ** grid, size_t rows, size_t cols){
    for(size_t r = 0; r<rows; ++r){
        for(size_t c = 0; c<cols; ++c){
            if(grid[r][c].t == INT || grid[r][c].d.c != 'X'){
                grid[r][c].d.c = '-';
                grid[r][c].t = CHAR;
            }
        }
    }
}

unsigned int snake(Coord a, Coord b){
    return abs((int)(a.row - b.row)) + abs((int)(a.col - b.col));
}

Node* pop (Node **pqPtr){
    Node *out = *pqPtr;
    *pqPtr = out->next;
    return out;
}

void insert (Node **pqPtr, Node  *new){
    Node *prev = NULL, *curr = *pqPtr;
    while(curr != NULL && new->f > curr->f){
        prev = curr;
        curr = curr->next;
    }
    new->next = curr;
    if(prev == NULL){
        *pqPtr = new;
    }
    else{
        prev->next = new; 
    }
}

Coord *Astar(Tile **grid, Coord src, Coord dst, size_t rows, size_t cols){
    resetGrid(grid, rows, cols);
    grid[src.row][src.col].d.c = 'S';
    grid[dst.row][dst.col].d.c = 'D';
   
    unsigned int cnt = 0;
    src.count = 0;
    //Node* srcNode= {NULL, snake(src, dst), src};
    Node *pq, *head;
    pq = malloc(sizeof(Node));
    pq->next = NULL;
    pq->f = snake(src, dst);
    pq->c = src;
    size_t row, col;

    while (pq != NULL && ((head = pop(&pq))->c.row != dst.row || head->c.col != dst.col)){
        row = head->c.row;
        col = head->c.col;
        grid[row][col].t = INT;
        grid[row][col].d.i = head->c.count;
    
        if(row > 0 && grid[row-1][col].t == CHAR && grid[row-1][col].d.c != 'X'){
            update(&pq, head->c.count+1, row-1, col, dst);
        }
        if(col > 0 && grid[row][col-1].t == CHAR && grid[row][col-1].d.c != 'X'){
            update(&pq, head->c.count+1, row, col-1, dst);
        }
        if(row < rows-1 && grid[row+1][col].t == CHAR && grid[row+1][col].d.c != 'X'){
            update(&pq, head->c.count+1, row+1, col, dst);
        }
        if(col < cols-1 && grid[row][col+1].t == CHAR && grid[row][col+1].d.c != 'X'){
            update(&pq, head->c.count+1, row, col+1, dst);
        }
        
        if(cnt < head->c.count){
            cnt = head->c.count;
        }
        free(head);
    }
    cnt++;
    Coord *path = malloc(sizeof(Coord) * (cnt+1));
    Coord curr = dst;
    path[cnt] = curr;
    path[0] = src;
    for(size_t i = cnt-1; i>0; --i){ 
        size_t r = curr.row;
        size_t c = curr.col;
        if(r > 0 && (grid[r-1][c].t == INT)){
            if(grid[r-1][c].d.i == i){
                curr = (Coord) {r-1, c, i};
                path[i] = curr;
                continue;
            }
        }

        if(c < cols-1 && (grid[r][c+1].t == INT)){
            if(grid[r][c+1].d.i == i){
                curr = (Coord) {r, c+1, i};
                path[i] = curr;
                continue;
            }
        }

        if(r < rows-1 && (grid[r+1][c].t == INT)){
            if(grid[r+1][c].d.i == i){
                curr = (Coord) {r+1, c, i};
                path[i] = curr;
                continue;
            }
        }

        if(c > 0 && (grid[r][c-1].t == INT)){
            if(grid[r][c-1].d.i == i){
                curr = (Coord) {r, c-1, i};
                path[i] = curr;
                continue;
            }
        }
    }

    puts("AStar Path:");
    for(size_t i = 0; path[i-1].row != dst.row || path[i-1].col != dst.col; ++i){
        printf("%zu, %zu, %u\n", path[i].row, path[i].col, path[i].count);
    }
    // for ( i = pathsize; i>0; i--)
    // For each possible condition
    //if == i path[i] get it continue
    //curr = it
    return NULL;
}

void update (Node **pqPtr, unsigned int gNew, size_t row, size_t col, Coord dst){
    Node *prev = NULL, *curr = *pqPtr;
    while (curr != NULL && (curr->c.row != row || curr->c.col != col)){
        prev = curr; 
        curr = curr->next;
    }
    if(curr == NULL){
        curr = malloc(sizeof(Node));
        curr->c = (Coord){row, col, gNew};
        curr->next = NULL;
        curr->f = gNew + snake(curr->c, dst);
        insert(pqPtr, curr);
        
    }
    else if(curr->c.count > gNew){
        curr->f = curr->f - (curr->c.count - gNew);
        curr->c.count = gNew;
        if(prev != NULL){
            prev->next = curr->next;
            insert(pqPtr, curr);
        }
    }
    else{
        return;
    }
}
// finish getting path on your own 

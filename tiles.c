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

Tile** generateGrid(size_t rows, size_t cols, int obChance);
Coord* getPath(Tile ** grid, Coord source, Coord dest, size_t rows, size_t cols);
void printGrid (Tile** grid,  size_t rows, size_t cols);
void resetGrid(Tile ** grid, size_t rows, size_t cols);

int main(void){
    srand(42);

    Tile** grid = generateGrid(ROWS, COL, CHANCE);
    printGrid(grid, ROWS, COL);
    
    Coord src = {0, 3, 0};
    Coord dst = {9, 2, 0};
    
    Coord *path = getPath(grid, src, dst, ROWS, COL);
    printGrid(grid, ROWS, COL);

    for(size_t i = 0; path[i-1].row != dst.row || path[i-1].col != dst.col; ++i){
        printf("%zu, %zu, %u\n", path[i].row, path[i].col, path[i].count);
    } 
    return 0;
}

Tile** generateGrid(size_t rows, size_t cols, int obChance){
    Tile** grid = malloc(sizeof(Tile*)*rows);
    Tile* row;
    for(size_t r = 0, c; r < rows; ++r){
       row = malloc(sizeof(Tile) * (cols));
        for(c = 0; c<cols; ++c){
            row[c].t = CHAR;
            if(rand()%100 < obChance){
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
    unsigned int maxInt = 1;
    size_t r, c;
    for(r = 0; r< rows; ++r){
        for(c = 0; c<cols; ++c){
            if(grid[r][c].t == INT && (grid[r][c].d.i)>maxInt){
               maxInt = grid[r][c].d.i; 
            }
        }
    }
    unsigned int maxWidth = (unsigned int)log10(maxInt) + 1;

    for(size_t r = 0; r < rows; ++r){
        for(size_t c = 0; c< cols; ++c){
            if(grid[r][c].t == CHAR){
                printf("%*c ", maxWidth, grid[r][c].d.c);
            }
            else{
                printf("%*u ", maxWidth, grid[r][c].d.i);
            }
        }
        puts("\n");
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

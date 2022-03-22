// macros

#define ROWMAX      100             // maximum lenght of SVG file row
#define COLCOUNT    10              // SVG file max columns
#define NAMELEN     30              // max length for string inputs
#define MAZEMIN     5               // min size for a maze
#define MAZEMAX     100000          // max size for a maze
#define BILLION     1000000000.0    // for keeping time on generation algorithms

// holding mazes

typedef struct data{    // keeping an individual maze
    short **maze;       // maze matrix
}data_t;

typedef struct dataArr{ // array of mazes for different algos
    short exits[4];     // mark entrance and exit coordinates
    int size;           // maze size
    int algoCount;      // the number of solving algorithms + 2 for generation and final solution 
    data_t *algo;       // array of maze matrixes
}maze_t;

// memory management

void freeMemory(maze_t *M); // free memory of maze_t

void feedMemory(maze_t *M); // give memory for maze_t according to M->size

// maze generation

void Ellermaze(int size, short **maze);                                 // maze generation using Eller's algorithm

void treemaze(int size, short **maze, short algo, short algoloop);      // treealgorithm

void addBorders(short **cells, int size);                               // adds borders for maze generation 

// maze solving

void findExits(int size, short **maze, short exits[4]);                 // finding the entrance and exit

double deadEnd(int size, short **maze, short exits[4]);                 // using the dead end filler algorithm

double recursion(int size, short **raw, short **sol, short exits[4]);   // recursive backtracker

double bfs(int size, short **raw, short **sol, short exits[4]);         // breath first search

// managing files

//void makeSVG(char *filename, int size, short **maze);                 // making the SVG file

int readSVG(char *filename, maze_t *M);                                 // reading SVG files

int makeBMP(int height, short **maze, int mode, int present_elements);  // experimental, BMP file creation


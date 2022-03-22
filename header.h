#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// macros

#define ROWMAX          100             // maximum lenght of SVG file row
#define COLCOUNT        10              // SVG file max columns
#define NAMELEN         30              // max length for string inputs
#define MAZEMIN         5               // min size for a maze
#define MAZEMAX         100000          // max size for a maze
#define BILLION         1000000000.0    // for keeping time on generation algorithms
#define MENUOPTION      10              // standard length for menu options
#define COLORCOUNT      14              // currently supported bitmap colors
#define BMPTARGETSIZE   3000            // target bitmap width in pixels

enum algoArr{generated, deadend, recursive, breath_first, final_maze};  // the same order the mazes are in the struct array
enum bitmap_mode{regular, secret};                                      // for BMP file creation color menu
enum element_type{wall, path, rec_path, bfs_path, crossover};                     // element type, used in BMP and SVG file creation


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
enum file_complexity{automatic, manual, ask_user};                                //  int complexity in manageFiles
enum file_mode{read, write};                                            //  int mode in manageFiles
enum file_type{txt_file, svg_file, bmp_file};                           /*  the int filetype in manageFiles, 
                                                                            makes for easier function calling */                                                                       
int manageFiles(int mode, int filetype, int complexity, int mazetype, maze_t *M);                                

FILE *getFilename(int mode, int filetype, int complexity, int mazetype);

int sizeCheck(int size);                                                // to check if maze size is allowed

int writeSVG(FILE *svg, maze_t *M, int mazetype);                       // making the SVG file

int readSVG(FILE *svg, maze_t *M);                                      // reading SVG files

int makeBMP(FILE *f, int mode, int mazetype, maze_t *M);                         // experimental, BMP file creation


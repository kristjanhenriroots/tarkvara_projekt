#ifndef _CAPP_H_
#define _CAPP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <QMessageBox>
#include "cconsole.h"

// Application ID..
#define APP_NAME "Labyrinth generator & resolver"
#define APP_VER_MAJOR 0
#define APP_VER_MINOR 4
#define APP_VER_BUILD 20220428

// macros
#define ROWMAX          100             // maximum lenght of SVG file row
#define COLCOUNT        10              // SVG file max columns
#define NAMELEN         30              // max length for string inputs
#define MAZEMIN         5               // min size for a maze
#define MAZEMAX         99999	        // max size for a maze
#define BILLION         1000000000.0    // for keeping time on generation algorithms
#define MENUOPTION      10              // standard length for menu options
#define COLORCOUNT      14              // currently supported bitmap colors
#define BMPTARGETSIZE   3000            // target bitmap width in pixels

typedef struct COLOR_RGB
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
}COLOR_RGB_T, *P_COLOR_RGB;

enum genalgo
{
	null,
	eller,
	tree,
	prim
};

enum algoArr // the same order the mazes are in the struct array
{
	generated,
	deadend,
	recursive,
	breath_first,
	final_maze
};

enum bitmap_mode // for BMP file creation color menu
{
	regular,
	secret,
	insanity
};

enum element_type // element type, used in BMP and SVG file creation
{
	wall,
	path,
	rec_path,
	bfs_path,
	crossover,
	fade_start,
	fade_end,
	element_n
};

// managing files
enum file_complexity //  int complexity in manageFiles
{
	automatic,
	manual,
	ask_user
};

enum file_mode //  int mode in manageFiles
{
	read,
	write
};

enum file_type //  the int filetype in manageFiles,
{
	txt_file,
	svg_file,
	bmp_file
};

// holding mazes
typedef struct data // keeping an individual maze
{
	short **maze; // maze matrix
}data_t;

typedef struct dataArr // array of mazes for different algos
{
	short exits[4]; // mark entrance and exit coordinates
	int size; // maze size
	int algoCount; // the number of solving algorithms + 2 for generation and final solution
	data_t *algo; // array of maze matrixes
}maze_t;

typedef struct //struct to number each cell
{
	int P1;
	int P2;
}cellsets_s;

// A structure to represent a queue
struct Queue
{
	int front;
	int rear;
	int size;
	unsigned capacity;
	int* array;
};

typedef struct needed_colors // holds the colors of all the maze elements
{
	double rgb[3];
}selected_t;

typedef struct color_fade // color fade values
{
	int size;                   // keeping maze size here
	int direction;              // horizontal or vertical
	double fade[3];             // individual RGB change values per row / column
	double original[3];         // keeping the original color in case of horizontal fades
}fade_t;


// bitmap file header (14 bytes)
typedef struct bitmap_file_header
{
	unsigned char   bitmap_type[2];     // 2 bytes
	int             file_size;          // 4 bytes
	short           reserved1;          // 2 bytes
	short           reserved2;          // 2 bytes
	unsigned int    offset_bits;        // 4 bytes
}bitmap_file_header_t;

// bitmap image header (40 bytes)
typedef struct bitmap_image_header
{
	unsigned int    size_header;        // 4 bytes
	unsigned int    width;              // 4 bytes
	unsigned int    height;             // 4 bytes
	short int       planes;             // 2 bytes
	short int       bit_count;          // 2 bytes
	unsigned int    compression;        // 4 bytes
	unsigned int    image_size;         // 4 bytes
	unsigned int    ppm_x;              // 4 bytes
	unsigned int    ppm_y;              // 4 bytes
	unsigned int    clr_used;           // 4 bytes
	unsigned int    clr_important;      // 4 bytes
}bitmap_image_header_t;

enum colors // all colors
{
	white,
	silver,
	gray,
	black,
	purple,
	magenta,
	blue,
	lime,
	green,
	cyan,
	aqua,
	red,
	yellow,
	maroon,
	random_list = 66,
	random_overall = 77,
	custom_RGB_code = 88,
	fade_cl = 99
};

enum fade_direction // fade direction
{
	none,
	horizontal,
	vertical
};


//------ Application class -------


class CApp
{
public:
	CApp();
	~CApp();

	void SetConsole(P_CConsole _pCConsole);
	int Generate(int _Mode, int _Algo, int lops, int _size, P_COLOR_RGB _ElmntClr);
	int Solve(int _Mode, P_COLOR_RGB _ElmntClr, int _FadeDir);
	int SetColor(P_COLOR_RGB _ElemntClr);
	int SaveTxt(char *_Filename, int mazetype);
	int LoadTxt(char *_Filename);
	int SaveSvg(char *_Filename, int mazetype);
	int LoadSvg(char *_Filename);
	int IsSolved(void);

private:
	P_CConsole pConsole;
	maze_t Maze;
	int moves;
	bitmap_file_header bfh;
	bitmap_image_header bih;
	bool SolveDisplayedFlag;

	// Added
	void Matrix2DShortAlloc(int _h, int _w, short ***_mtrx);
	void Matrix2DShortFree(int _h, short **_mtrx);
	// General
	void feedMemory(maze_t *M); // give memory for maze_t according to M->size
	void freeMemory(maze_t *M); // free memory of maze_t
	void mazeReset(maze_t *M);                                              // resetting a solved maze
	int getSize(maze_t *M);
	int generateMenu(maze_t *M);
	int solveMenu(maze_t *M);
	// ellermaze
    void generateEller(short **cells, short **cellsets, int size);			// maze generation
	void addBorders(short **cells, int size);                               // adds borders for maze generation
	void Ellermaze(int size, short **maze);                                 // maze generation using Eller's algorithm
	// treemaze
	void findneighbours(int P1, int P2, int *neighbours, int *neighbourcount, short **cells, int length);
	int searchforcell (short **cells, int length, int* P1, int* P2, int *neighbours, int *neighbourcount, cellsets_s *cellsets, int counter, int *visited);
	int searchforcell_backtracer (short** cells, int length, int* P1, int* P2, int *neighbours, int *neighbourcount, cellsets_s *cellsets, int counter);
	void generateTree(short **cells, cellsets_s *cellsets, int length, short algo, short algoback);
	void treemaze(int size, short **maze, short algo, short algoloop);      // treealgorithm
	// maze solving
	int findExits(int size, short **maze, short *exits);                 // finding the entrance and exit
	// Dead ends
	void eliminate(int size, short **maze, int x, int y);
	double deadEnd(int size, short **maze, short *exits);                 // using the dead end filler algorithm
	// Recursion
	int solvemaze(int y, int x, int endy, int endx, int size, short **maze, short **solution, short **helper);
	double recursion(int size, short **raw, short **sol, short *exits);   // recursive backtracker
	// bfs
	struct Queue* createQueue(unsigned capacity);
	int isFull(struct Queue* queue);
	int isEmpty(struct Queue* queue);
	void enqueue(struct Queue* queue, int item);
	int dequeue(struct Queue* queue);
	void findPath(int size, short **sol, short **adjacent, short **previous, short *exits);
	int solve(int size, short **raw, short **sol, short *exits);
	double bfs(int size, short **raw, short **sol, short *exits);         // breath first search
	// Manage files
	FILE *getFilename(int mode, int filetype, int complexity, int mazetype);
	int readTXT(FILE *f, maze_t *M);
	int writeTXT(FILE *f, maze_t *M, int mazetype);
	int breakIt(char *format[COLCOUNT], char *row);
	short findElement(char *input);
	int readSVG(FILE *svg, maze_t *M);                                      // reading SVG files
	int writeSVG(FILE *svg, maze_t *M, int mazetype);                       // making the SVG file
	int manageFiles(int mode, int filetype, int complexity, int mazetype, maze_t *M); // central hub for file management
	// BMP
	void set_color(double *to, short from[3]);
	void fade_calculate(short *to, short *from, fade_t *fade_arr, int element);
	short randomNr(int max, int count, short RGB[3]);
	int userselectRGB(double *input);
	void get_colors(double *input, int mode, P_COLOR_RGB _ElmntClr, int _FadeDir, int type, fade_t *fade_arr);
	int round4(int x);
	int makeBMP(FILE *f, int mode, P_COLOR_RGB _ElmntClr, int _FadeDir, int mazetype, maze_t *M);
	// experimental, BMP file creation
};
typedef CApp *P_CApp;

#endif // _CAPP_H_

//---------------

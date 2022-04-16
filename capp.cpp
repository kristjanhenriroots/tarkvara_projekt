
#include "capp.h"


CApp::CApp()
{
	pConsole = nullptr;
	Maze.size = 0;
	Maze.algoCount = 5; // current nr

    // Added.
	Maze.algo = (data_t*)malloc((Maze.algoCount * sizeof(data_t))); // Allcoate algo mazes array.
}

CApp::~CApp()
{
	// Cleanup.
	if(Maze.size > 0) // something is already generated
	{
		freeMemory(&Maze);
	}
	free(Maze.algo);
}

//removed MsgBox here.

void CApp::SetConsole(P_CConsole _pConsole)
{
	pConsole = _pConsole;
}

//man
int CApp::Generate(int _Algo, int _Lops, int _Size, P_COLOR_RGB _ElmntClr)
{
	struct timespec start, end; // keep accurate time
	double time_spent;

	if(Maze.size > 0) // something is already generated
	{
		pConsole->Print("Overwriting previous maze");
		freeMemory(&Maze);
	}

	Maze.size = _Size; // Set size.
	feedMemory(&Maze); // give sufficient memory

	if(_Algo == eller)
	{
		clock_gettime(CLOCK_REALTIME, &start); // start timer
		Ellermaze(Maze.size, Maze.algo[0].maze); // make a maze using Eller's algorithm
		clock_gettime(CLOCK_REALTIME, &end); // stop timer
	}
	else if(_Algo == tree || _Algo == prim)
	{
		clock_gettime(CLOCK_REALTIME, &start); // start timer
		treemaze(Maze.size, Maze.algo[0].maze, _Algo, _Lops); // make a maze using Auris Prääm's TM generation algorithm
		clock_gettime(CLOCK_REALTIME, &end); // stop timer
	}
	pConsole->Print("Generation successful");
	time_spent = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)) / BILLION; // calculate time spent in milliseconds

	pConsole->Print("Generation time %.4f ms", (time_spent * 1000));
	makeBMP(getFilename(write, bmp_file, automatic, generated), regular, _ElmntClr, none, generated, &Maze); // also make a BMP

	return 0;
}

//man2
int CApp::Solve(int _Mode, P_COLOR_RGB _ElmntClr, int _FadeDir)
{
	int i, j, k;
	double tdead, trec, tbfs; // solving times

	if(Maze.size == 0) // see if there actually is a maze to solve
	{
		pConsole->Print("No maze to solve!");
		return 0;
	}
	for(j = 0; j < Maze.size; j++)
	{
		for(k = 0; k < Maze.size; k++)
		{
			Maze.algo[1].maze[j][k] = Maze.algo[0].maze[j][k]; // copy the values for dead end filler, it will destroy the original
		}
	}

	if(findExits(Maze.size, Maze.algo[generated].maze, Maze.exits)) // does it have an exit?
	{
		// No exits.
		return 1;
	}

	tdead = deadEnd(Maze.size, Maze.algo[deadend].maze, Maze.exits);
	trec = recursion(Maze.size, Maze.algo[generated].maze, Maze.algo[recursive].maze, Maze.exits); // measuring time and solving
	tbfs = bfs(Maze.size, Maze.algo[generated].maze, Maze.algo[breath_first].maze, Maze.exits); // measuring time and solving

	pConsole->Print("Dead end solution %.4f ms", tdead * 1000);
	pConsole->Print("Recursive backtracker %.4f ms", trec * 1000);
	pConsole->Print("Breath first search %.4f ms", tbfs * 1000);

	for(i = 0; i < final_maze; i++)
	{
		if(i == 1) // don't want dead end filler showing
			continue;
		for(j = 0; j < Maze.size; j++)
			for(k = 0; k < Maze.size; k++)
				Maze.algo[final_maze].maze[j][k] +=  Maze.algo[i].maze[j][k]; // adding all layers up for the final solution
	}
	makeBMP(getFilename(write, bmp_file, automatic, final_maze), _Mode, _ElmntClr, _FadeDir, final_maze, &Maze); // also make a BMP

	/*
	for(i = 0; i < Maze.algoCount; i++){
			for(j = 0; j < Maze.size; j++){
					for(k = 0; k < Maze.size; k++)
							pConsole->Print("%hd ", Maze.algo[i].maze[j][k]); // to print everything if you so please
					printf("\n");
			}
	}
	*/

	pConsole->Print("Solving complete, see program directory for raw.bmp and solved.bmp\n");

	return 0;
}

//---------- extr, added

// Alocate 2D matrix of integers.
void CApp::Matrix2DShortAlloc(int _h, int _w, short ***_mtrx)
{
	int i;
	short **mtrx;

	mtrx = (short**)malloc((_h * sizeof(short*)));
	for(i = 0; i < _h; i++)
	{
		mtrx[i] = (short*)malloc((_w * sizeof(short)));
	}
	*_mtrx = mtrx;
}

// Free 2D matrix alocate by 'Matrix2DIntAlloc'.
void CApp::Matrix2DShortFree(int _h, short **_mtrx)
{
	int i;

	for(i = 0; i < _h; i++)
	{
		free(_mtrx[i]);
	}
	free(_mtrx);
}

//----
/*
 * General
 * main.c
 */

void CApp::feedMemory(maze_t *M) // allocate memory for mazes
{
	int i, j;

	for(i = 0; i < M->algoCount; i++)
	{
		M->algo[i].maze = (short**)calloc(M->size, (M->size) * sizeof(short*)); // feeding the behemoth memory
		for(j = 0; j < M->size; j++)
		{
			M->algo[i].maze[j] = (short*)calloc(M->size, (M->size) * sizeof(short));
		}
		/*
		*/
	}
	pConsole->Print("Memory allocated");
}

void CApp::freeMemory(maze_t *M)
{
	int i, j;

	// free the memory
	for(i = 0; i < M->algoCount; i++)
	{
		for(j = 0; j < M->size; j++)
		{
			free(M->algo[i].maze[j]);
		}
		free(M->algo[i].maze);
		/*
		*/
	}
	pConsole->Print("Memory freed");
	M->size = 0;
}

void CApp::mazeReset(maze_t *M)
{ // function to reset solved mazes, used when reading in already solved mazes
		for(int i = 0; i < M->size; i++){
				for(int j = 0; j < M->size; j++){
						if(M->algo[generated].maze[i][j] > path)
								M->algo[generated].maze[i][j] = path; // remove the solutions, just set them to 1 aka a path
						M->algo[final_maze].maze[i][j] = 0;       // reset the final solution maze
				}
		}
}

/*
 * Eller maze
 * ellermaze.c
 */

void CApp::generateEller(short **cells, short **cellsets, int size)
{
	int wallcounter = 0;
	int RandNr = 2;
	int setcounter = 0;
	int setbuf;
	int setcount;

	srand(time(NULL));
	for(int i = 1; i < size; i += 2) //loop for rows
	{
		for(int a = 1; a < size; a++) //Generate unique sets
		{
			if(cellsets[i][a] == 0)
			{
				cellsets[i][a] = RandNr++;
			}
		}

		for(int b = 1; b < size-2; b += 2)
		{
			if(i == size - 1) //unique algorithm for last row
			{
				if(cellsets[i][b] == cellsets[i][b+2]) //for last row, separate same sets
				{
					cells[i][b+1] = 0;
				}
				else
				{
					setbuf = cellsets[i][b+2];
					setcount = 1;
					while(setcount != size + 1) //go through the whole row and assign all sets with same set value to the same set value
					{
						if(cellsets[i][setcount] == setbuf)
						{
							cellsets[i][setcount] = cellsets[i][b]; //assign left set value to right and middle (middle not really necessary, good for debugging)
							setcount += 2;//increment to next set
						}
						else
						{
							setcount += 2; //increment to next set
						}
					}
				}
			}
			else if(cellsets[i][b] != cellsets[i][b+2] && cellsets[i][b+1] != 1) //If right cell is not equal to left then rand to combine or to not combine
			{
				if(rand() % 2) //if true, then make path between sets
				{
					setbuf = cellsets[i][b+2];
					setcount = 1;
					while(setcount != size + 1) //go through the whole row and assign all sets with right set value to left set value
					{
						if(cellsets[i][setcount] == setbuf)
						{
							cellsets[i][setcount] = cellsets[i][b]; //assign left set value to right and middle (middle not really necessary, good for debugging)
							setcount += 2;//increment to next set
						}
						else
						{
							setcount += 2;//increment to next set
						}
					}
				}
				else
				{
					cells[i][b+1] = 0; //if not true, add wall between sets
					cellsets[i][b+1] = 1;
				}
			}
			else
			{
				cells[i][b+1] = 0;
				cellsets[i][b+1] = 1; //if they are already equal then wall is needed (already done before, just in case)
			}
		}

		for(int c = 1; c < size; c += 2)
		{
			cells[i+1][c+1] = 0; //add walls between row and column sets for filler (because of matrix characteristics)
			cellsets[i+1][c+1] = 1;
			if(cellsets[i][c] == cellsets[i][c+2] && c != size - 1) //if sets are same. find whole set from row
			{
				setcounter += 2;
			}
			else
			{
				for(int d = 0; d <= setcounter; d += 2)
				{
					if(d == setcounter && wallcounter == setcounter)
					{ //if at the end of set and all previous floors set
						cellsets[i+2][c - setcounter + d] = cellsets[i][c - setcounter +d]; //at least 1 cell in set needs a path (no floor)
					}
					else if(rand() % 2)
					{
						cellsets[i+2][c - setcounter + d] = cellsets[i][c - setcounter +d]; //make path and assign above value to bottom set
					}
					else
					{
						cells[i+1][c - setcounter + d] = 0; //if not make path
						cellsets[i+1][c - setcounter + d] = 1;
						wallcounter += 2;
					}
				}
				setcounter = 0;
				wallcounter = 0;
			}
		}
	}
}

void CApp::addBorders( short **cells, int size)
{
	for(int b = 0; b < size; b++) //add left border
	{
		cells[b][0] = 0;
	}

	for(int a = 0; a < size; a++) //add top border
	{
		cells[0][a] = 0;
	}

	int a = rand() % (size - 2); //make entrance on top
	while(cells[1][a] == 0)
	{
		a++;
	}
	cells[0][a] = 1;

	for(int c = size-2; c >= 0; c--) //add bottom border
	{
		cells[size - 1][c] = 0;
	}

	int c = rand() % (size - 2); //make exit on bottom
	while(cells[size - 2][c] == 0)
	{
		c++;
	}
	cells[size - 1][c] = 1;

	for(int d = 0; d < size; d++) //add right border
	{
		cells[d][size - 1] = 0;
	}
}

void CApp::Ellermaze(int size, short **maze)
{
	int i, j;
	short **cellsets = (short**)calloc(size+4, sizeof(short*)); //create matrix for set identification
	for(i = 0; i < size + 4; i++)
	{
		cellsets[i] = (short*)calloc(size+4, sizeof(short));
	}

	for(i = 0; i < size; i++) //change maze matrix to all 1-s because algorithm will add walls to white canvas
	{
		for(j = 0; j < size; j++)
		{
			maze[i][j] = 1;
		}
	}

	generateEller(maze, cellsets, size - 1);
	addBorders(maze, size);
	for(i = 0; i < size + 4; i++) //free set identifications
	{
		free(cellsets[i]);
	}
}

/*
 * Tree maze
 * treemaze.c
 */

void CApp::findneighbours(int P1, int P2, int *neighbours, int *neighbourcount, short **cells, int length)
{
	*neighbourcount = 0;
	if(P2 + 2 <= length && cells[P1][P2+2] == 0) //Checks all neigbours and puts 1 in array if there is a free neighbour in that position
	{
		neighbours[1] = 1;
		(*neighbourcount)++;
	}
	if(P1 + 2 <= length && cells[P1+2][P2] == 0)
	{
		neighbours[2] = 1;
		(*neighbourcount)++;
	}
	if(P1 - 2 >= 1 && cells[P1-2][P2] == 0)
	{
		neighbours[0] = 1;
		(*neighbourcount)++;
	}
	if(P2 - 2 >= 1 && cells[P1][P2-2] == 0)
	{
		neighbours[3] = 1;
		(*neighbourcount)++;
	}
}

int CApp::searchforcell (short **cells, int length, int* P1, int* P2, int *neighbours, int *neighbourcount, cellsets_s *cellsets, int counter, int *visited)
{
	int countbuf = 1;
	int visitedlength = counter;

	countbuf = (rand() % (visitedlength)) + 1;//picks an available cell from array
	int start1 = cellsets[visited[countbuf]].P1;
	int start2 = cellsets[visited[countbuf]].P2;
	visited[countbuf] = visited[visitedlength];//takes that cell out of available cells
	visitedlength--;//decreases array size
	findneighbours(start1, start2, neighbours, neighbourcount, cells, length);//checks if picked cell has an unvisited neighbour cell
	while(*neighbourcount == 0) {//while loop for if cell did not have any free neighbours, same process as above
				countbuf = (rand() % (visitedlength)) + 1;
				start1 = cellsets[visited[countbuf]].P1;
				start2 = cellsets[visited[countbuf]].P2;
				visited[countbuf] = visited[visitedlength];
				visitedlength--;
		findneighbours(start1, start2, neighbours, neighbourcount, cells, length);
		if (visitedlength == 0)//if array is out of free cells then can assume all cells have been already visited
			return 0;
	}
	*P1 = start1;//assign new cell values if suitable cell found
	*P2 = start2;
	return 1;
}

int CApp::searchforcell_backtracer (short** cells, int length, int* P1, int* P2, int *neighbours, int *neighbourcount, cellsets_s *cellsets, int counter)
{
	while(counter != 0) //backtracer always pick the newest cell and if that cell is not suitable, then second newest and so on until it finds a suitable cell.
	{
		findneighbours(cellsets[counter].P1, cellsets[counter].P2, neighbours, neighbourcount, cells, length);
		if(*neighbourcount != 0)
		{
			*P1 = cellsets[counter].P1;
			*P2 = cellsets[counter].P2;
			return 1;
		}
		counter--;
	}
	return 0;
}

void CApp::generateTree(short **cells, cellsets_s *cellsets, int length, short algo, short algoback)
{
		int *neighbours = (int*)calloc(4, sizeof(int *));
		int neighbourcount = 0;
		srand(time(NULL));
		int randneighbour;
		int counter = 1;//counter to keep track of visited cells
		int visited[length*length/2];
		int P1 = (rand() % (length - 2)) + 1;//pick starting cell
		int P2 = (rand() % (length - 2)) + 1;
		if (P1 % 2 == 0) {//cell location must be uneven
				P1++;
		}
		if (P2 % 2 == 0) {
				P2++;
		}
		int cellcounter = 0;

		cellsets[counter].P1 = P1;//save first cell position
		cellsets[counter].P2 = P2;
		visited[counter] = counter;
		counter++;
		cells[P1][P2] = 1;
		while (cellcounter <= length * length / 2) {//loops until all cells have been visited
				for (int s = 0; s < 4; s++)//sets array to 0
						neighbours[s] = 0;

				findneighbours(P1, P2, neighbours, &neighbourcount, cells, length);//finds free neighbours
				if (neighbourcount != 0) { //if there are no free neighbour cells then skip code in brackets
						randneighbour = 1;
						if (neighbourcount > 1)//if there is a choice
								randneighbour = (rand() % (neighbourcount) + 1); //randomly picks a free neighbour
						int i = 0;
						while (randneighbour > 0) {//Finds the correct chosen neighbour, stores position to poscounter
								if (neighbours[i] == 1) {
										i++;
										randneighbour--;
								} else {
										i++;
								}
						}
						visited[counter] = counter;
						switch (i - 1) {//goes to neighbour and carves path. Updates P1 and P2 to neighbours position
								case 0:
										cellsets[counter].P1 = P1 - 2;
										cellsets[counter].P2 = P2;
										counter++;
										cells[P1 - 1][P2] = 1;
										P1 -= 2;
										break;
								case 1:
										cellsets[counter].P1 = P1;
										cellsets[counter].P2 = P2 + 2;
										counter++;
										cells[P1][P2 + 1] = 1;
										P2 += 2;
										break;
								case 2:
										cellsets[counter].P1 = P1 + 2;
										cellsets[counter].P2 = P2;
										counter++;
										cells[P1 + 1][P2] = 1;
										P1 += 2;
										break;
								case 3:
										cellsets[counter].P1 = P1;
										cellsets[counter].P2 = P2 - 2;
										counter++;
										cells[P1][P2 - 1] = 1;
										P2 -= 2;
										break;
						}
						cellcounter++;
						cells[P1][P2] = 1;

						if (algoback == 2) {
								if (!(rand() % 4)) {//braided maze code part (adds loops. loop count can be changed with rand() % x value)
										if (P2 != length - 1 && cells[P1][P2 + 1] != 1)
												cells[P1][P2 + 1] = 1;
										else if (P1 != length - 1 && cells[P1 + 1][P2] != 1)
												cells[P1 + 1][P2] = 1;
										else if (P1 != 1 && cells[P1 - 1][P2] != 1)
												cells[P1 - 1][P2] = 1;
										else if (P2 != 1 && cells[P1][P2 - 1] != 1)
												cells[P1][P2 - 1] = 1;
								}
						}
						if (algo == 3) {//to differentiate maze generation algorithm
								if (!searchforcell(cells, length, &P1, &P2, neighbours, &neighbourcount, cellsets, counter -1, visited))//if there is no free neighbour cell then search for new starting point, if all cells taken, then exit function
										return;
						}else{
								if (!searchforcell_backtracer(cells, length, &P1, &P2, neighbours, &neighbourcount, cellsets, counter - 1))//if there is no free neighbour cell then search for new starting point, if all cells taken, then exit function
										return;
						}
			 }
		}
}

void CApp::treemaze(int length, short **maze, short algo, short algoloop)
{
	cellsets_s *cellsets = (cellsets_s*)calloc((length + 4)*(length + 4), sizeof(cellsets_s));//array to keep track of visited cells
	generateTree(maze, cellsets, length - 1, algo, algoloop);
	addBorders(maze, length);
	free(cellsets);
}

/*
 * Find exits
 * findExits.c
 */

int CApp::findExits(int size, short **maze, short *exits)
{
	int i;

	exits[0] = 0;
	exits[2] = 0; // not sure if exit is on the bottom

	for(i = 0; i < size; i++)
	{
			if(maze[0][i] == 1)
			{
					exits[0] = i;
					exits[1] = 0;
			}
			if(maze[size - 1][i] == 1)
			{
					exits[2] = i;
					exits[3] = size - 1;
			}
	}

	if(exits[0] == 0)
	{
			pConsole->Print("There is no entrance!!");
			//exit(0);
			return 1;
	}
	if(exits[2] == 0) // exit wasn't on the bottom, checking right side
	{
		for(i = 0; i < size - 1; i++)
		{
			if(maze[i][size - 1] == 1)
			{
				exits[2] = size - 1;
				exits[3] = i;
			}
		}
	}
	if(exits[2] == 0)
	{
		pConsole->Print("There is no exit!!");
		//exit(0);
		return 1;
	}

	return 0;
}

/*
 * Dead ends
 * dead.c
 */

void CApp::eliminate(int size, short **maze, int x, int y) // follow a dead end til an intersection
{
		int free = 0;
		// To stop the unused variable warning;
		size++;
		size--;
		// --
		while(free < 2){
				free = 0;
				if(maze[y + 1][x] == 1){ // looking where to go
						maze[y][x] = 0;
						y++;
				}
				else if(maze[y - 1][x] == 1){
						maze[y][x] = 0;
						y--;
				}
				else if(maze[y][x + 1] == 1){
						maze[y][x] = 0;
						x++;
				}
				else if(maze[y][x - 1] == 1){
						maze[y][x] = 0;
						x--;
				}
				if(maze[y - 1][x] == 1) // checking if its at an intersection
						free++;
				if(maze[y][x + 1] == 1)
						free++;
				if(maze[y][x - 1] == 1)
						free++;
				if(maze[y + 1][x] == 1)
						free++;
		}
}

double CApp::deadEnd(int size, short **maze, short *exits)
{
		struct timespec start, end;
		int deadend, x, y;
		clock_gettime(CLOCK_REALTIME, &start); // start timer
		// 1 = passage, 0 = wall
		for(y = 0; y < size; y++){
				for(x = 0; x < size; x++){
						deadend = 0;
						if((exits[1] == y && x == exits[0]) || (y == exits[3] && x == exits[2]))
						{ // if it thinks an entrance or exit is a dead end, skip an interation
								continue;
						}
						else if(maze[y][x] == 1){
								//check all directions
								if(maze[y - 1][x] == 0 && y - 1 >= 0){ // north
										deadend++;
								}
								if(maze[y][x + 1] == 0 && x + 1 < size){ // east
										deadend++;
								}

								if(maze[y][x - 1] == 0 && x - 1 >= 0){ // west
										deadend++;
								}
								if(maze[y + 1][x] == 0 && y + 1 < size){ // east
										deadend++;
								}
						}
						if(deadend >= 3){ // found a dead end
								eliminate(size, maze, x, y); // fill it
						}
				}
		}
		clock_gettime(CLOCK_REALTIME, &end); // stop timer
		// time_spent = end - start
		double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
		return time_spent;
}

/*
 * Recursion
 * rec.c
 */

// https://www.codesdope.com/blog/article/backtracking-to-solve-a-rat-in-a-maze-c-java-pytho/
int CApp::solvemaze(int y, int x, int endy, int endx, int size, short **maze, short **solution, short **helper)
{
		// checking if it has reached the end

		if((y == endy) && (x == endx)){
				solution[y][x] = 1;
				return 1;
		}
		// is it within bounds and has it not been here before
		if(y >= 0 && x >= 0 && y < size && x < size && solution[y][x] == 0 && maze[y][x] == 1 && helper[y][x] == 0){
				solution[y][x] = 1;
				helper[y][x] = 1;
				moves += 1;
				//going down
				if(solvemaze(y + 1, x, endy, endx, size, maze, solution, helper))
						return 1;
				//going right
				if(solvemaze(y, x + 1, endy, endx, size, maze, solution, helper))
						return 1;
				//going up
				if(solvemaze(y - 1, x, endy, endx, size, maze, solution, helper))
						return 1;
				//going left
				if(solvemaze(y, x - 1, endy, endx, size, maze, solution, helper))
						return 1;
				//backtracking
				moves--;
				solution[y][x] = 0; // can't go anywhere, dead end, go back
				return 0;
		}
		helper[y][x] = 1;
		return 0;
}

double CApp::recursion(int size, short **raw, short **sol, short *exits)
{
		struct timespec start, end;
		moves = 0;
		clock_gettime(CLOCK_REALTIME, &start); // start timer
		short **helper;
		helper = (short**)calloc(size, size * sizeof(short*));
		for(int i = 0; i < size; i++)
				helper[i] = (short*)calloc(size, size * sizeof(short));
		solvemaze(exits[1], exits[0], exits[3], exits[2], size, raw, sol, helper);
		for(int i = 0; i < size; i++)
				free(helper[i]);
		clock_gettime(CLOCK_REALTIME, &end); // stop timer
		// time_spent = end - start
		double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;

		pConsole->Print("Rec solution lenght %d squares", moves + 1);
		return time_spent;
}

/*
 * bfs
 * bfs.c
 */

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* CApp::createQueue(unsigned capacity)
{
		struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
		queue->capacity = capacity;
		queue->front = queue->size = 0;

		// This is important, see the enqueue
		queue->rear = capacity - 1;
		queue->array = (int*)malloc(
				queue->capacity * sizeof(int));
		return queue;
}

// Queue is full when size becomes
// equal to the capacity
int CApp::isFull(struct Queue* queue)
{
		return (queue->size == (int)queue->capacity);
}

// Queue is empty when size is 0
int CApp::isEmpty(struct Queue* queue)
{
		return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void CApp::enqueue(struct Queue* queue, int item)
{
		if(isFull(queue))
				return;
		queue->rear = (queue->rear + 1)
									% queue->capacity;
		queue->array[queue->rear] = item;
		queue->size = queue->size + 1;
		//printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
int CApp::dequeue(struct Queue* queue)
{
		if(isEmpty(queue))
				return INT_MIN;
		int item = queue->array[queue->front];
		queue->front = (queue->front + 1)
									 % queue->capacity;
		queue->size = queue->size - 1;
		return item;
}

// finding the path of the shortest solution after the search has reached the end
void CApp::findPath(int size, short **sol, short **adjacent, short **previous, short *exits)
{
		int moves = 0;
		int x = exits[2], y = exits[3];
		int next = previous[exits[3]][exits[2]]; // first position is the exit
		while(x != exits[0] || y != exits[1]){ // until it reaches the entrance
				sol[y][x] = 2; // mark it as a path on the solution maze
				moves++;
				// checking all sides
				if(y > 0 && adjacent[y - 1][x] == next)
						y--;
				else if(x + 1 < size && adjacent[y][x + 1] == next)
						x++;
				else if(x > 0 && adjacent[y][x - 1] == next)
						x--;
				else if(y + 1 < size && adjacent[y + 1][x] == next)
						y++;
				next = previous[y][x]; // next position
		}
		moves++;
		sol[y][x] = 2; // marking the beginning as a path as well
		pConsole->Print("BFS solution lenght %d squares", moves);
}

int CApp::solve(int size, short **raw, short **sol, short *exits)
{
		struct Queue* rowqueue = createQueue(100000); // creating queues for x and y coordinates
		struct Queue* colqueue = createQueue(100000); // unnecessary size for now
		int r, c, nodes_in_current = 1, nodes_in_next;
		int rr, cc;
		/*
		short visited[size][size]; // marks if a square has been visited
		short previous[size][size]; // for finding the shortest way back from the exit
		short adjacent[size][size];
		*/



        // Allocate arrays Added
		short **visited; // marks if a square has been visited
		short **previous; // for finding the shortest way back from the exit
		short **adjacent;
		Matrix2DShortAlloc(size, size, &visited); // marks if a square has been visited
		Matrix2DShortAlloc(size, size, &previous); // for finding the shortest way back from the exit
		Matrix2DShortAlloc(size, size, &adjacent);



		short dr[4] = {-1, 1, 0, 0}; // movement vectors S, N, E, W
		short dc[4] = {0, 0, 1, -1};
		int i, j, nr = 0;

		for(i = 0; i < size; i++){
				for(j = 0; j < size; j++){
						previous[i][j] = 0;
						visited[i][j] = 0;
						adjacent[i][j] = nr; // marking every square with an unique number to
						nr++;                // remember where it got to the current position
				}
		}
		enqueue(rowqueue, exits[1]); // queueing the start
		enqueue(colqueue, exits[0]);
		while(rowqueue->size > 0){ // works until there are nodes to visit
				r = dequeue(rowqueue); // take the current position off the queue, no need to visit it again
				c = dequeue(colqueue);
				nr++;
				if(r == exits[3] && c == exits[2]){ // check if reached the exit
						findPath(size, sol, adjacent, previous, exits); // find the shortest way back
						free(rowqueue->array); // free the queues
						free(rowqueue);
						free(colqueue->array);
						free(colqueue);
						Matrix2DShortFree(size, visited);
						Matrix2DShortFree(size, previous);
						Matrix2DShortFree(size, adjacent);
						return 0;
				}
				for(i = 0; i < 4; i++){ // try every move
						rr = r + dr[i]; // new position where to move
						cc = c + dc[i];


						//printf("\nvisited %d\nraw %d\n", visited[rr][cc], raw[rr][cc]);
						if(rr < 0 || cc < 0 || rr >= size || c >= size){ // new position would be out of bounds
								continue;
						}
						if((visited[rr][cc] == 1) ||( raw[rr][cc] == 0)){ // already been here or new position would be a wall
								continue;
						}
						enqueue(rowqueue, rr); // mark it as new node to visit
						enqueue(colqueue, cc);

						visited[rr][cc] = 1; // mark current position as visited
						previous[rr][cc] = adjacent[r][c]; // mark the unique number of the square that brought us here
						nodes_in_next++; // increase the nr of nodes to visit in next layer
				}
				nodes_in_current--; // remove a visited node
				if(nodes_in_current == 0){ // reached the end of current nodes, moving to next layer
						nodes_in_current = nodes_in_next;
						nodes_in_next = 0;
				}
		}

		free(rowqueue->array); // free the queues
		free(rowqueue);
		free(colqueue->array);
		free(colqueue);
		Matrix2DShortFree(size, visited);
		Matrix2DShortFree(size, previous);
		Matrix2DShortFree(size, adjacent);
		return -1;
}

double CApp::bfs(int size, short **raw, short **sol, short *exits)
{
		struct timespec start, end;
		clock_gettime(CLOCK_REALTIME, &start); // start timer
		// 1 = passage, 0 = wall
		int result = solve(size, raw, sol, exits);
		if(result == -1){
				pConsole->Print("There is no solution!");
				return 0;
		}
		clock_gettime(CLOCK_REALTIME, &end); // stop timer
		// time_spent = end - start
		double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
		return time_spent;
}

/*
 * Manage files
 * manageFiles.c
 */

/*
		mode        = read / write
		filetype    = txt / svg / bmp
		complexity  = ask for filenames / don't / ask user
		mazetype    = raw or solved
		M           = maze struct

*/
FILE *CApp::getFilename(int mode, int filetype, int complexity, int mazetype)
{    // getting the filename
		FILE *f = NULL;
		char default_filenames[2][NAMELEN]  = { {"raw"},  {"solved"} };
		char default_extensions[3][7]       = { {".txt"}, {".svg"}, {".bmp"} }; // all possible combiantions of default files and user questions
		//char chat_mode[2][MENUOPTION]       = { {"read"}, {"write"} };
		//char chat_file[3][MENUOPTION]       = { {"text"}, {"svg"}, {"bitmap"} };
		//char chat_grammar[2][MENUOPTION]    = { {"from"}, {"to"} };
		if(mazetype == final_maze)
				mazetype = 1;
		//printf("mazetype is %d\n", mazetype);
		/*
		if(complexity == ask_user){ // ask user if they want custom filenames
				char ans;
				getchar();
				printf("Would you like to select personal filenames? y/n\n");
				scanf("%c", &ans);
				if(ans == 'y')
						complexity = manual;        // yes
				else if(ans == 'n')
						complexity = automatic;     // no
				else{
						printf("Invalid input\n");
						return 0;
				}
		}
		*/
		if(complexity == automatic){                                            // using default files like "raw.svg"
				pConsole->Print("opening file %s\n",strcat(default_filenames[mazetype], default_extensions[filetype]));
				if(filetype == svg_file){
						if(mode == write)
								f = fopen(default_filenames[mazetype], "wb");
						else
								f = fopen(strcat(default_filenames[mazetype], default_extensions[filetype]), "rb");
				}
				else if(filetype == bmp_file)
						f = fopen(default_filenames[mazetype], "wb");

				else
						f = fopen(strcat(default_filenames[mazetype], default_extensions[filetype]), "r+");
				if(f == NULL){
						pConsole->Print("Error automatically opening file");                     // not supposed to go wrong
						return NULL;
				}
				return f;
		}
		/*
		int file = 0;
		char ans;
		char buf[NAMELEN];
		// asking user what file would he like to read / save the txt / svg / bmp
		printf("Where would you like to %s the %s file %s? ", chat_mode[mode], chat_file[filetype], chat_grammar[mode]);
		scanf("%s", buf);
		if(filetype == bmp_file || filetype == svg_file){
				if(mode == write)
						f = fopen(buf, "wb");
				else
						f = fopen(buf, "rb");
		}
		else
				f = fopen(buf, "r+");

		while(f == NULL){ // user is messing with incorrect files
				while(file == 0){
						printf("Failed to open %s\n", buf);
						printf("Would you like to try again? y/n\n");
						scanf("%c", &ans);
						if(ans == 'n')
								return f;
						else if(ans == 'y'){
								printf("Enter filename: ");
								scanf("%s", buf);
								if(filetype == bmp_file)
										f = fopen(buf, "wb");
								else
										f = fopen(buf, "r+");
								if(f != NULL)
										break;
						}
				}
		}
		*/
		return f;
}

int CApp::readTXT(FILE *f, maze_t *M)
{    // reading maze from a text file
		short maze_element;
		if(f == NULL)
				return -1;
		fscanf(f, "%d", &M->size);      // getting maze size
		//sizeCheck(M->size);             // seeing if size is allowed
		feedMemory(M);                  // giving enough memory
		int i, j, solved = 0;
		for(i = 0; i < M->size; i++){
				for(j = 0; j < M->size; j++){
						fscanf(f, "%hd", &maze_element);
						if(i == 0 && maze_element > 1){ // checking if input is an already solved maze, only need to check the first row
								solved = 1;
						}
						if(maze_element < wall && maze_element > crossover){ // thats not our maze file!
								pConsole->Print("Incorrect file!!");
								return -1;
						}
						M->algo[generated].maze[i][j] = maze_element;
				}
		}

		pConsole->Print("Maze read from text file");
		fclose(f);
		return solved; // returning whether the read maze was already solved or not
}

int CApp::writeTXT(FILE *f, maze_t *M, int mazetype)
{ // writing to a txt file
		if(M->size == 0){
				pConsole->Print("No maze generated!"); // let's see if something is actually generated
				return -1;
		}

		fprintf(f, "%d\n", M->size); // printing maze size
		int i, j;
		for(i = 0; i < M->size; i++){
				for(j = 0; j < M->size; j++){
						fprintf(f, "%d ", M->algo[mazetype].maze[i][j]);
				}
				fprintf(f, "\n");
		}
		pConsole->Print("Maze saved to text file");
		fclose(f);
		return 0;
}

// necessary subfunctions for SVG file reading
int CApp::breakIt(char *format[COLCOUNT], char *row)
{ // breaking fgets with strtok
		int i = 0;
		char *token = strtok(row, "\"");            // separator "
		while(token != NULL){                       // breaking row into elements
				format[i++] = token;
				token = strtok(NULL, "\"");
		}
		return i;
}

short CApp::findElement(char *input)
{                 // finding the color value
		static char elements[5][15] = { {"fill:black"}, {"fill:white"}, {"fill:red"}, {"fill:blue"}, {"fill:purple"} };
		for(int i = 0; i < 5; i++)                  // check all possible colors
				if(strcmp(input, elements[i]) == 0)
						return i;                           // return the index aka the color value in maze array
		return -1;                                  // incorrect SVG file was fed
}

int CApp::readSVG(FILE *svg, maze_t *M)
{// read from a SVG file
		int solved = 0;                             // marking if the scanned maze is solved or not
		char xml_header[ROWMAX] = {"<?xml version=\"1.0\"?>"}; // xml header
		int len = strlen(xml_header);
		char row[ROWMAX];
		fgets(row, ROWMAX, svg);
		printf("%s\n", row);
		if(strncmp(xml_header, row, len) != 0){           // checking if inserted file is svg
				pConsole->Print("incorrect file type inserted!");
				return -1;
		}
		char *format[COLCOUNT];                     //keeping strtok result here
		enum color_values{black, white, red, blue, purple};
		enum svg_values{x = 1, y = 3, color = 9};   // format array indexes where to find a certain value

		breakIt(format, fgets(row, ROWMAX, svg));   // getting maze size
		int nr_format = atoi(format[1]);
		pConsole->Print("SVG file maze size is %d", nr_format);
		/*
		if(sizeCheck(nr_format) == 0)         // seeing if size is allowed
				return -1;
		*/
		M->size = nr_format;                  // got the maze size from svg file
		feedMemory(M);                              // giving sufficient memory
		short element;
		fgets(row, ROWMAX, svg);                    // getting rid of unnecessary info in the svg header
		for(int i = 0; i < M->size * M->size; i++){ // scanning everything from the file
				breakIt(format, fgets(row, ROWMAX, svg));
				element = findElement(format[color]);
				if(element == -1){
						pConsole->Print("Unknown value present");  // value not supposed to be in svg file
						freeMemory(M);                      // free memory and set size to 0 again
						M->size = 0;
						return -1;
				}
				else if(i < M->size && element > 1)     // checking if the maze is already solved, only need to check the first row
						solved = 1;
				M->algo[generated].maze[atoi(format[y])][atoi(format[x])] = element; // write maze data
		}

		pConsole->Print("SVG file successfully read, maze size %d", M->size);
		fclose(svg);
		return solved; // return is maze was solved or not
}

int CApp::writeSVG(FILE *svg, maze_t *M, int mazetype)
{ // write to SVG file
		if(M->size == 0){
				pConsole->Print("No maze generated!"); // let's see if something is actually generated
				return -1;
		}
		int value;
		int size = M->size;
		fprintf(svg, "<?xml version=\"1.0\"?>\n");  // SVG header stuff
	fprintf(svg, "<svg width=\"%dcm\" height=\"%dcm\" viewBox=\"0 0 %d %d\"\n", size, size, size*5, size *5);
	fprintf(svg, "\txmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");
	fprintf(svg, "<g>");
	for(int a = 0; a < size; a++) {
		for(int b = 0; b < size; b++){
						value = M->algo[mazetype].maze[a][b]; // add every element of every matrix at the same xy
			if(value == crossover)         // passage visited by both algos aka crossover
								fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:purple\" />\n", b, a);
			else if(value == bfs_path)          // bfs algo path
								fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:blue\" />\n", b, a);
			else if(value == path)         // must be an unvisited passage
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:white\" />\n", b, a);
			else if(value == wall)        // must be a wall
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:black\" />\n", b, a);
			else if(value == rec_path)   // passage visited by recursive algo
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:red\" />\n", b, a);
		}
	}
	fprintf(svg, "</g>"); // SVG footer
	fprintf(svg, "</svg>\n");
	fclose(svg);
		printf("SVG file saved\n");
		return 0;
}

/*
		mode        = read / write
		filetype    = txt / svg / bmp
		complexity  = ask for filenames / don't / ask user
		mazetype    = raw or solved
		M           = maze struct

*/
int CApp::manageFiles(int mode, int filetype, int complexity, int mazetype, maze_t *M)
{
		if(mode == read && M->size > 0){ // something is already in memory
				printf("Overwriting previous maze\n");
				freeMemory(M);
		}
		int success = 0;
		switch(mode){ // lets see if were reading or writing
				case read:
						switch(filetype){   // reading, can either be from a text or svg file
								case txt_file:
										success = readTXT(getFilename(read, filetype, complexity, mazetype), M);
										break;
								case svg_file:
										success = readSVG(getFilename(read, filetype, complexity, mazetype), M);
										break;
						}
						break;
				case write:             // writing, can be to a text, svg or bmp file
						switch(filetype){
								case txt_file:
										success = writeTXT(getFilename(write, filetype, complexity, mazetype), M, mazetype);
										break;
								case svg_file:
										success = writeSVG(getFilename(write, filetype, complexity, mazetype), M, mazetype);
										break;
								case bmp_file:
										//success = makeBMP(getFilename(write, filetype, complexity, mazetype), regular, mazetype, M);
										break;
						}
						break;

		}

		if(success == 1){ // maze is already solved, just warn the user
				char user;
				printf("Warning, maze is already solved\n");
				printf("Would you like to reset the maze to an unsolved state? y/n\n");
				getchar();
				scanf("%c", &user);
				if(user == 'y'){
						mazeReset(M);   // resetting the maze
						printf("Maze reset successful\n");
						return 0;    // mark the maze as unsolved
				}
				else
						M->algo[final_maze].maze = M->algo[generated].maze; // if its solved it'll be added to appropriate place in the array
		}

		return success;
}

/*
 * BMP
 * mazeBMP.c
 */

void CApp::set_color(double *to, short from[3])
{       // setting the actual RGB color to an element
		for(int i = 0; i < 3; i++){
				to[i] = from[i];
		}
}

void CApp::fade_calculate(short *to, short *from, fade_t *fade_arr, int element)
{ // calculating the color change per row / column
		for(int i = 0; i < 3; i++){ // for R, G and B
				fade_arr[element].fade[i] = abs(((double)(to[i]) - (double)(from[i]))) / (double)(fade_arr[0].size); // calculating the change in color per row
				if(to[i] < from[i])
						fade_arr[element].fade[i] *= -1;        // if a value needs to decrease it needs to change it to negative
				fade_arr[element].original[i] = from[i];    // saving the original color
		}
}

// max = maximum random value, count = how many, RGB = give a full array random colors
short CApp::randomNr(int max, int count, short RGB[3])
{ // if the user wants a random nr
		if(count < 3){
				return rand() % max;
		}
		for(int i = 0; i < count; i++){
				RGB[i] = rand() % max; // don't need the minimum, it will always be 0
		}
		return 0;
}

int CApp::userselectRGB(double *input)
{ // letting the user select a fully custom color with an RGB code
		printf("Use format: 255 255 255\n");
		printf("Enter RGB code (NB! values have a range of 0 - 255): ");
		scanf("%lf %lf %lf", &input[2], &input[1], &input[0]); // holding the color in BGR format so input needs to be saved backwards
		for(int i = 0; i < 3; i++)
				if(input[i] < 0 || input[i] > 255) // checking user input
						return 0;
		return 1;
}

void CApp::get_colors(double *input, int mode, P_COLOR_RGB _ElmntClr, int _FadeDir, int type, fade_t *fade_arr)
{
    //##############
    // Changed color

	// Element color set.
	switch(type)
	{
	case wall: // giving every element its color
		input[0] = _ElmntClr[wall].Blue;
		input[1] = _ElmntClr[wall].Green;
		input[2] = _ElmntClr[wall].Red;
		//set_color(input, colors[black]);    // wall, default color: black
		break;
	case path:
		input[0] = _ElmntClr[path].Blue;
		input[1] = _ElmntClr[path].Green;
		input[2] = _ElmntClr[path].Red;
		//set_color(input, colors[white]);    // path, default color: white
		break;
	case recursive:
		input[0] = _ElmntClr[recursive].Blue;
		input[1] = _ElmntClr[recursive].Green;
		input[2] = _ElmntClr[recursive].Red;
		//set_color(input, colors[red]);      // recursive solution, default color: red
		break;
	case breath_first:
		input[0] = _ElmntClr[breath_first].Blue;
		input[1] = _ElmntClr[breath_first].Green;
		input[2] = _ElmntClr[breath_first].Red;
		//set_color(input, colors[blue]);     // BFS solution, default color: blue
		break;
	case crossover:
		input[0] = _ElmntClr[crossover].Blue;
		input[1] = _ElmntClr[crossover].Green;
		input[2] = _ElmntClr[crossover].Red;
		//set_color(input, colors[purple]);   // solution crossover, default color: purple
		break;
	}

	// Fade setup.
	fade_arr[type].direction = _FadeDir;

	return;

	//###################################################################################

	// set the element colors
	// all maze elements
	static char options[5][30] = { {"walls"}, {"paths"}, {"recursive solution"}, {"shortest solution"}, {"algo crossover"} };

	// color library, note RGB values are backwards
	static short colors[COLORCOUNT][3] = {
			{255, 255, 255},    // white
			{192, 192, 192},    // silver
			{128, 128, 128},    // gray
			{0, 0, 0},          // black
			{128, 0, 128},      // purple
			{255, 0, 255},      // magenta
			{255, 0, 0},        // blue
			{0, 255, 0},        // lime
			{0, 128, 0},        // green
			{255, 255, 0},      // cyan
			{212, 255, 127},    // aqua
			{0, 0, 255},        // red
			{0, 255, 255},      // yellow
			{0, 0, 128}         // maroon
	};

	if(mode == regular)
	{ // preset colors, no input needed
		switch(type)
		{
		case wall: // giving every element its color
			input[0] = _ElmntClr[wall].Blue;
			input[1] = _ElmntClr[wall].Green;
			input[2] = _ElmntClr[wall].Red;
			//set_color(input, colors[black]);    // wall, default color: black
			break;
		case path:
			input[0] = _ElmntClr[path].Blue;
			input[1] = _ElmntClr[path].Green;
			input[2] = _ElmntClr[path].Red;
			//set_color(input, colors[white]);    // path, default color: white
			break;
		case recursive:
			input[0] = _ElmntClr[recursive].Blue;
			input[1] = _ElmntClr[recursive].Green;
			input[2] = _ElmntClr[recursive].Red;
			//set_color(input, colors[red]);      // recursive solution, default color: red
			break;
		case breath_first:
			input[0] = _ElmntClr[breath_first].Blue;
			input[1] = _ElmntClr[breath_first].Green;
			input[2] = _ElmntClr[breath_first].Red;
			//set_color(input, colors[blue]);     // BFS solution, default color: blue
			break;
		case crossover:
			input[0] = _ElmntClr[crossover].Blue;
			input[1] = _ElmntClr[crossover].Green;
			input[2] = _ElmntClr[crossover].Red;
			//set_color(input, colors[purple]);   // solution crossover, default color: purple
			break;
		}
	}
	else if(mode == secret){ // user input needed on color
			int user = -1;
			int fade_inputs[2];
			short gen_fade[2][3]; // placeholder for random colors
			int i = 0;
			int j = 0;
			enum fadeInput{start_color, end_color};     // indexes for user fade input gen_fade
			printf("What color should the %s be?\n", options[type]); // asking for every element
			scanf("%d", &user);
			switch(user){
					case white ... maroon:                  // just a color from the list
							fade_arr[type].direction = none;    // mark there is no fade
							set_color(input, colors[user]);     // set user wanted color for element
							break;
					case fade_cl:                           // user selected fade
							randomNr(255, 3, gen_fade[start_color]);   // precalculating a random color, that way two consecutive randoms won't be equal
							printf("Select starting color: ");
							scanf("%d", &fade_inputs[start_color]);
							printf("Select ending color: ");
							scanf("%d", &fade_inputs[end_color]);   // where to begin and end
							for(i = 0; i < 2; i++){
									switch(fade_inputs[i]){
											case white ... maroon:  // just a color, no further modification needed
													for(j = 0; j < 3; j++)
															gen_fade[i][j] = colors[fade_inputs[i]][j]; // can't use set color due to gen_fade being short
													break;
											case random_list:       // giving the element a random color from the list 0 - 14
													fade_inputs[i] = randomNr(COLORCOUNT, 0, NULL);
													for(j = 0; j < 3; j++)
															gen_fade[i][j] = colors[fade_inputs[i]][j]; // can't use set color due to gen_fade being short
													break;
											case random_overall:    // giving the element a random R, G and B value 0 - 255
													if(i > start_color)
															randomNr(255, 3, gen_fade[i]);
													break;
											case custom_RGB_code:
													if(userselectRGB(input) == 0){
															printf("Invalid input\n");
															exit(0);
													}
													for(j = 0; j < 3; j++)
															gen_fade[i][j] = input[j]; // can't use set color due to gen_fade being short
													printf("%d: %hd %hd %hd\n", i, gen_fade[i][0], gen_fade[i][1], gen_fade[i][2]);
													break;
											default:
													printf("Invalid input\n");
													exit(0);
													break;
									}
							}
							// horizontal is 1, vertical is 2
							/*  printing a BMP starts from the bottom so to get the colors correct the start and end need to be
									different depending on the fade direction
							*/
							user = none;
							while(user != horizontal && user != vertical){ // asking user for fade direction
									printf(" 1. Horizontal fade\n");
									printf(" 2. Vertical fade\n");
									scanf("%d", &user);
									if(user < horizontal || user > vertical)
											printf("Invalid input\n");
									getchar();
							}
							fade_arr[type].direction = user;            // marking down the fade direction
							if(fade_arr[type].direction == vertical){
									fade_calculate(gen_fade[start_color], gen_fade[end_color], fade_arr, type); // calculate fade
									set_color(input, gen_fade[end_color]);

							}
							else{
									fade_calculate(gen_fade[end_color], gen_fade[start_color], fade_arr, type); // calculate fade
									set_color(input, gen_fade[start_color]);
							}
							break;
					case random_list:                               // user just wants a random color from the list
							fade_arr[type].direction = none;            // there is no fade
							set_color(input, colors[randomNr(COLCOUNT, 0, NULL)]);
							break;                                      // get a random color between 0 and max colors, then set the input to it
					case random_overall:                            // user wants a completely random color for element
							fade_arr[type].direction = none;            // there is no fade
							randomNr(255, 3, gen_fade[start_color]);    // using an array meant for fade colors but it'll do when not in use
							set_color(input, gen_fade[start_color]);    // get 3 random colors for R, G and B, set the element to it
							break;
					case custom_RGB_code:                           // user wants a custom color
							fade_arr[type].direction = none;            // there is no fade
							if(userselectRGB(input) == 0){              // ask for a color and check the function output
									printf("Invalid input\n");
									exit(0);
							}
							break;
					default:                                        // user messed something up, give up
							printf("Invalid input\n");
							exit(0);
			}
	}
}

// Function to round an int to a multiple of 4
int CApp::round4(int x)
{
		return x % 4 == 0 ? x : x - x % 4 + 4;
}

/* Input values:
								height = maze size
								**maze = maze matrix
								mode = no color options or with selectable colors
								present_elements = how many elements need colors,
																if the maze is not solved then only the wall and path colors are needed
*/

int CApp::makeBMP(FILE *f, int mode, P_COLOR_RGB _ElmntClr, int _FadeDir, int mazetype, maze_t *M)
{ // making a BMP file from a maze matrix
		srand(time(NULL));                                   // start time in case of random colors
		int height = M->size;                                // mark actual maze size to element height
		int original = height;                               // it will be known as original size
		int width;
		int dpi = 1000;                                      // bmp image dpi
		int upscale_factor = BMPTARGETSIZE / height;         // setting the upscale factor, all mazes will be roughly the same size
		height *= upscale_factor;                            // calculate new size and width according to upscaling
		width = height;

		int padded_width = round4(width * 3);                // calculating image and file size, in a bmp file colors have 4 bytes
		int image_size = height * padded_width * 3;          // counting for padding size, actual image size
		int file_size = 54 + 4 * image_size;                 // calculating file size, adding the header
		int ppm = dpi * 39.375;

		memcpy(&bfh.bitmap_type, "BM", 2); // setting up the file header
		bfh.file_size       = file_size;
		bfh.reserved1       = 0;
		bfh.reserved2       = 0;
		bfh.offset_bits     = 0;

		bih.size_header     = sizeof(bih);
		bih.width           = width;
		bih.height          = height;
		bih.planes          = 1;
		bih.bit_count       = 24;
		bih.compression     = 0;
		bih.image_size      = file_size;
		bih.ppm_x           = ppm;
		bih.ppm_y           = ppm;
		bih.clr_used        = 0;
		bih.clr_important   = 0;

		fwrite(&bfh, 1, 14, f);
		fwrite(&bih, 1, sizeof(bih), f); // writing the header to the file

		int idx = 0, idy = 0;
		selected_t elements[5]; // color values for all elements
		fade_t colorfade[5];    // fade change values for all elements
		colorfade[0].size = (float)height; // including the maze size, need it for calculation in fade_calculate()

		/*
		if(mode == secret){ // lets print the color cheatsheet
				printf("!!!Color cheatsheet!!!\n");
				printf(" 0: White    |   7: Lime\n");
				printf(" 1: Silver   |   8: Green\n");
				printf(" 2: Gray     |   9: Cyan\n");
				printf(" 3: Black    |  10: Aqua\n");
				printf(" 4: Purple   |  11: Red\n");
				printf(" 5: Magenta  |  12: Yellow\n");
				printf(" 6: Blue     |  13: Maroon\n");
				printf("---------------------------\n");
				printf("66: Random from list\n");
				printf("77: Fully random\n");
				printf("88: Custom RGB value\n");
				printf("99: Fade :o\n");
		}
		*/

		int present_elements;
		if(mazetype == final_maze)                  // does the sent maze have a solution path
				present_elements = 5;                   // yes, there are 5 different elements
		else
				present_elements = 2;                   // no, there are 2 different elements (wall and path)

		if(mode != insanity){
				for(int i = 0; i < present_elements; i++)
				{
					// if an unsolved maze was given, no colors for solutions will be needed
					//get_colors(elements[i].rgb, mode, _ElmntClr, _FadeDir, i, colorfade); // set colors for all present elements
					elements[i].rgb[0] = _ElmntClr[i].Blue;
					elements[i].rgb[1] = _ElmntClr[i].Green;
					elements[i].rgb[2] = _ElmntClr[i].Red;
					colorfade[i].direction = _FadeDir;
				}
		}
		else if(mode == insanity){              // every row is a new color
				for(int i = 0; i < 3; i++){
						elements[wall].rgb[i] = 192;    // just give a random value for now
						elements[path].rgb[i] = 255;
				}
		}
		unsigned char *bitmap = (unsigned char *) malloc(bih.image_size * sizeof(char));  // actual bitmap to be written in the file
		int x_fade_count = 0; // elements with a color fade in both directions
		int y_fade_count = 0;

		for(int i = 0; i < image_size; i++) bitmap[i] = 255; // painting the whole bitmap white, could be unnecessary

		if(mode == secret){ // don't need to check when the user didn't choose colors
				for(int i = 0; i < 5; i++){
						if(colorfade[i].direction == horizontal) // checking if horizontal and/or vertical color fades are present
								x_fade_count++;
						else if(colorfade[i].direction == vertical)
								y_fade_count++;
				}
		}

		int i, j; // loop variables
		int yprevious = 0, xprevious = 0; // keeping track if were printing a new element from the maze matrix, necessary for insanity mode
		short maze_value;
		for (int row = 0; row < height; row++){ // for every row
				if(row / upscale_factor == 0) // see if the maze row needs to be changed, actual file size is larger than the maze
						idy = 0;
				else
						idy = row / upscale_factor;
				if(mode == insanity && (yprevious != idy || xprevious != idx)){ // changing the color for every row if mode = insanity
						for(i = 0; i < 3; i++){
								elements[wall].rgb[i] = rand() % 255;
						}
						//printf("%.0f %.0f %.0f\n", elements[wall].rgb[0], elements[wall].rgb[1], elements[wall].rgb[2]);
				}
				for (int col = 0; col < width; col++){ // for every column
						if(col / upscale_factor == 0) // see if the maze column needs to be changed
								idx = 0;
						else
								idx = col / upscale_factor;
						for (int color = 0; color < 3; color++){ // add the R, G and B values
								int index = row * padded_width + col * 3 + color; // find location in the bitmap
								maze_value = M->algo[mazetype].maze[original - 1 - idy][idx];
								if(maze_value == crossover)
										bitmap[index] = elements[crossover].rgb[color];      // case: crossover
								else if(maze_value == breath_first)
										bitmap[index] = elements[breath_first].rgb[color];   // case: bfs algo
								else if(maze_value == wall)
										bitmap[index] = elements[wall].rgb[color];           // case: wall
								else if(maze_value == path)
										bitmap[index] = elements[path].rgb[color];           // case: path
								else if(maze_value == recursive)
										bitmap[index] = elements[recursive].rgb[color];      // case: recursive algo
						}
						if(x_fade_count > 0){ // if a horizontal color fade is present
								for(i = 0; i < 5; i++)
										if(colorfade[i].direction == horizontal)
												for(j = 0; j < 3; j++)
														elements[i].rgb[j] += colorfade[i].fade[j]; // increment the RGB values
						}
						xprevious = idx;
				}
				if(x_fade_count > 0){ // if a horizontal color fade is present
						for(i = 0; i < 5; i++)
								if(colorfade[i].direction == horizontal)
										for(j = 0; j < 3; j++)
												elements[i].rgb[j] = colorfade[i].original[i]; // reset the RGB values
				}
				if(y_fade_count > 0){ // if a vertical color fade is present
						for(i = 0; i < 5; i++)
								if(colorfade[i].direction == vertical)
										for(j = 0; j < 3; j++)
												elements[i].rgb[j] += colorfade[i].fade[j]; // increment the RGB values
				}
				yprevious = idy;
		}
		fwrite(bitmap, bih.image_size * sizeof(char), 1, f); // write the bitmap to the file
		fclose(f); // close the file
        pConsole->Print("upscaling x%d to %d x %d px\n", upscale_factor, height, height); // lets look at the upscale factor
        pConsole->Print("\n"); // \n dont work, prints row
		free(bitmap); // free memory
		return 0;
}

//-----------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void addBorders(short **cells, int size);//prototype, function in ellermaze.c

typedef struct {//struct to number each cell
    int P1;
    int P2;
}cellsets_s;

void findneighbours(int P1, int P2, int *neighbours, int *neighbourcount, short **cells, int length) {
	*neighbourcount = 0;
	if(P2 + 2 <= length && cells[P1][P2+2] == 0){//Checks all neigbours and puts 1 in array if there is a free neighbour in that position
		neighbours[1] = 1;
		(*neighbourcount)++;
	}
	if(P1 + 2 <= length && cells[P1+2][P2] == 0){
		neighbours[2] = 1;
		(*neighbourcount)++;
	}
	if(P1 - 2 >= 1 && cells[P1-2][P2] == 0) {
		neighbours[0] = 1;
		(*neighbourcount)++;
	}
	if(P2 - 2 >= 1 && cells[P1][P2-2] == 0) {
		neighbours[3] = 1;
		(*neighbourcount)++;
	}
}

int searchforcell (short **cells, int length, int* P1, int* P2, int *neighbours, int *neighbourcount, cellsets_s *cellsets, int counter, int visited[length*length/2]){
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

int searchforcell_backtracer (short** cells, int length, int* P1, int* P2, int *neighbours, int *neighbourcount, cellsets_s *cellsets, int counter){
    while(counter != 0){//backtracer always pick the newest cell and if that cell is not suitable, then second newest and so on until it finds a suitable cell.
        findneighbours(cellsets[counter].P1, cellsets[counter].P2, neighbours, neighbourcount, cells, length);
        if (*neighbourcount != 0){
            *P1 = cellsets[counter].P1;
            *P2 = cellsets[counter].P2;
            return 1;
        }
    counter--;
    }
    return 0;
}

void generateTree(short **cells, cellsets_s *cellsets, int length, short algo, short algoback) {
    int *neighbours = calloc(4, sizeof(int *));
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

void treemaze(int length, short **maze, short algo, short algoloop){
    cellsets_s *cellsets = calloc((length + 4)*(length + 4), sizeof(cellsets_s));//array to keep track of visited cells
	generateTree(maze, cellsets, length - 1, algo, algoloop);
	addBorders(maze, length);
    free(cellsets);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "header.h"


void generateEller(short **cells, short **cellsets, int size) {
	srand(time(NULL));
	int wallcounter = 0;
	unsigned int RandNr = 2;
	int setcounter = 0;
	int setbuf;
	int setcount;
	for (int i = 1; i < size; i += 2) {//loop for rows

		for(int a = 1; a < size; a++) { //Generate unique sets
			if(cellsets[i][a] == 0)
				cellsets[i][a] = RandNr++;
		}

		for(int b = 1; b < size-2; b += 2){ 
			if (i == size - 1){//unique algorithm for last row
				if (cellsets[i][b] == cellsets[i][b+2]) //for last row, separate same sets
					cells[i][b+1] = 0;
				else{
					setbuf = cellsets[i][b+2];
					setcount = 1;
					while(setcount != size + 1) { //go through the whole row and assign all sets with same set value to the same set value
						if(cellsets[i][setcount] == setbuf){
							cellsets[i][setcount] = cellsets[i][b]; //assign left set value to right and middle (middle not really necessary, good for debugging)
							setcount += 2;//increment to next set
						}else{
							setcount += 2;//increment to next set
						}
					}
				}
			}else if (cellsets[i][b] != cellsets[i][b+2] && cellsets[i][b+1] != 1){ //If right cell is not equal to left then rand to combine or to not combine
				if (rand() % 2) {//if true, then make path between sets
					setbuf = cellsets[i][b+2];
					setcount = 1;
					while(setcount != size + 1) { //go through the whole row and assign all sets with right set value to left set value
						if(cellsets[i][setcount] == setbuf){
							cellsets[i][setcount] = cellsets[i][b]; //assign left set value to right and middle (middle not really necessary, good for debugging)
							setcount += 2;//increment to next set
						}else{
							setcount += 2;//increment to next set
						}
					}
				}else{
					cells[i][b+1] = 0; //if not true, add wall between sets
					cellsets[i][b+1] = 1;
				}
			}else{
				cells[i][b+1] = 0;
				cellsets[i][b+1] = 1;//if they are already equal then wall is needed (already done before, just in case)
			}
		}

		for (int c = 1; c < size; c += 2) {
			cells[i+1][c+1] = 0; //add walls between row and column sets for filler (because of matrix characteristics)
			cellsets[i+1][c+1] = 1;
			if (cellsets[i][c] == cellsets[i][c+2] && c != size - 1){//if sets are same. find whole set from row
				setcounter += 2;
			}else{
				for (int d = 0; d <= setcounter; d += 2) {
					if (d == setcounter && wallcounter == setcounter) { //if at the end of set and all previous floors set
						cellsets[i+2][c - setcounter + d] = cellsets[i][c - setcounter +d]; //at least 1 cell in set needs a path (no floor)
					}else if (rand() % 2) {
						cellsets[i+2][c - setcounter + d] = cellsets[i][c - setcounter +d]; //make path and assign above value to bottom set
					}else{
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

void addBorders( short **cells, int size){
	for (int b = 0; b < size; b++)//add left border
		cells[b][0] = 0;
	
	for (int a = 0; a < size; a++)//add top border
        cells[0][a] = 0;

    int a = rand() % (size - 2);//make entrance on top
    while (cells[1][a] == 0)
        a++;
    cells[0][a] = 1;

	for (int c = size-2; c >= 0; c--)//add bottom border
		cells[size - 1][c] = 0;

    int c = rand() % (size - 2);//make exit on bottom
    while (cells[size - 2][c] == 0)
        c++;
    cells[size - 1][c] = 1;

	for (int d = 0; d < size; d++)//add right border
		cells[d][size - 1] = 0;
}

void Ellermaze(int size, short **maze){
	int i, j;
	short **cellsets = calloc(size+4, sizeof(short*));//create matrix for set identification
	for(i = 0; i < size + 4; i++)
		cellsets[i] = calloc(size+4, sizeof(short));

    for(i = 0; i < size; i++) {//change maze matrix to all 1-s because algorithm will add walls to white canvas
        for (j = 0; j < size; j++) {
            maze[i][j] = 1;
        }
    }
	generateEller(maze, cellsets, size - 1);
	addBorders(maze, size);
	for(i = 0; i < size + 4; i++)//free set identifications
		free(cellsets[i]);
}

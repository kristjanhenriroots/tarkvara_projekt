#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define BILLION  1000000000.0

void eliminate(int size, short **maze, int x, int y){ // follow a dead end til an intersection
    int free = 0;
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

double deadEnd(int size, short **maze, short exits[4]){
    struct timespec start, end;
    int deadend, x, y;
    clock_gettime(CLOCK_REALTIME, &start); // start timer
    // 1 = passage, 0 = wall
    for(y = 0; y < size; y++){
        for(x = 0; x < size; x++){
            deadend = 0;
            if((exits[1] == y && x == exits[0]) || (y == exits[3] && x == exits[2])){ // if it thinks an entrance or exit is a dead end, skip an interation
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
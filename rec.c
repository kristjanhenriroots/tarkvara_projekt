#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BILLION  1000000000.0

// https://www.codesdope.com/blog/article/backtracking-to-solve-a-rat-in-a-maze-c-java-pytho/
int solvemaze(int y, int x, int endy, int endx, int size, short **maze, short **solution){
    // checking if it has reached the end
    if((y == endy) && (x == endx)){
        solution[y][x] = 1;
        return 1;
    }
    // is it within bounds and has it not been here before
    if(y >= 0 && x >= 0 && y < size && x < size && solution[y][x] == 0 && maze[y][x] == 1){
        solution[y][x] = 1;
        //going down
        if(solvemaze(y + 1, x, endy, endx, size, maze, solution))
            return 1;
        //going right
        if(solvemaze(y, x + 1, endy, endx, size, maze, solution))
            return 1;
        //going up
        if(solvemaze(y - 1, x, endy, endx, size, maze, solution))
            return 1;
        //going left
        if(solvemaze(y, x - 1, endy, endx, size, maze, solution))
            return 1;
        //backtracking
        solution[y][x] = 0; // can't go anywhere, dead end, go back
        return 0;
    }
    return 0;
}

double recursion(int size, short **raw, short **sol, short exits[4]){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start); // start timer
    solvemaze(exits[1], exits[0], exits[3], exits[2], size, raw, sol);
    clock_gettime(CLOCK_REALTIME, &end); // stop timer
    // time_spent = end - start
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION * 1000;
    return time_spent;
}
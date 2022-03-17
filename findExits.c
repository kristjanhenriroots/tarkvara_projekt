#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void findExits(int size, short **maze, short exits[4]){
    exits[0] = 0;
    exits[2] = 0; // not sure if exit is on the bottom
    for(int x = 0; x < size; x++){
        if(maze[0][x] == 1){
            exits[0] = x;
            exits[1] = 0;
        } 
        if(maze[size - 1][x] == 1){
            exits[2] = x;
            exits[3] = size - 1;
        }
    }
    if(exits[0] == 0){
        printf("There is no entrance!!\n");
        exit(0);
    }
    if(exits[2] == 0){ // exit wasn't on the bottom, checking right side
        for(int y = 0; y < size - 1; y++){
            if(maze[y][size - 1] == 1){
                exits[2] = size - 1;
                exits[3] = y;
            }
        }
    }
    if(exits[2] == 0){
        printf("There is no exit!!\n");
        exit(0);
    }
}
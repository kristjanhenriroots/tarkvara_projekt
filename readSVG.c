#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWMAX 100
#define COLCOUNT 10

typedef struct data{
    short **maze; // maze matrix
}data_t;

typedef struct dataArr{
    short exits[4]; // mark entrance and exit coordinates
    int size; // maze size
    int algoCount; // the number of solving algorithms + 2 for generation and final solution 
    data_t *algo; // array of maze matrixes
}maze_t;

void feedMemory(maze_t *M);
void freeMemory(maze_t *M);

int breakIt(char *format[COLCOUNT], char *row){
    //printf("breaking it\n");
    int i = 0;
    char *token = strtok(row, "\"");
    while(token != NULL){ // lõhun rea eraldi elementideks
        format[i++] = token;
        token = strtok(NULL, "\"");
    }
    return i;
}

short findElement(char *input){
    static char elements[5][15] = { {"fill:black"}, {"fill:white"}, {"fill:red"}, {"fill:blue"}, {"fill:purple"} };
    for(int i = 0; i < 5; i++)
        if(strcmp(input, elements[i]) == 0)
            return i;
    return -1;
}

int readSVG(char *filename, maze_t *M){
    char ans = 'y';
    int file = 0;
    int solved = 0;
    FILE *svg;
    svg = fopen(filename, "r"); // trying to open file
    if(svg == NULL){  // failed
        while(file == 0){
            printf("Failed to open %s\n", filename);
            printf("Would you like to try again? y/n\n");
            scanf("%c", &ans);
            if(ans == 'n')
                return 0;
            else if(ans == 'y'){
                printf("Enter filename: ");
                scanf("%s", filename);
                svg = fopen(filename, "r");
                if(svg != NULL)
                    break;
            }
        }
    }
    char xml_header[ROWMAX] = {"<?xml version=\"1.0\"?>\n"}; // xml header
    char row[ROWMAX];
    fgets(row, ROWMAX, svg);
    if(strcmp(xml_header, row) != 0){ // checking if inserted file is svg
        printf("%s is not the correct file type!\n", filename);
        return 0;
    }
    char *format[COLCOUNT];
    printf("reached here\n");

    breakIt(format, fgets(row, ROWMAX, svg)); // getting maze size
    enum color_values{black, white, red, blue, purple};
    enum svg_values{x = 1, y = 3, color = 9}; // format array indexes where to find a certain value
    

    M->size = atoi(format[1]); // got the maze size from svg file
    feedMemory(M); // giving sufficient memory
    short element;
    fgets(row, ROWMAX, svg); // getting rid of unnecessary info in the svg header
    for(int i = 0; i < M->size * M->size; i++){
        breakIt(format, fgets(row, ROWMAX, svg));
        element = findElement(format[color]);
        if(element == -1){
            printf("Unknown value present\n");
            freeMemory(M);
            M->size = 0;
            return 0;
        }
        else if(i < M->size && element > 1) // checking if the maze is already solved, only need to check the first row
            solved = 1;
        M->algo[0].maze[atoi(format[y])][atoi(format[x])] = element;
    }
    if(solved == 1){
        printf("Warning, maze is already solved\n");
    }
    printf("SVG file successfully read\n");
    fclose(svg);
    return solved;
}
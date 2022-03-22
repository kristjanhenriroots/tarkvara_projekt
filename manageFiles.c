#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

enum maze_type{unsolved_maze, solved_maze};

FILE *getFilename(int mode, int filetype, int complexity, int mazetype){    // getting the filename
    FILE *f = NULL;
    char default_filenames[2][NAMELEN]  = { {"raw"},  {"solved"} };
    char default_extensions[3][5]       = { {".txt"}, {".svg"}, {".bmp"} }; // all possible combiantions of default files and user questions
    char chat_mode[2][MENUOPTION]       = { {"read"}, {"write"} };
    char chat_file[3][MENUOPTION]       = { {"text"}, {"svg"}, {"bitmap"} };
    char chat_grammar[2][MENUOPTION]    = { {"from"}, {"to"} };
    if(mazetype == final_maze)
        mazetype = 1;
    printf("mazetype is %d\n", mazetype);
    if(complexity == ask_user){
        char ans;
        printf("Would you like to select personal filenames? y/n\n");
        scanf("%c", &ans);
        if(ans == 'y')
            complexity = manual;
        else if(ans == 'n')
            complexity = automatic;
        else{
            printf("Invalid input\n");
            return 0;
        }
    }
    if(complexity == automatic){                                            // using default files like "raw.svg"
        printf("opening file %s\n",strcat(default_filenames[mazetype], default_extensions[filetype]));
        if(filetype == bmp_file)
            f = fopen(default_filenames[mazetype], "wb");
        else    
            f = fopen(default_filenames[mazetype], "r+");
        if(f == NULL){
            printf("Error automaticly opening file\n");                     // not supposed to go wrong
            return f;
        }
        return f;
    }
    int file = 0;
    char ans;
    char buf[NAMELEN];
    printf("Where would you like to %s the %s %s file %s?", chat_mode[mode], chat_file[filetype], default_filenames[mazetype], chat_grammar[mode]);
    scanf("%s", buf);
    if(filetype == bmp_file)
        f = fopen(buf, "wb");
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
    return f;
}

int readTXT(FILE *f, maze_t *M){    // reading maze from a text file 
    fscanf(f, "%d", &M->size);      // getting maze size
    sizeCheck(M->size);             // seeing if size is allowed
    feedMemory(M);                  // giving enough memory
    int i, j, solved = 0;
    for(i = 0; i < M->size; i++){
        for(j = 0; j < M->size; j++){
            fscanf(f, "%hd", &M->algo[generated].maze[i][j]);
            if(i == 0 && M->algo[generated].maze[i][j] > 1){ // checking if input is an already solved maze, only need to check the first row
                solved = 1;
            }
            if(M->algo[generated].maze[i][j] != 0 && M->algo[generated].maze[i][j] != 1){ // thats not our maze file!
                printf("Incorrect file!!\n");
                return -1;
            } 
        }
    }
    if(solved == 1){
        printf("Warning, maze is already solved\n");
    }
    printf("Maze read from text file\n");
    fclose(f);
    return solved;
}

int writeTXT(FILE *f, maze_t *M, int mazetype){ // writing to a txt file
    if(M->size == 0){
        printf("No maze generated!\n"); // let's see if something is actually generated
        return 0;
    }
    /*
    ans = 0
    if(solved > 0){ // maze has a solution
        printf("Would you like to save the solution (1), the unsolved maze (2) or both (3)?\n");
        scanf("%d", &ans);
        while(ans != 1 && ans != 2 && ans != 3){
            printf("Invalid input\nTry again: ");
            scanf("%d", &ans);
        } 
    }
    */
    
    fprintf(f, "%d\n", M->size); // printing maze size
    int i, j;
    for(i = 0; i < M->size; i++){
        for(j = 0; j < M->size; j++){
            fprintf(f, "%d ", M->algo[mazetype].maze[i][j]);
        }
        fprintf(f, "\n");   
    }
    printf("Maze saved to textfile\n");
    fclose(f);
    return 1;
}

// necessary subfunctions for SVG file reading
int breakIt(char *format[COLCOUNT], char *row){ // breaking fgets with strtok
    int i = 0;
    char *token = strtok(row, "\"");            // separator "
    while(token != NULL){                       // breaking row into elements
        format[i++] = token;
        token = strtok(NULL, "\"");
    }
    return i;
}

short findElement(char *input){                 // finding the color value
    static char elements[5][15] = { {"fill:black"}, {"fill:white"}, {"fill:red"}, {"fill:blue"}, {"fill:purple"} };
    for(int i = 0; i < 5; i++)                  // check all possible colors
        if(strcmp(input, elements[i]) == 0)
            return i;                           // return the index aka the color value in maze array
    return -1;                                  // incorrect SVG file was fed
}

int readSVG(FILE *svg, maze_t *M){// read from a SVG file
    int solved = 0;                             // marking if the scanned maze is solved or not
    char xml_header[ROWMAX] = {"<?xml version=\"1.0\"?>\n"}; // xml header
    char row[ROWMAX];
    fgets(row, ROWMAX, svg);
    if(strcmp(xml_header, row) != 0){           // checking if inserted file is svg
        printf("incorrect file type inserted!\n");
        return 0;
    }
    char *format[COLCOUNT];                     //keeping strtok result here
    enum color_values{black, white, red, blue, purple};
    enum svg_values{x = 1, y = 3, color = 9};   // format array indexes where to find a certain value
    
    breakIt(format, fgets(row, ROWMAX, svg));   // getting maze size
    int nr_format = atoi(format[1]);
    if(sizeCheck(nr_format) == 0)         // seeing if size is allowed
        return -1;
    M->size = nr_format;                  // got the maze size from svg file
    feedMemory(M);                              // giving sufficient memory
    short element;
    fgets(row, ROWMAX, svg);                    // getting rid of unnecessary info in the svg header
    for(int i = 0; i < M->size * M->size; i++){ // scanning everything from the file
        breakIt(format, fgets(row, ROWMAX, svg));
        element = findElement(format[color]);
        if(element == -1){
            printf("Unknown value present\n");  // value not supposed to be in svg file
            freeMemory(M);                      // free memory and set size to 0 again
            M->size = 0;
            return -1;
        }
        else if(i < M->size && element > 1)     // checking if the maze is already solved, only need to check the first row
            solved = 1;
        M->algo[generated].maze[atoi(format[y])][atoi(format[x])] = element; // write maze data
    }
    if(solved == 1){                                    // maze is already solved, just warn the user
        printf("Warning, maze is already solved\n");
        M->algo[final_maze].maze = M->algo[generated].maze;    // if its solved it'll be added to appropriate place in the array
    }
    printf("SVG file successfully read, maze size %d\n", M->size);
    fclose(svg);
    return solved;
}

int writeSVG(FILE *svg, maze_t *M, int mazetype){
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
    return 1;
}

int manageFiles(int mode, int filetype, int complexity, int mazetype, maze_t *M){
    if(mode == read && M->size > 0){ // something is already in memory
        printf("Overwriting previous maze\n");
        freeMemory(M);
    }
    int success;
    switch(mode){
        case read:
            switch(filetype){
                case txt_file:
                    success = readTXT(getFilename(read, filetype, complexity, mazetype), M);
                    break;
                case svg_file:
                    success = readSVG(getFilename(read, filetype, complexity, mazetype), M);
                    break;
            }
            break;
        case write:
            switch(filetype){
                case txt_file:
                    success = writeTXT(getFilename(read, filetype, complexity, mazetype), M, mazetype);
                    break;
                case svg_file:
                    success = writeSVG(getFilename(read, filetype, complexity, mazetype), M, mazetype);
                    break;
                case bmp_file:
                    success = makeBMP(getFilename(read, filetype, complexity, mazetype), regular, mazetype, M);
                    break;
            }
            break;
        
    }

    return success;
}
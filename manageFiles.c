#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

/* 
    mode        = read / write
    filetype    = txt / svg / bmp
    complexity  = ask for filenames / don't / ask user
    mazetype    = raw or solved
    M           = maze struct

*/
FILE *getFilename(int mode, int filetype, int complexity, int mazetype){    // getting the filename
    FILE *f = NULL;
    char default_filenames[2][NAMELEN]  = { {"raw"},  {"solved"} };
    char default_extensions[3][7]       = { {".txt"}, {".svg"}, {".bmp"} }; // all possible combiantions of default files and user questions
    char chat_mode[2][MENUOPTION]       = { {"read"}, {"write"} };
    char chat_file[3][MENUOPTION]       = { {"text"}, {"svg"}, {"bitmap"} };
    char chat_grammar[2][MENUOPTION]    = { {"from"}, {"to"} };
    if(mazetype == final_maze)
        mazetype = 1;
    //printf("mazetype is %d\n", mazetype);
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
    if(complexity == automatic){                                            // using default files like "raw.svg"
        printf("opening file %s\n",strcat(default_filenames[mazetype], default_extensions[filetype]));
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
            printf("Error automatically opening file\n");                     // not supposed to go wrong
            return NULL;
        }
        return f;
    }
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
    return f;
}

int readTXT(FILE *f, maze_t *M){    // reading maze from a text file 
    short maze_element;
    if(f == NULL)
        return -1;
    fscanf(f, "%d", &M->size);      // getting maze size
    sizeCheck(M->size);             // seeing if size is allowed
    feedMemory(M);                  // giving enough memory
    int i, j, solved = 0;
    for(i = 0; i < M->size; i++){
        for(j = 0; j < M->size; j++){
            fscanf(f, "%hd", &maze_element);
            if(i == 0 && maze_element > 1){ // checking if input is an already solved maze, only need to check the first row
                solved = 1;
            }
            if(maze_element < wall && maze_element > crossover){ // thats not our maze file!
                printf("Incorrect file!!\n");
                return -1;
            } 
            M->algo[generated].maze[i][j] = maze_element;
        }
    }
    
    printf("Maze read from text file\n");
    fclose(f);
    return solved; // returning whether the read maze was already solved or not
}

int writeTXT(FILE *f, maze_t *M, int mazetype){ // writing to a txt file
    if(M->size == 0){
        printf("No maze generated!\n"); // let's see if something is actually generated
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
    printf("Maze saved to text file\n");
    fclose(f);
    return 0;
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
    char xml_header[ROWMAX] = {"<?xml version=\"1.0\"?>"}; // xml header
    int len = strlen(xml_header);
    char row[ROWMAX];
    fgets(row, ROWMAX, svg);
    printf("%s\n", row);
    if(strncmp(xml_header, row, len) != 0){           // checking if inserted file is svg
        printf("incorrect file type inserted!\n");
        return -1;
    }
    char *format[COLCOUNT];                     //keeping strtok result here
    enum color_values{black, white, red, blue, purple};
    enum svg_values{x = 1, y = 3, color = 9};   // format array indexes where to find a certain value
    
    breakIt(format, fgets(row, ROWMAX, svg));   // getting maze size
    int nr_format = atoi(format[1]);
    printf("SVG file maze size is %d\n", nr_format);
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
    
    printf("SVG file successfully read, maze size %d\n", M->size);
    fclose(svg);
    return solved; // return is maze was solved or not
}

int writeSVG(FILE *svg, maze_t *M, int mazetype){ // write to SVG file
    if(M->size == 0){
        printf("No maze generated!\n"); // let's see if something is actually generated
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
int manageFiles(int mode, int filetype, int complexity, int mazetype, maze_t *M){
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
                    success = makeBMP(getFilename(write, filetype, complexity, mazetype), regular, mazetype, M);
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
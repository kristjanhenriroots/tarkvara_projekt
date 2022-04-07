#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "header.h"

// lets make the code more readable



int sizeCheck(int size){
    if(size < MAZEMIN || size > MAZEMAX || size % 2 == 0){
        printf("Not allowed size!\n");
        return 0;
    }
    return 1;
}

int getSize(maze_t *M){ // ask the user for the size
    int size;
    printf("Insert maze size (must be an odd number!!): ");
	scanf("%d", &size);
    if(sizeCheck(size) == 0)
        return 0;
    M->size = size;
    return 1;
}

void feedMemory(maze_t *M){ // allocate memory for mazes
    int i, j;
    short **buf;
    short *bufrow;
    for(i = 0; i < M->algoCount; i++){
        buf = (short**)calloc(M->size, (M->size) * sizeof(short*));
        if (buf == NULL){
            printf("Error allocating memory\n");
            exit(0);
        }
        M->algo[i].maze = buf; // feeding the behemoth memory
        for(j = 0; j < M->size; j++) {
            bufrow = (short *) calloc(M->size, (M->size) * sizeof(short));
            if (bufrow == NULL) {
                printf("Error allocating memory\n");
                exit(0);
            }
            M->algo[i].maze[j] = bufrow;
        }
    }
}

void freeMemory(maze_t *M){ // free the memory
    for(int i = 0; i < M->algoCount; i++){
        for(int j = 0; j < M->size; j++){
            free(M->algo[i].maze[j]);
        }
        free(M->algo[i].maze);
    }
    printf("Memory freed\n");
    M->size = 0;
}

void printMain(){ // Commands
    printf("1: Generate labyrinth\n"
        "2: Solve labyrinth\n"
        "3: Read from file\n"
        "4: Save to file\n"
        "H: Help\n"
        "X: Exit\n");
        // ~: secret menu :o
}

int generateMenu(maze_t *M){ // user wanted to generate a maze
    struct timespec start, end; // keep accurate time
    enum genalgo{null, eller, tree, prim};
    if(M->size > 0){ // something is already generated
        printf("Overwriting previous maze\n");
        freeMemory(M);
    }
    short algo, algoloop;
    printf(" 1: Eller's algorithm\n");
    printf(" 2: Growing tree backtracer implementation\n");
    printf(" 3: Growing tree Prim implementation\n");
    printf("-> ");
    scanf("%hd", &algo);
    if(algo != eller && algo != tree && algo != prim){
        printf("Invalid input\n");
        return 0;
    }
    if(algo == 2){
        printf(" 1: Without loops\n");
        printf(" 2: With loops\n");
        printf("-> ");
        scanf("%hd", &algoloop);
        if(algoloop != 1 && algoloop != 2){      
            printf("Invalid input\n");
            return 0;
        }
    }
    if(getSize(M) == 0) // get the size the user wants
        return 0;
    feedMemory(M); // give sufficient memory
    if(algo == eller){
        clock_gettime(CLOCK_REALTIME, &start); // start timer
        Ellermaze(M->size, M->algo[0].maze); // make a maze using Eller's algorithm
    }
    else if(algo == tree || algo == prim){
        clock_gettime(CLOCK_REALTIME, &start); // start timer
        treemaze(M->size, M->algo[0].maze, algo, algoloop); // make a maze using Auris Prääm's TM generation algorithm
    }
    clock_gettime(CLOCK_REALTIME, &end); // stop timer
    printf("Generation successful\n");
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION; // calculate time spent in milliseconds
    
    printf("Generation time %.4f ms\n", time_spent * 1000);    
    makeBMP(getFilename(write, bmp_file, automatic, generated), regular, generated, M); // also make a BMP

    return 0; 
}

int solveMenu(maze_t *M){ // user wants to solve a maze
    if(M->size == 0){ // see if there actually is a maze to solve
        printf("No maze to solve!\n");
        return 0;
    }
    int i, j, k;
    double tdead, trec, tbfs; // solving times
    for(j = 0; j < M->size; j++){
        for(k = 0; k < M->size; k++){
            M->algo[1].maze[j][k] = M->algo[0].maze[j][k]; // copy the values for dead end filler, it will destroy the original
        }
    }
    findExits(M->size, M->algo[generated].maze, M->exits); // does it have an exit?
    
    tdead = deadEnd(M->size, M->algo[deadend].maze, M->exits);
    trec = recursion(M->size, M->algo[generated].maze, M->algo[recursive].maze, M->exits); // measuring time and solving
    tbfs = bfs(M->size, M->algo[generated].maze, M->algo[breath_first].maze, M->exits); // measuring time and solving

    printf("\nDead end solution %.4f ms\n", tdead * 1000);
    printf("Recursive backtracker %.4f ms\n", trec * 1000);
    printf("Breath first search %.4f ms\n\n", tbfs * 1000);

    for(i = 0; i < final_maze; i++){
        if(i == 1) // don't want dead end filler showing
            continue;
        for(j = 0; j < M->size; j++)
            for(k = 0; k < M->size; k++)
                M->algo[final_maze].maze[j][k] +=  M->algo[i].maze[j][k]; // adding all layers up for the final solution
    }
    makeBMP(getFilename(write, bmp_file, automatic, final_maze), regular, final_maze, M); // also make a BMP

    /*
    for(i = 0; i < M->algoCount; i++){
        for(j = 0; j < M->size; j++){
            for(k = 0; k < M->size; k++)
                printf("%hd ", M->algo[i].maze[j][k]); // to print everything if you so please
            printf("\n");
        }
        printf("\n");
    }
    */
    
    printf("Solving complete, see program directory for raw.bmp and solved.bmp\n");
    return 0;
}

void mazeReset(maze_t *M){ // function to reset solved mazes, used when reading in already solved mazes
    for(int i = 0; i < M->size; i++){
        for(int j = 0; j < M->size; j++){
            if(M->algo[generated].maze[i][j] > path)
                M->algo[generated].maze[i][j] = path; // remove the solutions, just set them to 1 aka a path
            M->algo[final_maze].maze[i][j] = 0;       // reset the final solution maze
        }
    }
}

int main(void){
    maze_t M;// if you move this line below the int definition the program crashes????
    int inloop = 1, solved = 0, usr_filetype;
    //manageFiles(read, txt_file, ask_user, generated, &M);
	//getSize(&M);
    M.algoCount = 5; // current nr
    M.size = 0;
    printMain(); // print the commands
    while(inloop){
        char input;
        scanf("%c", &input);
        switch(input){
            case '1': // generate a maze
                generateMenu(&M);
                solved = 0;
                break;
            case '2': // solve a maze
                if(solved == 0){ // no need to solve something twice
                    solveMenu(&M); 
                    solved++;
                }
                else{
                    printf("Already solved!\n");
                }
                break;
            case '3': // read from file
                printf("Read from:\n 1. TXT file\n 2. SVG file\n");
                scanf("%d", &usr_filetype);
                usr_filetype--;
                if(usr_filetype != txt_file && usr_filetype != svg_file){
                    printf("Invalid input\n");
                    break;
                }
                solved = manageFiles(read, usr_filetype, manual, generated, &M);
                break;
                
            case '4': // save to file
                
                printf("Write to:\n 1. TXT file\n 2. SVG file\n 3. BMP file\n");
                scanf("%d", &usr_filetype);
                usr_filetype--;
                if(usr_filetype < txt_file || usr_filetype > bmp_file){
                    printf("Invalid input\n");
                    break;
                }
                if(solved > 0){
                    int times;
                    enum xsave{save_raw = 1, save_sol = 2, save_both = 3};
                    printf("Would you like to save the raw file(1), the solution(2) or both(3)?\n");
                    scanf("%d", &times);
                    if(times < save_raw || times > save_both)
                        printf("Invalid input\n");
                    switch(times){
                        case save_raw:
                            manageFiles(write, usr_filetype, ask_user, generated, &M);
                            break;
                        case save_sol:
                            manageFiles(write, usr_filetype, ask_user, final_maze, &M);
                            break;
                        case save_both:
                            printf("Generated:\n");
                            manageFiles(write, usr_filetype, ask_user, generated, &M);
                            printf("Solved:\n");
                            manageFiles(write, usr_filetype, ask_user, final_maze, &M);
                            break;
                        default:
                            break;
                    }
                }
                else
                    manageFiles(write, usr_filetype, ask_user, generated, &M);
                break;
            
            case 'H': // print commands again
                printMain();
                break;
            case 'X': // exit program
                printf("Exiting program\n");
                inloop = 0;
                break;
            case '~': {// secret menu
                if(M.size == 0){
                    printf("Generate something first ;)\n");
                    break;
                }
                usr_filetype = 0;
                if(solved > 0){
                    printf("Would you like to tweak the raw(1) or the solution (2)?\n");
                    scanf("%d", &usr_filetype);
                    if(usr_filetype != 1 && usr_filetype != 2)
                        break;
                }
                if(usr_filetype == 2)
                    makeBMP(getFilename(write, bmp_file, ask_user, final_maze), secret, final_maze, &M);
                else
                    makeBMP(getFilename(write, bmp_file, ask_user, generated), secret, generated, &M);
                break; 
            }
            case '^':
                if(M.size == 0){
                    printf("Generate something first ;)\n");
                    break;
                }
                makeBMP(getFilename(write, bmp_file, ask_user, generated), insanity, generated, &M);
            break;
        }
    }
    if(M.size > 0) // immediately leaving without generating would crash as the memory would be empty
        freeMemory(&M);
    return 0;
}


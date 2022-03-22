#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// to do: remove svg file creation from maze generation, use the same svg function at the end of the program

#define NAMELEN 30
#define MAZEMIN 5
#define MAZEMAX 100000
#define BILLION  1000000000.0 // for keeping time on generation algorithms

typedef struct data{
    short **maze; // maze matrix
}data_t;

typedef struct dataArr{
    short exits[4]; // mark entrance and exit coordinates
    int size; // maze size
    int algoCount; // the number of solving algorithms + 2 for generation and final solution 
    data_t *algo; // array of maze matrixes
}maze_t;

// lets make the code more readable
enum algoArr{generated, deadend, recursive, breath_first, final_maze}; // the same order the mazes are in the struct array
enum bitmap_mode{regular, secret};

void Ellermaze(int size, short **maze); // maze generation
void treemaze(int size, short **maze, short algo, short algoloop);
void findExits(int size, short **maze, short exits[4]); // finding the entrance and exit
double deadEnd(int size, short **maze, short exits[4]); // using the dead end filler algorithm
double recursion(int size, short **raw, short **sol, short exits[4]); // recursive backtracker
double bfs(int size, short **raw, short **sol, short exits[4]);
void makeSVG(char *filename, int size, short **maze); // making the SVG file
int makeBMP(int height, short **maze, int mode, int present_elements); // experimental, BMP file creation


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
    for(i = 0; i < M->algoCount; i++){
        M->algo[i].maze = (short**)calloc(M->size, (M->size) * sizeof(short*)); // feeding the behemoth memory
        for(j = 0; j < M->size; j++)
            M->algo[i].maze[j] = (short*)calloc(M->size, (M->size) * sizeof(short));
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

int readFile(maze_t *M){ // reading maze from a file
    if(M->size > 0){ // something is already in memory
        printf("Overwriting previous maze\n");
        freeMemory(M);
    }
    int i, j;
    char filename[NAMELEN];
    printf("Enter filename from where to read: "); // filename is user input
    scanf("%s", filename);
    FILE *f = fopen(filename, "r");
	if(f == NULL){
		printf("failed to open file\n");
		exit(0);
	}
    fscanf(f, "%d", &M->size); // getting maze size
    sizeCheck(M->size);
    feedMemory(M);
    for(i = 0; i < M->size; i++){
        for(j = 0; j < M->size; j++){
            fscanf(f, "%hd", &M->algo[generated].maze[i][j]);
            if(i == 0 && M->algo[generated].maze[i][j] > 1){ // checking if input is an already solved maze, only need to check the first row
                printf("Maze is already solved!\n");
                return 0;
            }
            if(M->algo[generated].maze[i][j] != 0 && M->algo[generated].maze[i][j] != 1){ // thats not our maze file!
                printf("Incorrect file!!\n");
                return 0;
            } 
        }
    }
    printf("Maze read from %s\n", filename);
    fclose(f);
    return 0;
}

int writeFile(maze_t *M, int solved){ // writing to a txt file
    if(M->size == 0){
        printf("No maze generated!\n"); // let's see if something is actually generated
        return 0;
    }
    int i, j, save = 0, ans = 0;
    char filename[NAMELEN];
    if(solved > 0){ // maze has a solution
        printf("Would you like to save the solution (1), the unsolved maze (2) or both (3)?\n");
        scanf("%d", &ans);
        if(ans != 1 && ans != 2 && ans != 3){
            printf("Invalid input\n");
            return 0;
        } 
    }
    if(ans > 1){ // solution is saved
        save = final_maze;
        printf("Enter filename where to write solution: ");
    }
    else{ // unsolved maze is saved
        printf("Enter filename where to write unsolved maze: ");
    }
    scanf("%s", filename);
    FILE *f = fopen(filename, "w");
	if(f == NULL){
		printf("failed to open file\n");
		exit(0);
	}
    
    fprintf(f, "%d\n", M->size); // printing maze size
    for(i = 0; i < M->size; i++){
        for(j = 0; j < M->size; j++){
            fprintf(f, "%d ", M->algo[save].maze[i][j]);
        }
        fprintf(f, "\n");   
    }
    printf("Maze saved to %s\n", filename);
    fclose(f);
    if(ans == 3){
        writeFile(M, 0); // user wanted both files, recursion insues
    }
    return 0;
}


void printMain(){ // Commands
    printf("1: Generate labyrinth\n"
        "2: Solve labyrinth\n"
        "3: Read from .txt file\n"
        "4: Save to .txt file\n"
        "H: Help\n"
        "X: Exit\n");
        // 5: free memory (for now)
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
    //makeSVG("raw.svg", M->size, M->algo[0].maze, M->algo[3].maze); // make an SVG of the raw maze
    
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION; // calculate time spent in milliseconds
    
    printf("Generation time %.4f ms\n", time_spent * 1000);    

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
        
    //makeSVG("solved.svg", M->size, M->algo[final_maze].maze); // make an SVG of the solution
    makeBMP(M->size, M->algo[final_maze].maze, regular, 5); // also make a BMP

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
    
    printf("Solving complete, see program directory for raw.svg and solved.svg\n");
    return 0;
}

int main(void){
    int inloop = 1, solved = 0;
    maze_t M;
	//getSize(&M);
    M.algoCount = 5; // current nr
    M.size = 0;
    printMain(); // print the commands
    while(inloop){
        char input[1];
        scanf("%c", &input[0]);
        switch(input[0]){
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
                readFile(&M);
                solved = 0;
                break;
            case '4': // save to file
                writeFile(&M, solved);
                break;
            case 'H': // print commands again
                printMain();
                break;
            case 'X': // exit program
                printf("Exiting program\n");
                inloop = 0;
                break;
            case '5': // free memory
                freeMemory(&M);
                break;
            case '~': // secret menu
                if(M.size == 0){
                    printf("Generate something first ;)\n");
                    break;
                }

                if(solved == 1)
                    makeBMP(M.size, M.algo[final_maze].maze, secret, 5);
                else
                    makeBMP(M.size, M.algo[generated].maze, secret, 2);            
                break;
        }
    }
    if(M.size > 0) // immediately leaving without generating would crash as the memory would be empty
        freeMemory(&M);
    return 0;
}


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>


#define BILLION  1000000000.0
// A structure to represent a queue
struct Queue{
    int front, rear, size;
    unsigned capacity;
    int* array;
};
 
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity){
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
        queue->capacity * sizeof(int));
    return queue;
}
 
// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue){
    return (queue->size == queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue){
    return (queue->size == 0);
}
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item){
    if(isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}
 
// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue){
    if(isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
 
// Function to get front of queue
int front(struct Queue* queue){
    if(isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
int rear(struct Queue* queue)
{
    if(isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

void adjacency(int size, int adj_size, short **sol, short **raw, short **adj_matrix){
    int i, j, nr = 0;
    int x, y;

    for(y = 0; y < size; y++){
        for(x = 0; x < size; x++){
            printf("%3hd ", raw[y][x]);
        }
        printf("\n");
    }
    printf("\n\n");
    
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            sol[i][j] = nr;
            nr++;
        }
    }
    //printf("\n\n");
    int ite = 0;
    for(y = 0; y < size; y++){
        for(x = 0; x < size; x++){
            if(raw[y][x] == 1){ // current position is a passage
                // check all directions
                if(y > 0 && raw[y - 1][x] == 1){
                    adj_matrix[sol[y][x]][sol[y - 1][x]] = 1;
                }
                if(x + 1 < size && raw[y][x + 1] == 1){
                    adj_matrix[sol[y][x]][sol[y][x + 1]] = 1;
                }
                if(x < 0 && raw[y][x - 1] == 1){
                    adj_matrix[sol[y][x]][sol[y][x - 1]] = 1;
                }
                if(y + 1 < size && raw[y + 1][x] == 1){
                    adj_matrix[sol[y][x]][sol[y + 1][x]] = 1;
                }
            }
        }
    }
    printf("finished the loop\n");
}

void findPath(int size, short **sol, short adjacent[size][size], short previous[size][size], short exits[4]){
    int x = exits[2], y = exits[3];
    int next = previous[exits[3]][exits[2]];
    printf("\nint the place %d\n", previous[exits[3]][exits[2]]);
    printf("next is %d\n", next);
    while(x != exits[0] || y != exits[1]){
        //printf("x %d y %d\n", x, y);
        sol[y][x] = 1;
        
        //printf("next %d\n", next);
        // checking all sides
        if(y > 0 && adjacent[y - 1][x] == next){
            printf("up\n");
            y--;
        }
        else if(x + 1 < size && adjacent[y][x + 1] == next){
            x++;
            printf("right\n");

        }
        else if(x > 0 && adjacent[y][x - 1] == next){
            x--;
            printf("left\n");

        }
        else if(y + 1 < size && adjacent[y + 1][x] == next){
            y++;
            printf("down\n");

        }
        printf("%d\n", previous[y][x]);
        
        next = previous[y][x];
    }
    printf("quit at %d %d", x, y);
    sol[y][x] = 1;
}

int solve(int size, int adj_size, short **raw, short **sol, short **adj_matrix, short exits[4]){
    struct Queue* rowqueue = createQueue(1000);
    struct Queue* colqueue = createQueue(1000);
    int r, c, nodes_in_current = 1, nodes_in_next, moves = 0;
    int rr, cc;
    short visited[size][size];
    short previous[size][size];
    short adjacent[size][size];
    short dr[4] = {-1, 1, 0, 0};
    short dc[4] = {0, 0, 1, -1};
    int i, j, nr = 0;

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            previous[i][j] = 0;
            visited[i][j] = 0;
            adjacent[i][j] = nr;
            nr++;
        }
    }
    
    enqueue(rowqueue, exits[1]); // queueing the start
    enqueue(colqueue, exits[0]);
    printf("starting at %d %d\n", exits[0], exits[1]);
    while(rowqueue->size > 0){ // works until there are nodes to visit
        r = dequeue(rowqueue);
        c = dequeue(colqueue);
        if(r == exits[3] && c == exits[2]){
            //previous[rr][cc] = adjacent[r][c];
            for(i = 0; i < size; i++){
                for(j = 0; j < size; j++){
                    printf("%2hd ", previous[i][j]);
                }
                printf("\n");
            }
            for(i = 0; i < size; i++){
                for(j = 0; j < size; j++){
                    printf("%hd ", visited[i][j]);
                }
                printf("\n");
            }
            findPath(size, sol, adjacent, previous, exits);
            return moves;
        }
        for(i = 0; i < 4; i++){
            rr = r + dr[i]; // new position where to move
            cc = c + dc[i];

            printf("%d %d\n", rr, cc);
            //printf("\nvisited %d\nraw %d\n", visited[rr][cc], raw[rr][cc]);
            if(rr < 0 || cc < 0 || rr >= size || c >= size){
                printf("size limit\n");
                continue;
            }
            if((visited[rr][cc] == 1) ||( raw[rr][cc] == 0)){
                continue;
            }
            enqueue(rowqueue, rr);
            enqueue(colqueue, cc);

            visited[rr][cc] = 1;
            previous[rr][cc] = adjacent[r][c];
            nodes_in_next++;
        }
        nodes_in_current--;
        if(nodes_in_current == 0){
            nodes_in_current = nodes_in_next;
            nodes_in_next = 0;
            moves++;
        }
        printf("queue size %d\n", rowqueue->size);
    }
   
       

    return -1;
}

double bfs(int size, short **raw, short **sol, short exits[4]){
    struct timespec start, end;
    short **adj_matrix;
    int adj_size = size * size;
    printf("adj size %d\n", adj_size);
    adj_matrix = (short**)calloc(adj_size + 10, (adj_size + 10) * sizeof(short*));
    for(int i = 0; i < adj_size; i++){
        adj_matrix[i] = (short*)calloc(adj_size + 10, (adj_size + 10) * sizeof(short));
        if(adj_matrix == NULL){
            printf("Fatal error in bfs.c\n");
            exit(0);
        }
    }
    
    
    clock_gettime(CLOCK_REALTIME, &start); // start timer
    // 1 = passage, 0 = wall
    
    //adjacency(size, adj_size, sol, raw, adj_matrix);
    int result = solve(size, adj_size, raw, sol, adj_matrix, exits);
    printf("moves: %d\n", result);
    printf("main end\n");
    clock_gettime(CLOCK_REALTIME, &end); // stop timer
    // time_spent = end - start
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION * 1000;
    return time_spent;
}
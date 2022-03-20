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
    //printf("%d enqueued to queue\n", item);
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

// finding the path of the shortest solution after the search has reached the end
void findPath(int size, short **sol, short adjacent[size][size], short previous[size][size], short exits[4]){ 
    int x = exits[2], y = exits[3];
    int next = previous[exits[3]][exits[2]]; // first position is the exit
    while(x != exits[0] || y != exits[1]){ // until it reaches the entrance
        sol[y][x] = 1; // mark it as a path on the solution maze
        
        // checking all sides
        if(y > 0 && adjacent[y - 1][x] == next)
            y--;
        else if(x + 1 < size && adjacent[y][x + 1] == next)
            x++;
        else if(x > 0 && adjacent[y][x - 1] == next)
            x--;
        else if(y + 1 < size && adjacent[y + 1][x] == next)
            y++;
        next = previous[y][x]; // next position
    }
    sol[y][x] = 1; // marking the beginning as a path as well
}

int solve(int size, short **raw, short **sol, short exits[4]){
    struct Queue* rowqueue = createQueue(100000); // creating queues for x and y coordinates
    struct Queue* colqueue = createQueue(100000); // unnecessary size for now
    int r, c, nodes_in_current = 1, nodes_in_next;
    int rr, cc;
    short visited[size][size]; // marks if a square has been visited
    short previous[size][size]; // for finding the shortest way back from the exit
    short adjacent[size][size];
    short dr[4] = {-1, 1, 0, 0}; // movement vectors S, N, E, W
    short dc[4] = {0, 0, 1, -1};
    int i, j, nr = 0;

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            previous[i][j] = 0;
            visited[i][j] = 0;
            adjacent[i][j] = nr; // marking every square with an unique number to 
            nr++;                // remember where it got to the current position
        }
    }
    enqueue(rowqueue, exits[1]); // queueing the start
    enqueue(colqueue, exits[0]);
    while(rowqueue->size > 0){ // works until there are nodes to visit
        r = dequeue(rowqueue); // take the current position off the queue, no need to visit it again
        c = dequeue(colqueue);
        if(r == exits[3] && c == exits[2]){ // check if reached the exit
            printf("starting pathfinding\n");
            findPath(size, sol, adjacent, previous, exits); // find the shortest way back
            //free(rowqueue);
            //free(rowqueue->array); // free the queues
            //free(colqueue);
            //free(colqueue->array);
            return 0;
        }
        for(i = 0; i < 4; i++){ // try every move
            rr = r + dr[i]; // new position where to move
            cc = c + dc[i];

         
            //printf("\nvisited %d\nraw %d\n", visited[rr][cc], raw[rr][cc]);
            if(rr < 0 || cc < 0 || rr >= size || c >= size){ // new position would be out of bounds
                continue;
            }
            if((visited[rr][cc] == 1) ||( raw[rr][cc] == 0)){ // already been here or new position would be a wall
                continue;
            }
            enqueue(rowqueue, rr); // mark it as new node to visit
            enqueue(colqueue, cc);

            visited[rr][cc] = 1; // mark current position as visited
            previous[rr][cc] = adjacent[r][c]; // mark the unique number of the square that brought us here
            nodes_in_next++; // increase the nr of nodes to visit in next layer
        }
        nodes_in_current--; // remove a visited node
        if(nodes_in_current == 0){ // reached the end of current nodes, moving to next layer
            nodes_in_current = nodes_in_next;
            nodes_in_next = 0;
        }
    }
   
    //free(rowqueue);
    //free(rowqueue->array); // //free the queues
    //free(colqueue);
    //free(colqueue->array);
    return -1;
}

double bfs(int size, short **raw, short **sol, short exits[4]){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start); // start timer
    // 1 = passage, 0 = wall
    int result = solve(size, raw, sol, exits);
    if(result == -1){
        printf("There is no solution!\n");
        return 0;
    }
    clock_gettime(CLOCK_REALTIME, &end); // stop timer
    // time_spent = end - start
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION * 1000;
    return time_spent;
}
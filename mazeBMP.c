#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "header.h"

typedef struct needed_colors{   // holds the colors of all the maze elements
    double rgb[3];
}selected_t;

typedef struct color_fade{      // color fade values
    int size;                   // keeping maze size here
    int direction;              // horizontal or vertical
    double fade[3];             // individual RGB change values per row / column
    double original[3];         // keeping the original color in case of horizontal fades
}fade_t;


// bitmap file header (14 bytes)
struct bitmap_file_header{
    unsigned char   bitmap_type[2];     // 2 bytes
    int             file_size;          // 4 bytes
    short           reserved1;          // 2 bytes
    short           reserved2;          // 2 bytes
    unsigned int    offset_bits;        // 4 bytes
}bfh;

// bitmap image header (40 bytes)
struct bitmap_image_header{
    unsigned int    size_header;        // 4 bytes
    unsigned int    width;              // 4 bytes
    unsigned int    height;             // 4 bytes
    short int       planes;             // 2 bytes
    short int       bit_count;          // 2 bytes
    unsigned int    compression;        // 4 bytes
    unsigned int    image_size;         // 4 bytes
    unsigned int    ppm_x;              // 4 bytes
    unsigned int    ppm_y;              // 4 bytes
    unsigned int    clr_used;           // 4 bytes
    unsigned int    clr_important;      // 4 bytes
}bih;

enum colors{white, silver, gray, black, purple, magenta, blue, lime, green, cyan, aqua, red, yellow, maroon, 
            random_list = 66, random_overall = 77, custom_RGB_code = 88, fade_cl = 99}; // all colors
enum fade_direction{none, horizontal, vertical}; // fade direction

void set_color(double *to, short from[3]){       // setting the actual RGB color to an element
    for(int i = 0; i < 3; i++){
        to[i] = from[i];
    }
}

void fade_calculate(short *to, short *from, fade_t *fade_arr, int element){ // calculating the color change per row / column
    for(int i = 0; i < 3; i++){ // for R, G and B
        fade_arr[element].fade[i] = abs(((double)(to[i]) - (double)(from[i]))) / (double)(fade_arr[0].size); // calculating the change in color per row
        if(to[i] < from[i])
            fade_arr[element].fade[i] *= -1;        // if a value needs to decrease it needs to change it to negative
        fade_arr[element].original[i] = from[i];    // saving the original color
    }
}

// max = maximum random value, count = how many, RGB = give a full array random colors
short randomNr(int max, int count, short RGB[3]){ // if the user wants a random nr
    if(count < 3){
        return rand() % max;
    }
    for(int i = 0; i < count; i++){
        RGB[i] = rand() % max; // don't need the minimum, it will always be 0
    }
    return 0;
}

int userselectRGB(double *input){ // letting the user select a fully custom color with an RGB code
    printf("Use format: 255 255 255\n");
    printf("Enter RGB code (NB! values have a range of 0 - 255): ");
    scanf("%lf %lf %lf", &input[2], &input[1], &input[0]); // holding the color in BGR format so input needs to be saved backwards
    for(int i = 0; i < 3; i++)
        if(input[i] < 0 || input[i] > 255) // checking user input
            return 0;
    return 1;
}

void get_colors(double *input, int mode, int type, fade_t *fade_arr){ // set the element colors
    // all maze elements
    static char options[5][30] = { {"walls"}, {"paths"}, {"recursive solution"}, {"shortest solution"}, {"algo crossover"} };
    
    // color library, note RGB values are backwards
    static short colors[COLORCOUNT][3] = {
        {255, 255, 255},    // white   
        {192, 192, 192},    // silver  
        {128, 128, 128},    // gray    
        {0, 0, 0},          // black   
        {128, 0, 128},      // purple  
        {255, 0, 255},      // magenta 
        {255, 0, 0},        // blue    
        {0, 255, 0},        // lime    
        {0, 128, 0},        // green   
        {255, 255, 0},      // cyan    
        {212, 255, 127},    // aqua    
        {0, 0, 255},        // red     
        {0, 255, 255},      // yellow  
        {0, 0, 128}         // maroon  
    };
    
    if(mode == regular){ // preset colors, no input needed
        switch(type){
            case wall: // giving every element its color
                set_color(input, colors[black]);    // wall, default color: black
                break;
            case path:
                set_color(input, colors[white]);    // path, default color: white
                break;
            case recursive:
                set_color(input, colors[red]);      // recursive solution, default color: red
                break;
            case breath_first:
                set_color(input, colors[blue]);     // BFS solution, default color: blue
                break;
            case crossover:
                set_color(input, colors[purple]);   // solution crossover, default color: purple
                break;
        }
    }
    else if(mode == secret){ // user input needed on color
        int user = -1;
        int fade_inputs[2];
        short gen_fade[2][3]; // placeholder for random colors
        enum fadeInput{start_color, end_color};     // indexes for user fade input gen_fade
        printf("What color should the %s be?\n", options[type]); // asking for every element
        scanf("%d", &user);
        switch(user){
            case white ... maroon:                  // just a color from the list
                fade_arr[type].direction = none;    // mark there is no fade
                set_color(input, colors[user]);     // set user wanted color for element
                break;
            case fade_cl:                           // user selected fade
                randomNr(255, 3, gen_fade[start_color]);   // precalculating a random color, that way two consecutive randoms won't be equal
                printf("Select starting color: ");
                scanf("%d", &fade_inputs[start_color]);
                printf("Select ending color: ");    
                scanf("%d", &fade_inputs[end_color]);   // where to begin and end
                for(int i = 0; i < 2; i++){
                    switch(fade_inputs[i]){
                        case white ... maroon:  // just a color, no further modification needed
                            for(int j = 0; j < 3; j++)
                                gen_fade[i][j] = colors[fade_inputs[i]][j]; // can't use set color due to gen_fade being short
                            break;
                        case random_list:       // giving the element a random color from the list 0 - 14
                            fade_inputs[i] = randomNr(COLORCOUNT, 0, NULL);
                            for(int j = 0; j < 3; j++)
                                gen_fade[i][j] = colors[fade_inputs[i]][j]; // can't use set color due to gen_fade being short 
                            break;
                        case random_overall:    // giving the element a random R, G and B value 0 - 255
                            if(i > start_color)
                                randomNr(255, 3, gen_fade[i]);
                            break;
                        case custom_RGB_code:
                            if(userselectRGB(input) == 0){
                                printf("Invalid input\n");
                                exit(0);
                            }
                            for(int j = 0; j < 3; j++)
                                gen_fade[i][j] = input[j]; // can't use set color due to gen_fade being short
                            printf("%d: %hd %hd %hd\n", i, gen_fade[i][0], gen_fade[i][1], gen_fade[i][2]);
                            break;
                        default:
                            printf("Invalid input\n");
                            exit(0);
                            break;
                    }
                }
                // horizontal is 1, vertical is 2
                /*  printing a BMP starts from the bottom so to get the colors correct the start and end need to be
                    different depending on the fade direction
                */
                int user = none;
                while(user != horizontal && user != vertical){ // asking user for fade direction
                    printf(" 1. Horizontal fade\n");
                    printf(" 2. Vertical fade\n");
                    scanf("%d", &user);
                    if(user < horizontal || user > vertical)
                        printf("Invalid input\n");
                    getchar();
                }
                fade_arr[type].direction = user;            // marking down the fade direction
                if(fade_arr[type].direction == vertical){
                    fade_calculate(gen_fade[start_color], gen_fade[end_color], fade_arr, type); // calculate fade
                    set_color(input, gen_fade[end_color]);
                    
                }   
                else{
                    fade_calculate(gen_fade[end_color], gen_fade[start_color], fade_arr, type); // calculate fade
                    set_color(input, gen_fade[start_color]);
                }
                break;
            case random_list:                               // user just wants a random color from the list
                fade_arr[type].direction = none;            // there is no fade
                set_color(input, colors[randomNr(COLCOUNT, 0, NULL)]); 
                break;                                      // get a random color between 0 and max colors, then set the input to it
            case random_overall:                            // user wants a completely random color for element
                fade_arr[type].direction = none;            // there is no fade
                randomNr(255, 3, gen_fade[start_color]);    // using an array meant for fade colors but it'll do when not in use
                set_color(input, gen_fade[start_color]);    // get 3 random colors for R, G and B, set the element to it
                break;
            case custom_RGB_code:                           // user wants a custom color
                fade_arr[type].direction = none;            // there is no fade
                if(userselectRGB(input) == 0){              // ask for a color and check the function output
                    printf("Invalid input\n");
                    exit(0);
                }
                break;
            default:                                        // user messed something up, give up
                printf("Invalid input\n");
                exit(0);
        }
    }
}

// Function to round an int to a multiple of 4
int round4(int x) {
    return x % 4 == 0 ? x : x - x % 4 + 4;
}

/* Input values:
                height = maze size
                **maze = maze matrix
                mode = no color options or with selectable colors
                present_elements = how many elements need colors,
                                if the maze is not solved then only the wall and path colors are needed
*/

int makeBMP(FILE *f, int mode, int mazetype, maze_t *M){ // making a BMP file from a maze matrix
    srand(time(NULL));                                   // start time in case of random colors
    int height = M->size;                                // mark actual maze size to element height
    int original = height;                               // it will be known as original size
    int width;                          
    int dpi = 1000;                                      // bmp image dpi
    int upscale_factor = BMPTARGETSIZE / height;         // setting the upscale factor, all mazes will be roughly the same size
    height *= upscale_factor;                            // calculate new size and width according to upscaling
    width = height;

    int padded_width = round4(width * 3);                // calculating image and file size, in a bmp file colors have 4 bytes
    int image_size = height * padded_width * 3;          // counting for padding size, actual image size
    int file_size = 54 + 4 * image_size;                 // calculating file size, adding the header
    int ppm = dpi * 39.375;

    memcpy(&bfh.bitmap_type, "BM", 2); // setting up the file header
    bfh.file_size       = file_size;
    bfh.reserved1       = 0;
    bfh.reserved2       = 0;
    bfh.offset_bits     = 0;

    bih.size_header     = sizeof(bih);
    bih.width           = width;
    bih.height          = height;
    bih.planes          = 1;
    bih.bit_count       = 24;
    bih.compression     = 0;
    bih.image_size      = file_size;
    bih.ppm_x           = ppm;
    bih.ppm_y           = ppm;
    bih.clr_used        = 0;
    bih.clr_important   = 0;

    fwrite(&bfh, 1, 14, f);
    fwrite(&bih, 1, sizeof(bih), f); // writing the header to the file

    int idx = 0, idy = 0;
    selected_t elements[5]; // color values for all elements
    fade_t colorfade[5];    // fade change values for all elements
    colorfade[0].size = (float)height; // including the maze size, need it for calculation in fade_calculate()

    if(mode == secret){ // lets print the color cheatsheet
        printf("!!!Color cheatsheet!!!\n");
        printf(" 0: White    |   7: Lime\n");
        printf(" 1: Silver   |   8: Green\n");
        printf(" 2: Gray     |   9: Cyan\n");
        printf(" 3: Black    |  10: Aqua\n");
        printf(" 4: Purple   |  11: Red\n");
        printf(" 5: Magenta  |  12: Yellow\n");
        printf(" 6: Blue     |  13: Maroon\n");
        printf("---------------------------\n");
        printf("66: Random from list\n");
        printf("77: Fully random\n");
        printf("88: Custom RGB value\n");
        printf("99: Fade :o\n");
    }
    
    int present_elements;
    if(mazetype == final_maze)                  // does the sent maze have a solution path
        present_elements = 5;                   // yes, there are 5 different elements
    else
        present_elements = 2;                   // no, there are 2 different elements (wall and path)
    
    if(mode != insanity){
        for(int i = 0; i < present_elements; i++){  // if an unsolved maze was given, no colors for solutions will be needed
            get_colors(elements[i].rgb, mode, i, colorfade); // set colors for all present elements
        }
    }
    else if(mode == insanity){              // every row is a new color
        for(int i = 0; i < 3; i++){
            elements[wall].rgb[i] = 192;    // just give a random value for now
            elements[path].rgb[i] = 255;
        }
    }
    char *bitmap = (char *) malloc(bih.image_size * sizeof(char));  // actual bitmap to be written in the file
    int x_fade_count = 0; // elements with a color fade in both directions
    int y_fade_count = 0;

    for(int i = 0; i < image_size; i++) bitmap[i] = 255; // painting the whole bitmap white, could be unnecessary

    if(mode == secret){ // don't need to check when the user didn't choose colors
        for(int i = 0; i < 5; i++){
            if(colorfade[i].direction == horizontal) // checking if horizontal and/or vertical color fades are present
                x_fade_count++;
            else if(colorfade[i].direction == vertical)
                y_fade_count++;
        }     
    }
    
    int i, j; // loop variables
    int yprevious = 0, xprevious = 0; // keeping track if were printing a new element from the maze matrix, necessary for insanity mode
    short maze_value;
    for (int row = 0; row < height; row++){ // for every row
        if(row / upscale_factor == 0) // see if the maze row needs to be changed, actual file size is larger than the maze
            idy = 0;
        else
            idy = row / upscale_factor;
        if(mode == insanity && (yprevious != idy || xprevious != idx)){ // changing the color for every row if mode = insanity
            for(i = 0; i < 3; i++){
                elements[wall].rgb[i] = rand() % 255;
            }
            //printf("%.0f %.0f %.0f\n", elements[wall].rgb[0], elements[wall].rgb[1], elements[wall].rgb[2]);
        }
        for (int col = 0; col < width; col++){ // for every column
            if(col / upscale_factor == 0) // see if the maze column needs to be changed
                idx = 0;
            else
                idx = col / upscale_factor;
            for (int color = 0; color < 3; color++){ // add the R, G and B values
                int index = row * padded_width + col * 3 + color; // find location in the bitmap
                maze_value = M->algo[mazetype].maze[original - 1 - idy][idx];
                if(maze_value == crossover)
                    bitmap[index] = elements[crossover].rgb[color];      // case: crossover
                else if(maze_value == breath_first)
                    bitmap[index] = elements[breath_first].rgb[color];   // case: bfs algo
                else if(maze_value == wall)
                    bitmap[index] = elements[wall].rgb[color];           // case: wall
                else if(maze_value == path)
                    bitmap[index] = elements[path].rgb[color];           // case: path
                else if(maze_value == recursive)
                    bitmap[index] = elements[recursive].rgb[color];      // case: recursive algo
            }
            if(x_fade_count > 0){ // if a horizontal color fade is present
                for(i = 0; i < 5; i++)
                    if(colorfade[i].direction == horizontal)
                        for(j = 0; j < 3; j++)
                            elements[i].rgb[j] += colorfade[i].fade[j]; // increment the RGB values
            }
            xprevious = idx;
        }
        if(x_fade_count > 0){ // if a horizontal color fade is present
            for(i = 0; i < 5; i++)
                if(colorfade[i].direction == horizontal)
                    for(j = 0; j < 3; j++)
                        elements[i].rgb[j] = colorfade[i].original[i]; // reset the RGB values
        }
        if(y_fade_count > 0){ // if a vertical color fade is present
            for(i = 0; i < 5; i++)
                if(colorfade[i].direction == vertical)
                    for(j = 0; j < 3; j++)
                        elements[i].rgb[j] += colorfade[i].fade[j]; // increment the RGB values
        }
        yprevious = idy;
    }
    fwrite(bitmap, bih.image_size * sizeof(char), 1, f); // write the bitmap to the file
    fclose(f); // close the file
    printf("upscaling x%d to %d x %d px\n", upscale_factor, height, height); // lets look at the upscale factor
    free(bitmap); // free memory
    return 0;
}


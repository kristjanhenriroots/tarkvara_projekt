#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 700
#define MIN 30
#define COLORCOUNT 14 // currently supported colors
#define TARGETSIZE 3000 // target width in pixels

typedef struct needed_colors{ // holds the colors of all the maze elements
    double rgb[3];
}selected_t;

typedef struct color_fade{ // color fade values
    int size; // keeping maze size here
    int direction; // horizontal or vertical
    double fade[3]; // individual RGB change values per row / column
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

enum color_type{wall, path, recursive, breath_first, crossover}; // element type
// note, always move fade last!!
enum colors{white, silver, gray, black, purple, magenta, blue, lime, green, cyan, teal, red, yellow, maroon, fade_cl = 99}; // all colors
enum mode_select{regular, secret}; // mode the function was called in
enum fade_direction{none, horizontal, vertical}; // fade direction

void set_color(double *to, short from[3]){ // setting the actual RGB color to an element
    for(int i = 0; i < 3; i++){
        to[i] = from[i];
    }
}

void fade_calculate(short *to, short *from, fade_t *fade_arr, int element){ // calculating the color change per row / column
    for(int i = 0; i < 3; i++){ // for R, G and B
        fade_arr[element].fade[i] = abs(((double)(to[i]) - (double)(from[i]))) / (double)(fade_arr[0].size); // calculating the change in color per row
        if(to[i] < from[i])
            fade_arr[element].fade[i] *= -1; // if a value needs to decrease it needs to change it to negative
    }
    int user = none;
    while(user != horizontal && user != vertical){ // asking user for fade direction
        printf(" 1. Horizontal fade\n");
        printf(" 2. Vertical fade\n");
        scanf("%d", &user);
        if(user < 1 || user > 2)
            printf("Invalid input\n");
    }
    fade_arr[element].direction = user; // marking down the fade direction
}

void get_colors(double *input, int mode, int type, fade_t *fade_arr){ // set the element colors
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
        {128, 128, 0},      // teal    
        {0, 0, 255},        // red     
        {0, 255, 255},      // yellow  
        {0, 0, 128}         // maroon  
    };

    if(mode == regular){ // preset colors, no input needed
        switch(type){
            case wall: // giving every element its color
                set_color(input, colors[black]);
                break;
            case path:
                set_color(input, colors[white]);
                break;
            case recursive:
                set_color(input, colors[red]);
                break;
            case breath_first:
                set_color(input, colors[blue]);
                break;
            case crossover:
                set_color(input, colors[purple]);
                break;
        }
    }
    else if(mode == secret){ // user input needed on color
        int user = -1;
        printf("What color should the %s be?\n", options[type]); // asking for every element
        scanf("%d", &user);
        while((user != fade_cl && user > maroon) || user < white){
            printf("Invalid input, try again\n");
            printf("What color should the %s be?\n", options[type]);
            scanf("%d", &user);
        }
        if(user == fade_cl){ // user wants a color fade
            int starting = -1, ending = -1;
            while(starting < white || ending < white || starting > maroon || ending > maroon){
                printf("Select starting color: ");
                scanf("%d", &starting);
                printf("Select ending color: ");
                scanf("%d", &ending);
            }
            fade_calculate(colors[starting], colors[ending], fade_arr, type); // calculate fade
            set_color(input, colors[starting]); // set the starting value to element
        }
        else{
            fade_arr[type].direction = none; // mark there is no fade
            set_color(input, colors[user]); // set user wanted color for element
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

int makeBMP(int height, short **maze, int mode, int present_elements){ // making a BMP file from a maze matrix
    int original = height;
    int width;
    int dpi = 1000;
    int upscale_factor = TARGETSIZE / height; // setting the upscale factor, all mazes will be roughly the same size
    height *= upscale_factor;
    width = height;

    int padded_width = round4(width * 3); // calculating image and file size
    int image_size = height * padded_width * 3;
    int file_size = 54 + 4 * image_size;
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

    FILE *f = fopen("bitmap.bmp", "wb");

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
        printf(" 3: Black    |  10: Teal\n");
        printf(" 4: Purple   |  11: Red\n");
        printf(" 5: Magenta  |  12: Yellow\n");
        printf(" 6: Blue     |  13: Maroon\n");
        printf("99: Fade :o\n");
    }   
    for(int i = 0; i < present_elements; i++){ // if an unsolved maze was given, no colors for solutions will be needed
        get_colors(elements[i].rgb, mode, i, colorfade); // set colors for all present elements
    }

    char *bitmap = (char *) malloc(bih.image_size * sizeof(char)); // actual bitmap to be written in the file
    int x_fade_count = 0; // elements with a color fade in both directions
    int y_fade_count = 0;

    for(int i = 0; i < image_size; i++) bitmap[i] = 255; // painting the whole bitmap white

    if(mode == secret){ // don't need to check when the user didn't choose colors
        for(int i = 0; i < 5; i++){
            if(colorfade[i].direction == horizontal) // checking if horizontal and/or vertical color fades are present
                x_fade_count++;
            else if(colorfade[i].direction == vertical)
                y_fade_count++;
        }
           
    }
    
    int i, j; // loop variables
    for (int row = 0; row < height; row++){ // for every row
        if(row / upscale_factor == 0) // see if the maze row needs to be changed, actual file size is larger than the maze
            idy = 0;
        else
            idy = row / upscale_factor;
        for (int col = 0; col < width; col++){ // for every column
            if(col / upscale_factor == 0) // see if the maze column needs to be changed
                idx = 0;
            else
                idx = col / upscale_factor;
            for (int color = 0; color < 3; color++){ // add the R, G and B values
                int index = row * padded_width + col * 3 + color; // find location in the bitmap
                if(maze[original - 1 - idy][idx] == crossover)
                    bitmap[index] = round(elements[crossover].rgb[color]);      // case: crossover
                else if(maze[original - 1 - idy][idx] == breath_first)
                    bitmap[index] = round(elements[breath_first].rgb[color]);   // case: bfs algo
                else if(maze[original - 1 - idy][idx] == wall)
                    bitmap[index] = round(elements[wall].rgb[color]);           // case: wall
                else if(maze[original - 1 - idy][idx] == path)
                    bitmap[index] = round(elements[path].rgb[color]);           // case: path
                else if(maze[original - 1 - idy][idx] == recursive)
                    bitmap[index] = round(elements[recursive].rgb[color]);      // case: recursive algo
            }
            if(x_fade_count > 0){ // if a horizontal color fade is present
                for(i = 0; i < 5; i++)
                    if(colorfade[i].direction == horizontal)
                        for(j = 0; j < 3; j++)
                            elements[i].rgb[j] += colorfade[i].fade[j]; // increment the RGB values
            }
        }
        if(x_fade_count > 0){ // if a horizontal color fade is present
            for(i = 0; i < 5; i++)
                if(colorfade[i].direction == horizontal)
                    for(j = 0; j < 3; j++)
                        elements[i].rgb[j] -= colorfade[i].fade[j] * (width - 1); // reset the RGB values
        }
        if(y_fade_count > 0){ // if a vertical color fade is present
            for(i = 0; i < 5; i++)
                if(colorfade[i].direction == vertical)
                    for(j = 0; j < 3; j++)
                        elements[i].rgb[j] += colorfade[i].fade[j]; // increment the RGB values
        }
    }
    fwrite(bitmap, bih.image_size * sizeof(char), 1, f); // write the bitmap to the file
    fclose(f); // close the file
    printf("upscaling x%d to %d\n", upscale_factor, height); // lets look at the upscale factor
    free(bitmap); // free memory
    return 0;
}


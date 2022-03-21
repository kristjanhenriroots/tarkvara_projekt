#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 700
#define MIN 30

struct rgb_data {
    float r, g, b;
};

typedef struct needed_colors{
    char rgb[3];
}colors_t;

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

enum color_type{wall, path, recursive, breath_first, crossover};
enum colors{whiteID, silverID, grayID, blackID, purpleID, magentaID, blueID, limeID, greenID, cyanID, tealID, redID, yellowID, maroonID};
enum mode_select{regular, secret};

void set_color(char *to, char from[3], int current){
    for(int i = 0; i < 3; i++){
        to[i] = from[i];
    }
}

void get_colors(char *input, int mode, int type){
    char options[5][30] = { {"walls"}, {"paths"}, {"recursive solution"}, {"shortest solution"}, {"algo crossover"} };
    // color library, note RGB values are backwards
    char white[]    = {255, 255, 255};
    char silver[]   = {192, 192, 192};
    char gray[]     = {128, 128, 128};
    char black[]    = {0, 0, 0};
    char purple[]   = {128, 0, 128};
    char magenta[]  = {255, 0, 255};
    char blue[]     = {255, 0, 0};
    char lime[]     = {0, 255, 0};
    char green[]    = {0, 128, 0};
    char cyan[]     = {255, 255, 0};
    char teal[]     = {128, 128, 0};
    char red[]      = {0, 0, 255};
    char yellow[]   = {0, 255, 255};
    char maroon[]   = {0, 0, 128};


    if(mode == regular){
        switch(type){
            case wall:
                set_color(input, black, wall);
                break;
            case path:
                set_color(input, white, path);
                break;
            case recursive:
                set_color(input, red, recursive);
                break;
            case breath_first:
                set_color(input, blue, breath_first);
                break;
            case crossover:
                set_color(input, purple, crossover);
                break;
        }
    }
    else if(mode == secret){
        int user;
        printf("What color should the %s be?\n", options[type]);
        scanf("%d", &user);
        while(user < whiteID || user > maroonID){
            printf("Invalid input, try again\n");
            printf("What color should the %s be?\n", options[type]);
            scanf("%d", &user);
        }
        switch(user){
            case whiteID:
                set_color(input, white, type);
                break;
            case silverID:
                set_color(input, silver, type);
                break;
            case grayID:
                set_color(input, gray, type);
                break;
            case blackID:
                set_color(input, black, type);
                break;
            case purpleID:
                set_color(input, purple, type);
                break;
            case magentaID:
                set_color(input, magenta, type);
                break;
            case blueID:
                set_color(input, blue, type);
                break;
            case limeID:
                set_color(input, lime, type);
                break;
            case greenID:
                set_color(input, green, type);
                break;
            case cyanID:
                set_color(input, cyan, type);
                break;
            case tealID:
                set_color(input, teal, type);
                break;
            case redID:
                set_color(input, red, type);
                break;
            case yellowID:
                set_color(input, yellow, type);
                break;
            case maroonID:
                set_color(input, maroon, type);
                break;
        }
    }
}

// Function to round an int to a multiple of 4
int round4(int x) {
    return x % 4 == 0 ? x : x - x % 4 + 4;
}

int makeBMP(int height, short **maze, int mode){
    int original = height;
    int width;
    int dpi = 1000;
    int upscale_factor = 3000 / height;
    height *= upscale_factor;
    width = height;

    int padded_width = round4(width * 3);
    int image_size = height * padded_width * 3;
    int file_size = 54 + 4 * image_size;
    int ppm = dpi * 39.375;

    memcpy(&bfh.bitmap_type, "BM", 2); // headeri algväärtustamine
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
    fwrite(&bih, 1, sizeof(bih), f); // kirjutan headeri

    int idx = 0, idy = 0;
    colors_t elements[5];
    
    if(mode == secret){ // lets print the color cheatsheet
        printf("!!!Color cheatsheet!!!\n");
        printf("0: White    |   7: Lime\n");
        printf("1: Silver   |   8: Green\n");
        printf("2: Gray     |   9: Cyan\n");
        printf("3: Black    |  10: Teal\n");
        printf("4: Purple   |  11: Red\n");
        printf("5: Magenta  |  12: Yellow\n");
        printf("6: Blue     |  13: Maroon\n");
    }   

    for(int i = 0; i < 5; i++){
        get_colors(elements[i].rgb, mode, i);
    }

    char *bitmap = (char *) malloc(bih.image_size * sizeof(char));
    for (int i = 0; i < image_size; i++) bitmap[i] = 255;
    // For each pixel in the RGB image...
    for (int row = 0; row < height; row++) {
        if(row / upscale_factor == 0)
            idy = 0;
        else
            idy = row / upscale_factor;
        for (int col = 0; col < width; col++) {
            // For R, G, and B...
            if(col / upscale_factor == 0)
                idx = 0;
            else
                idx = col / upscale_factor;
            //printf("%d %d : %d %d\n", row, col, idy, idx);
            for (int color = 0; color < 3; color++) {
                int index = row * padded_width + col * 3 + color;
                if(maze[original - 1 - idy][idx] == crossover)
                    bitmap[index] = elements[crossover].rgb[color];
                else if(maze[original - 1 - idy][idx] == breath_first)
                    bitmap[index] = elements[breath_first].rgb[color];
                else if(maze[original - 1 - idy][idx] == wall)
                    bitmap[index] = elements[wall].rgb[color];
                else if(maze[original - 1 - idy][idx] == path)
                    bitmap[index] = elements[path].rgb[color];
                else if(maze[original - 1 - idy][idx] == recursive)
                    bitmap[index] = elements[recursive].rgb[color];
            }
            
        }
        
    }
    fwrite(bitmap, bih.image_size * sizeof(char), 1, f);
    fclose(f);
    printf("upscaling x%d to %d\n", upscale_factor, height);
    free(bitmap);
    return 0;

}

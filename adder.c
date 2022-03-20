#include <stdio.h>
#include <stdlib.h>

void makeSVG(char *filename, int size, short **maze, short **shortest){
    FILE *svg;
    svg = fopen(filename, "w");
    if(svg == NULL){
        printf("Failed to open %s at adder.c\n", filename);
        exit(0);
    }
    int value;
    fprintf(svg, "<?xml version=\"1.0\"?>\n"); // SVG header stuff
	fprintf(svg, "<svg width=\"%dcm\" height=\"%dcm\" viewBox=\"0 0 %d %d\"\n", size, size, size*5, size *5);
	fprintf(svg, "\txmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");
	fprintf(svg, "<g>");
	for(int a = 0; a < size; a++) {
		for(int b = 0; b < size; b++){
            value = maze[a][b]; // add every element of every matrix at the same xy
			if(value == 3){ // passage visited twice aka the recursive backtracer on the dead end filler
                fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:purple\" />\n", b, a);
            }
			else if(shortest[a][b] == 1){
                fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:blue\" />\n", b, a);
			}
			else if(value == 1) { // must be an unvisited passage
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:white\" />\n", b, a);		
			}else if(value == 0){ // must be a wall
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:black\" />\n", b, a);
			}else if(value == 2){ // passage visited once
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:red\" />\n", b, a);
			}
            
		}
	}
	fprintf(svg, "</g>"); // SVG footer
	fprintf(svg, "</svg>\n");
	fclose(svg);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"
#include "build.h"

int main(int argc, char ** argv){
    if (argc == 4 && strcmp(argv[1], "-b") == 0){
        FILE * f_in = fopen(argv[2], "r");
        FILE * f_out = fopen(argv[3], "w");
        if (!f_in || !f_out){
            if (f_in) { fclose(f_in); }
            if (f_out) { fclose(f_out); }
            return EXIT_FAILURE;
        }

        buildTree(f_in, f_out);
        fclose(f_in);
        fclose(f_out);
    }
    else if (argc == 3 && strcmp(argv[1], "-e") == 0)
    {
        FILE * f_in = fopen(argv[2], "rb");
        if (!f_in){
            fclose(f_in);
            fprintf(stdout, "0,0,0\n");
            return EXIT_FAILURE;
        }

        evalTree(f_in);
        fclose(f_in);
    }
    else {
        return EXIT_FAILURE;
    }
}

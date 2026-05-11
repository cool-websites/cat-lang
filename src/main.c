#include "../include/cat.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: cat <file.cat>\n");
        return 1;
    }

    return cat_run_file(argv[1]);
}
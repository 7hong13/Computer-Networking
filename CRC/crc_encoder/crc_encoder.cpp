#include <iostream>
#include <cstdlib>
using namespace std;

void exitArgsErr() {
    fprintf(stderr, "usage: ./crc_encoder input_file output_file generator dataword_size\n");
    exit(1);
}

void exitInputErr() {
    fprintf(stderr, "input file open error.\n");
    exit(1);
}

void exitOutputErr() {
    fprintf(stderr, "output file open error.\n");
    exit(1);
}

void exitDatawordErr() {
    fprintf(stderr, "dataword size must be 4 or 8.\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        exitArgsErr();
    }

    FILE *input_fp, *output_fp;
    int dataword_size = atoi(argv[4]);
    input_fp = fopen(argv[1], "r"); //input file open
    if (input_fp == NULL) {
        exitInputErr();
    }

    output_fp = fopen(argv[2], "r"); //output file open
    if (output_fp == NULL) {
        exitOutputErr();
    }

    if (dataword_size!= 4 && dataword_size != 8) {
        exitDatawordErr();
    }

    return 0;
}
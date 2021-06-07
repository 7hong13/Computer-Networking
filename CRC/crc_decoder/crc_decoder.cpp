#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <string>
#include <algorithm>
#define ARGS_CNT_ERR 0
#define INPUT_ERR 1
#define OUTPUT_ERR 2
#define DATAWORD_SIZE_ERR 3
#define RESULT_ERR 4
using namespace std;

void exitErr(int err) {
    if (err == ARGS_CNT_ERR) {
        fprintf(stderr, "usage: ./crc_encoder input_file output_file generator dataword_size\n");
    }
    else if (err == INPUT_ERR) {
        fprintf(stderr, "input file open error.\n");
    }
    else if (err == OUTPUT_ERR) {
        fprintf(stderr, "output file open error.\n");
    }
    else if (err == DATAWORD_SIZE_ERR) {
        fprintf(stderr, "dataword size must be 4 or 8.\n");
    }
    else if (err == RESULT_ERR) {
        fprintf(stderr, "result file open error.\n");
    }
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        exitErr(ARGS_CNT_ERR);
    }

    FILE *input_fp, *output_fp, *result_fp;
    int dataword_size = atoi(argv[5]);
    input_fp = fopen(argv[1], "r"); //input file open
    if (input_fp == NULL) {
        exitErr(INPUT_ERR);
    }

    output_fp = fopen(argv[2], "r"); //output file open
    if (output_fp == NULL) {
        exitErr(OUTPUT_ERR);
    }

    result_fp = fopen(argv[3], "r"); //result file open
    if (result_fp == NULL) {
        exitErr(RESULT_ERR);
    }

    if (dataword_size!= 4 && dataword_size != 8) {
        exitErr(DATAWORD_SIZE_ERR);
    }

    
    return 0;
}
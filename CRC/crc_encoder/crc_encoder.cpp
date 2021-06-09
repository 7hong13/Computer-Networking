#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <string>
#define ARGS_CNT_ERR 0
#define INPUT_ERR 1
#define OUTPUT_ERR 2
#define DATAWORD_SIZE_ERR 3
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
    exit(1);
}

string getMod(string dataword, string generator) {
    int gen_len = generator.length();
    int lastIdx = gen_len;
    string rem = "";
    for (int iter = 0; iter < gen_len - 1; iter++) {
        dataword += '0';
    }
    string dividend = dataword.substr(0, gen_len);

    int len = dataword.length();
    while (lastIdx < len) {
        if (dividend[0] == '0') {
            dividend = dividend.substr(1, gen_len) + dataword[lastIdx++];
        }
        else {
            string res = "";
            for (unsigned idx = 1; idx < dividend.length(); idx++) {
                res += dividend[idx] == generator[idx] ? '0' : '1';
            }
            dividend = res;
            dividend += dataword[lastIdx++];
        }
    }

    //rem 구하기
    if (dividend[0] == '0') {
            rem = dividend.substr(1, gen_len);
    }
    else {
        for (unsigned idx = 1; idx < dividend.length(); idx++) {
            rem += dividend[idx] == generator[idx] ? '0' : '1';
        }
    }
    return rem;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        exitErr(ARGS_CNT_ERR);
    }

    FILE *input_fp, *output_fp;
    int dataword_size = atoi(argv[4]);
    input_fp = fopen(argv[1], "r"); //input file open
    if (input_fp == NULL) {
        exitErr(INPUT_ERR);
    }

    output_fp = fopen(argv[2], "r"); //output file open
    if (output_fp == NULL) {
        exitErr(OUTPUT_ERR);
    }

    if (dataword_size!= 4 && dataword_size != 8) {
        exitErr(DATAWORD_SIZE_ERR);
    }

    char input; 
    string dataword, codeword, dec_to_binary;
    fscanf(input_fp, "%c", &input);
    output_fp = fopen(argv[2], "w");
    
    while (!feof(input_fp)) {
        dec_to_binary = bitset<8>(int(input)).to_string();
        if (dataword_size == 8) {
            dataword = dec_to_binary;
            codeword += dataword + getMod(dataword, argv[3]);
        }
        else if (dataword_size == 4) {
            for (int start = 0; start < 5; start += 4) {
                dataword = dec_to_binary.substr(start, 4);
                codeword += dataword + getMod(dataword, argv[3]);
            }
        }
        fscanf(input_fp, "%c", &input);
    }

    int padding = 0;
    if (codeword.length() % 8 != 0) {
        padding = 8 - codeword.length() % 8;
        for (int iter = 0; iter < padding; iter++) {
            codeword = '0' + codeword;
        }
    }
    string padding_bit = bitset<8>(padding).to_string();
    string total_input = padding_bit + codeword;
    for (unsigned idx = 0; idx < total_input.length(); idx += 8) {
        fprintf(output_fp, "%c", (int) strtoull(total_input.substr(idx, 8).c_str(), NULL, 2));
    }

    fclose(input_fp);
    fclose(output_fp);
    return 0;
}
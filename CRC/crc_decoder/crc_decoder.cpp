#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <string>
#include <cstring>
#define ARGS_CNT_ERR 0
#define INPUT_ERR 1
#define OUTPUT_ERR 2
#define DATAWORD_SIZE_ERR 3
#define RESULT_ERR 4
using namespace std;

void exitErr(int err) {
    if (err == ARGS_CNT_ERR) {
        fprintf(stderr, "usage: ./crc_decoder input_file output_file result_file generator dataword_size\n");
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

bool hasCodewordErr(string codeword, string generator) {
    int gen_len = generator.length();
    int lastIdx = gen_len;
    string dividend = codeword.substr(0, gen_len);
    string rem = "";

    int len = codeword.length();
    while (lastIdx < len) {
        if (dividend[0] == '0') {
            dividend = dividend.substr(1, gen_len) + codeword[lastIdx++];
        }
        else {
            string res = "";
            for (unsigned idx = 1; idx < dividend.length(); idx++) {
                res += dividend[idx] == generator[idx] ? '0' : '1';
            }
            dividend = res;
            dividend += codeword[lastIdx++];
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
    
    for (unsigned idx = 0; idx < rem.length(); idx++) {
        if (rem[idx] != '0') return true;
    }
    return false;
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

    output_fp = fopen(argv[2], "w"); //output file open
    if (output_fp == NULL) {
        exitErr(OUTPUT_ERR);
    }

    result_fp = fopen(argv[3], "w"); //result file open
    if (result_fp == NULL) {
        exitErr(RESULT_ERR);
    }

    if (dataword_size != 4 && dataword_size != 8) {
        exitErr(DATAWORD_SIZE_ERR);
    }

    char char_padding; 
    int padding;
    fscanf(input_fp, "%c", &char_padding);
    padding = int(char_padding);

    char input;
    string bytes, codeword, dataword = "";
    int codeword_cnt = 0;
    int codeword_err = 0;
    int gen_len = strlen(argv[4]);
    int codeword_len = dataword_size + gen_len - 1;
    int dataword_to_dec;

    fscanf(input_fp, "%c", &input);
    while (!feof(input_fp)) {
        bytes += bitset<8>(int(input)).to_string();
        fscanf(input_fp, "%c", &input);
    }

    for (unsigned idx = padding; idx < bytes.length(); idx += codeword_len) {
        codeword = bytes.substr(idx, codeword_len);
        if (hasCodewordErr(codeword, argv[4])) codeword_err++;
        codeword_cnt++;
        dataword += codeword.substr(0, dataword_size);
        if (dataword.length() != 8) continue; 
        dataword_to_dec = (int) strtoull(dataword.c_str(), NULL, 2);
        fprintf(output_fp, "%c", dataword_to_dec);
        dataword = "";
    }

    fprintf(result_fp, "%d %d", codeword_cnt, codeword_err);

    fclose(input_fp);
    fclose(output_fp);
    fclose(result_fp);
    return 0;
}
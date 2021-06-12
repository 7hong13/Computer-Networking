# Cyclic Redundancy Check (CRC)
Cyclic Redundancy Check Implementation in C++


## 1. crc encoder
- encode datawords to codewords and write them to output file
- arguments: ./crc_encoder input_file output_file generator dataword_size 
  <br>(generator: MSB always 1, maximum length 9)


## 2. crc decoder
- decode each codeword and restore its original dataword
- check errors and record the result on result file
- arguments: ./crc_decoder input_file output_file result_file generator dataword_size

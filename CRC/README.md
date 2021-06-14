# Cyclic Redundancy Check (CRC)
Cyclic Redundancy Check Implementation in C++
<br>
<br>

## CRC encoder
- convert datawords to codewords and write them to output file
- execution: ./crc_encoder input_file output_file generator dataword_size 
  <br> (e.g. ./crc_encoder datastream.tx codedstream.tx 1101 4)
<br>


## CRC decoder
- decode each codeword and restore its original dataword
- check errors and record the result on result file
- execution: ./crc_decoder input_file output_file result_file generator dataword_size
  <br> (e.g. ./crc_decoder codedstream.rx datastream.rx result.txt 1101 4)
<br>

## Input
- intput file & output file: can be any format(e.g. jpeg, pdf, txt)
- generator: MSB always 1, maximum length 9
- dataword_size: always 4 or 8

# Near Lossless Time Series Data Compression Methods using Statistics and Deviation
The simulation of the entropy encoding algorithms can be found in their respective folders.
Terminal codes to run static Huffman encoding simulation are:
```
gcc testsender.c -o testsender -lpsapi
.\testsender 
gcc testreciver.c -o testreciver -lpsapi
.\testreciver 
```
Terminal codes to run adaptive(dynamic) Huffman encoding simulation are:
```
gcc fgk.c -o fgk -lpsapi
./fgk -c file-to-be-encoded.txt output.txt
./fgk -d file-to-be-decoded-5.txt output.txt
```
Terminal codes to run adaptive(dynamic) arithmetic encoding simulation are:
```
gcc AAC.c -o AAC -lpsapi
./AAC -c file-to-be-encoded.txt output.txt
./AAC -d file-to-be-decoded-5.txt output.txt
```
All the transformation algorithms can be found in transformation folder.
Terminal codes to run these algorithms are:
```
gcc file-to-be-run.c -o file-to-be-run -lpsapi
.\file-to-be-run
```
file names are given as encode/decode_float_blocksize.c
the default block size is 16.
All the datasets are gine in the dataset folder. 
The datasets produced post applying statistical and entropy encoding are named as follows.
* for statistical version 1:
`encoded-stat-number_of_decimal_digits-dataset-blocksize-tau_value.txt`
* for statistical version2:
`encoded-stat-II-number_of_decimal_digits-dataset-blocksize-tau_value.txt`
The datasets produced post applying statistical encoding are named as follows.
* for statistical version 1:
`stat-number_of_decimal_digits-dataset-blocksize-tau_value.txt`
* for statistical version2:
`stat-II-number_of_decimal_digits-dataset-blocksize-tau_value.txt`
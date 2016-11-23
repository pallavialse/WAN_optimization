decompr: decomprmain.c decompress.c lz-dict-decode.c hash.c decode.c
	gcc -g -O0 -o $@ $^ 

compr: main.c parse.c hash.c lz-dict.c compress.c encode.c
	gcc -g -O0 -o compr parse.c hash.c lz-dict.c main.c compress.c encode.c

all: compr decompr

# The following target tests the function add_phraseAt() used in de-compressor
add: test.c add.c hash.c 
	gcc -g -O0 -o add add.c test.c hash.c 
	
dump:dump_hash_table.c hash.c
	gcc -g -O0 -o dump dump_hash_table.c hash.c

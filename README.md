# XOR-enigma

## Description
XOR-enigma is a simple encryption program that uses the XOR operation to encrypt text or binary files. It employs a linear congruential generator (LCG) algorithm to generate pseudo-random numbers for encryption. The program takes a text input and a key, performs the encryption, and saves the encrypted result to a file to avoid the problem of special characters.

## Usage

### Compiling
To compile the program, use the following command:

- `gcc -std=c99 -Wall -pedantic *.c -lm -g`

### Running
To run the program, use the following command:

- `./a.out (optional -f: <filename.bin>, uses the file's content as input.)`

If the `-f` option is provided, the program will read the content of the specified binary file as input. Otherwise, it will prompt you to enter the text manually. After entering the text, you will be asked to provide a key for encryption. The program will then encrypt the text using the XOR operation and save the encrypted result to a file named "data.bin" by default.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEED 12 // or Start Value
#define M 4967296 // Modulus
#define A 3515245 // Multiplier
#define C 12345 // Increment

/* (C) N. de Boer & A. Janikowska, 2023 */

int abs(int x) {
  return x < 0 ? -x : x;
}

unsigned int lcg(int prevRandInt) {
  return (A * prevRandInt + C) % M;
}

unsigned int sg(unsigned char X_n24, unsigned char X_n55, unsigned int m) {
  return abs(X_n24 - X_n55) % M;
}

int main(int argc, char *argv[]) {
  char text[100];
  int key = 0;

  if (argc == 3 && strcmp(argv[1], "-f") == 0) {
    /* Read text from file */
    FILE *file = fopen(argv[2], "rb");
    if (file == NULL) {
      printf("Failed to open the file.\n");
      return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize >= sizeof(text)) {
      printf("File size exceeds the maximum limit.\n");
      fclose(file);
      return 1;
    }

    fread(text, sizeof(char), fileSize, file);
    fclose(file);
  } else {
    /* Read text from user input */
    printf("Please enter the text: ");
    fgets(text, sizeof(text), stdin);
  }

  printf("Please enter the key: ");
  scanf("%d", &key);

  unsigned int seed = key;
  int textLength = strlen(text);
  unsigned char result[strlen(text)];
  for (int i = 0; i < strlen(text); i++) {
    if (text[i] == '\n') { // Avoid encrypting the end of input
      result[i] = '\n';
      continue;
    }
    unsigned char randByte = lcg(seed);
    result[i] = text[i] ^ randByte;
    printf("'%c' (%d) became '%c' (%d) (randByte = %d)\n", text[i], text[i], result[i], result[i], randByte);
    seed = lcg(seed);
  }
  result[textLength] = '\0'; // Add null terminator
  printf("Output: %s", result);

  /* Check wether we got a given file name. */
  char *filename = argc == 3 ? argv[2] : "data.bin";

  FILE *file = fopen(filename, "wb");
  if (file != NULL) {
    fwrite(result, sizeof(unsigned char), textLength, file);
    fclose(file);
    printf("Result saved to %s\n", filename);
  } else {
    printf("Failed to open the file.\n");
  }

  return 0;
}
#include <stdio.h>
#include <string.h>

#define SEED 12 // or Start Value
#define M 94 // Modulus
#define A 1103515245 // Multiplier
#define C 12345 // Increment

/* (C) N. de Boer & A. Janikowska, 2023 */
//ola

int abs(int x) {
  return x < 0 ? -x : x;
}

unsigned int lcg(unsigned int m, int a, int c, int prevRandInt) {
  return (a * prevRandInt + c) % m;
}

unsigned int sg(unsigned char X_n24, unsigned char X_n55, unsigned int m) {
  return abs(X_n24 - X_n55) % m;
}

int main(int argc, char *argv[]) {
  int key = 0;
  printf("Please enter the key: ");
  scanf("%d", &key);
  char text[100];
  printf("Please enter the text: ");
  scanf("%s", text);
  /* printf("Do you want to 'e'ncrypt or 'd'ecrypt ? ");
  int isEncrypt;
  scanf("%c", &isEncrypt-100); // 0 if e, 1 if d. */

  /* Using the LCG algo */
  unsigned int seed = key;
  char result[strlen(text)];
  for(int i = 0; i < strlen(text); i++) {
    unsigned char randByte = lcg(M, A, C, seed);
    result[i] = text[i] ^ (randByte+32);
    seed = lcg(M, A, C, seed);
  }
  printf("%s\n", result);

  /* Reverting the encryption now */
  seed = key; // Resetting
  for(int i = 0; i < strlen(text); i++) {
    unsigned char randByte = lcg(M, A, C, seed); // Get a random byte
    result[i] = result[i] ^ (randByte+32); // do a XOR on the current char
    seed = lcg(M, A, C, seed); // get the next random number
  }
  printf("%s\n", result);
  return 0;
}
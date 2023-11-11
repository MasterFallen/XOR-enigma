#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEED 12       // or Start Value
#define M 2147483648  // Modulus
#define A 1103515245  // Multiplier
#define C 12345       // Increment

/* (C) N. de Boer & A. Janikowska, 2023 */

int abs(int x) {
   return x < 0 ? -x : x;
}
// unsigned int

unsigned int lcg(int prevRandInt) {
   return (A * prevRandInt + C) % M;
}

unsigned int sg(unsigned char X_n24, unsigned char X_n55, unsigned int m) {
   return abs(X_n24 - X_n55) % M;
}

void readFile(char *filename, char *text, int maxSize) {
   FILE *file = fopen(filename, "rb");
   if (file == NULL) {
      printf("Failed to open the file.\n");
      return;
   }

   fseek(file, 0, SEEK_END);
   long fileSize = ftell(file);
   fseek(file, 0, SEEK_SET);

   if (fileSize >= maxSize) {
      printf("File size exceeds the maximum limit.\n");
      fclose(file);
      return;
   }

   fread(text, sizeof(char), fileSize, file);
   fclose(file);
}

int main(int argc, char *argv[]) {
   char text[100];
   int key = 0;

   if (argc == 3 && strcmp(argv[1], "-f") == 0) {
      /* Read text from file */
      readFile(argv[2], text, sizeof(text));
   } else {
      /* Read text from user input */
      printf("Please enter the text: ");
      fgets(text, sizeof(text), stdin);
   }

   printf("Please enter the key: ");
   scanf("%d", &key);

   int encryptionMethod = 4;
   printf("Please enter the desired encryption method: \n\n");
   printf("[1] OS Random number generator, \n");
   printf("[2] Linear congruential generator, \n");
   printf("[3] Subtractive generator, \n");
   scanf("%d", &encryptionMethod);

   unsigned int seed = key;
   int textLength = strlen(text);
   unsigned char result[strlen(text)];

   for (int i = 0; i < strlen(text); i++) {
      if (text[i] == '\n') {  // Avoid encrypting the end of input
         result[i] = '\n';
         continue;
      }
      /* Generate a byte based on different methods */
      unsigned char randByte = 0;
      switch (encryptionMethod) {
         case 1:
            srand(seed);
            randByte = rand() % 256;
            break;
         case 2:
            randByte = lcg(seed);
            seed = lcg(seed);
            break;
         case 3:
            /* Uses the sg() function. sg has previously? been init w/ 55 PRGN */
            break;
         default:
            break;
      }

      result[i] = text[i] ^ randByte;  // XOR
   }
   result[textLength] = '\0';
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

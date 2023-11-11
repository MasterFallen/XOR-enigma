#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEED 12       // or Start Value
#define M 2147483648  // Modulus
#define A 1103515245  // Multiplier
#define C 12345       // Increment

/* (C) N. de Boer & A. Janikowska, 2023 */

unsigned int lcg(int prevRandInt) {
   return (A * prevRandInt + C) % M;
}

unsigned int sg(unsigned char *listOfNums) {
   unsigned int randNum = (listOfNums[29] - listOfNums[0]) % M;
   // Shift the array to the left
   for (int i = 0; i < 54; i++) {
      listOfNums[i] = listOfNums[i + 1];
   }
   // Store the new random number at the end of the array
   listOfNums[54] = lcg(listOfNums[53]);
   return randNum;

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

   int encryptionMethod = 2;
   printf("Please enter the desired encryption method: \n\n");
   printf("[1] OS Random number generator, \n");
   printf("[2] Linear congruential generator, \n");
   printf("[3] Subtractive generator, \n");
   scanf("%d", &encryptionMethod);

   unsigned int seed = key;
   
   /* If using Substractive generator, initialize it */
   unsigned char subtractive[55];
   if (encryptionMethod == 3) {
      for (int i = 0; i < 55; i++) {
         subtractive[i] = lcg(seed);
         seed = subtractive[i];
      }
   }

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
            seed = randByte; // Update seed
            break;
         case 2:
            randByte = lcg(seed);
            seed = randByte; // Update seed
            break;
         case 3:
            randByte = sg(subtractive);
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

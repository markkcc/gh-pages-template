#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/evp.h>

#define MAX_PLAINTEXT_LENGTH 21
#define MAX_WORD_LEN 16

/*
MAKEFILE:

INC=/usr/local/ssl/include/
LIB=/usr/local/ssl/lib/
all:
gcc -I$(INC) -L$(LIB) -o enc yourcode.c -lcrypto -ldl
*/

int main(int argc, char *arv[]) {
	
	char plaintext[MAX_PLAINTEXT_LENGTH];
	char ciphertext[100];
	char IV[16] = {0};

	FILE *fp = fopen("words.txt", "r");
	if(fp == NULL) {
		perror("Failed to open the dictionnary \"words.txt\".");
		return EXIT_FAILURE;
	}
	//AES-128-cbc

	printf("Plaintext (total %d characters): ", MAX_PLAINTEXT_LENGTH);
	scanf("%s", plaintext);
	printf("Ciphertext (in hex format): ");
	scanf("%s", ciphertext);

	char word[MAX_WORD_LEN];
	char first_letter = 'A';
	int wordcount = 0;
	printf("Trying words that start with the letter %c... ", first_letter);
	int ret;

	while(42) {

		 ret = fscanf(fp, "%16s", word);
		 if(ret == EOF)
		 	break;
		 wordcount++;

		 if(tolower(word[0]) != first_letter) {
		 	first_letter = word[0];
		 	printf("%c... ", first_letter);
		 }


		 //test
		 printf("%s", word);

	}


}

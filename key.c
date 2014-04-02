#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <openssl/aes.h>

#define MAX_PLAINTEXT_LENGTH 21
#define MAX_WORD_LEN 16
#define MAX_BUF_LEN 100

/*
MAKEFILE:

INC=/usr/local/ssl/include/
LIB=/usr/local/ssl/lib/
all:
	gcc -I$(INC) -L$(LIB) -o enc yourcode.c -lcrypto -ldl
*/

int main(int argc, char *arv[]) {
	
	char plaintext[MAX_PLAINTEXT_LENGTH];
	char ciphertext[MAX_BUF_LEN];
	char temp_cipher[MAX_BUF_LEN];
	char IV[16] = {0};

	FILE *fp = fopen("words.txt", "r");
	if(fp == NULL) {
		perror("Failed to open the dictionnary \"words.txt\".");
		return EXIT_FAILURE;
	}

	memset(ciphertext, 0x00, MAX_BUF_LEN);
	memset(plaintext, '\0', MAX_PLAINTEXT_LENGTH);

	printf("Plaintext (total %d characters): ", MAX_PLAINTEXT_LENGTH);
	scanf("%s", plaintext);
	printf("Ciphertext (in hex format): ");
	scanf("%s", ciphertext);

	AES_KEY aeskey;
	char word[MAX_WORD_LEN];
	char first_letter = 'A';
	int wordcount = 0;
	printf("Trying words that start with the letter %c... ", first_letter);
	int ret;

	while(42) {

		 memset(word, ' ', MAX_WORD_LEN);
		 ret = fscanf(fp, "%16s", word);
		 if(ret == EOF)
		 	break;
		 wordcount++;

		 if(tolower(word[0]) != tolower(first_letter)) {
		 	first_letter = word[0];
		 	printf("%c... ", first_letter);
		 }


		 //Encrypt the plaintext woth the word as the key and compare the ciphertexts
		 memset(temp_cipher, 0x00, MAX_BUF_LEN);
		 AES_set_encrypt_key (word, 128, &aeskey);
		 AES_cbc_encrypt (plaintext, temp_cipher, MAX_PLAINTEXT_LENGTH, &aeskey, IV, AES_ENCRYPT);

		 if(strncmp(ciphertext, temp_cipher, MAX_BUF_LEN) == 0) {
		 	printf("\nKey found! Key without quotes:\n\"%s\"", word);
		 	break;
		 }
	}


}

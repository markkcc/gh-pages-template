#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#define MAX_PLAINTEXT_LENGTH 21
#define MAX_WORD_LEN 16
#define MAX_BUF_LEN 100
#define IV_LEN 16

/*
 * MAKEFILE (tab before gcc):
 * 
 * INC=/usr/local/ssl/include/
 * LIB=/usr/local/ssl/lib/
 * all:
 * 		gcc -I$(INC) -L$(LIB) -o enc key.c -lcrypto -ldl
 */

static void hex_print(const void* pv, size_t len) {

	const unsigned char *p = (const unsigned char*)pv;
	if (NULL == pv)
		printf("NULL");
	else {
		size_t i = 0;
		for(; i<len; ++i)
			printf("%02X", *p++);
	}
	printf("\n");
}

int main(int argc, char *arv[]) {
	
	unsigned char plaintext[MAX_PLAINTEXT_LENGTH];
	unsigned char ciphertext[MAX_BUF_LEN];
	unsigned char temp_cipher[MAX_BUF_LEN];
	unsigned char IV[IV_LEN];

	FILE *fp = fopen("words.txt", "r");
	if(fp == NULL) {
		perror("Failed to open the dictionnary \"words.txt\".");
		return EXIT_FAILURE;
	}

	memset(ciphertext, 0x00, MAX_BUF_LEN);
	memset(plaintext, '\0', MAX_PLAINTEXT_LENGTH);
	memset(IV, 0, IV_LEN);

	printf("Plaintext (total %d characters): ", MAX_PLAINTEXT_LENGTH);
	fgets(plaintext, MAX_PLAINTEXT_LENGTH, stdin);
	printf("Ciphertext (in hex format): ");
	fgets(ciphertext, MAX_BUF_LEN, stdin);

	AES_KEY aeskey;
	unsigned char word[MAX_WORD_LEN];
	int wordcount = 0;
	int ret;

	while(42) {

		 memset(word, ' ', MAX_WORD_LEN);
		 ret = fscanf(fp, "%16s", word);
		 if(ret == EOF)
		 	break;
		 wordcount++;

		 //Encrypt the plaintext with the word as the key and compare the ciphertexts
		 memset(temp_cipher, 0x00, MAX_BUF_LEN);
		 AES_set_encrypt_key (word, 128, &aeskey);
		 AES_cbc_encrypt (plaintext, temp_cipher, MAX_PLAINTEXT_LENGTH, &aeskey, IV, AES_ENCRYPT);

		 if(strncmp(ciphertext, temp_cipher, MAX_BUF_LEN) == 0) {
		 	printf("\nKey found after trying %d words!\nKey without quotes:\n\"%s\"", wordcount, word);
		 	break;
		 }
	}

	printf("\nReached end of word list, aborting.\n");
	hex_print(temp_cipher, sizeof(temp_cipher));


}

/*
 * Sample input:
 * Plaintext (total 21 characters): This is a top secret.
 * Ciphertext (in hex format): 8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9
 */ 

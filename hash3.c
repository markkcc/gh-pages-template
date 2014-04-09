/*
 * Mark El-Khoury
 * April 2014
 * Available at http://mu.gl/key.c
 * 
 * Compile with: gcc hash.c -lcrypto 
 * You also need libssl-dev package
 *
 * Breaks Collision-free property of MD5
 *
 */ 

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <openssl/evp.h>

 #define LOWEST_ASCII 32
 #define HIGHEST_ASCII 126
 #define DIGEST_SIZE 6

 char* digest(char* message) {

 	 EVP_MD_CTX *mdctx;
	 const EVP_MD *md;
 	 char *cropped_dgst = (char *)malloc(DIGEST_SIZE*sizeof(char));
	 memset(cropped_dgst, '\0', DIGEST_SIZE);
	 char digestname[] = "MD5";
	 unsigned char md_value[EVP_MAX_MD_SIZE];
	 int md_len, i;

	 OpenSSL_add_all_digests();

	 md = EVP_get_digestbyname(digestname);

	 //Get the digest for the message
	 mdctx = EVP_MD_CTX_create();
	 EVP_DigestInit_ex(mdctx, md, NULL);
	 EVP_DigestUpdate(mdctx, message, strlen(message));
	 EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	 EVP_MD_CTX_destroy(mdctx);

	 //Print the digest to a string
	 md_len = 3; //use first 24 bits (3 bytes) only
	 for(i = 0; i < md_len; i++)
	 	 sprintf(cropped_dgst + strlen(cropped_dgst), "%02x", md_value[i]);

	 return (char*)cropped_dgst;
 }

 void main(int argc, char *argv[])
 {
	 char *new_msg = (char *)malloc(DIGEST_SIZE/2*sizeof(char));
	 char *message = (char *)malloc(DIGEST_SIZE/2*sizeof(char));
	 int char_range = HIGHEST_ASCII - LOWEST_ASCII;
	 long range_iter = char_range * char_range * char_range;
	 int new_iter = 0, done = 0;
	 long old_iter = 0;
	 memset(new_msg, ' ', DIGEST_SIZE/2);
	 memset(message, ' ', DIGEST_SIZE/2);
	 //when the oold and new iters match, update the msg to be new msg

	 char *cropped_dgst = (char *)malloc(DIGEST_SIZE*sizeof(char));
	 char *temp_dgst = (char *)malloc(DIGEST_SIZE*sizeof(char));
	 char *temp_msg = (char *)malloc(DIGEST_SIZE/2*sizeof(char));
	 int  i, j, k, d;
	 long countwords = 0;

	 EVP_MD_CTX *mdctx;
	 const EVP_MD *md;
	 char digestname[] = "MD5";
	 unsigned char md_value[EVP_MAX_MD_SIZE];
	 int md_len;

	 OpenSSL_add_all_digests();

	 md = EVP_get_digestbyname(digestname);
	 printf("Testing collision-free property ...\n");
	 FILE *f = fopen("collision.txt", "w");
	 if(f == NULL) 
		 printf("Error opening file!\n");

	 while(old_iter < range_iter) {

		 //This string is the one we will try to find by brute force
		 strcpy(message, new_msg);

		 memset(cropped_dgst, '\0', DIGEST_SIZE);
		 memset(temp_dgst, '\0', DIGEST_SIZE);
		 memset(temp_msg, '\0', DIGEST_SIZE/2);

		 mdctx = EVP_MD_CTX_create();
	 	 EVP_DigestInit_ex(mdctx, md, NULL);
	 	 EVP_DigestUpdate(mdctx, message, strlen(message));
	 	 EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	 	 EVP_MD_CTX_destroy(mdctx);

		 memset(cropped_dgst, '\0', DIGEST_SIZE);
	 	 md_len = 3; //use first 24 bits (3 bytes) only
	 	 for(i = 0; i < md_len; i++)
	 	 	 sprintf(cropped_dgst + strlen(cropped_dgst), "%02x", md_value[i]);
		 
		 //printf("Target digest: \"%s\" from message \"%s\"\n", cropped_dgst, message);

		 //Generate 3-character string "ijk" with random ASCII characters i, j and k
		 countwords = 0;
	 	 for(i = LOWEST_ASCII; i <= HIGHEST_ASCII; i++) {

		 	 temp_msg[0] = i;
		 	 for(j = LOWEST_ASCII; j <= HIGHEST_ASCII; j++) {

		 		 temp_msg[1] = j;
				 for(k = LOWEST_ASCII; k <= HIGHEST_ASCII; k++, countwords++) {

					 temp_msg[2] = k;

					 mdctx = EVP_MD_CTX_create();
				 	 EVP_DigestInit_ex(mdctx, md, NULL);
				 	 EVP_DigestUpdate(mdctx, temp_msg, strlen(temp_msg));
				 	 EVP_DigestFinal_ex(mdctx, md_value, &md_len);
				 	 EVP_MD_CTX_destroy(mdctx);

	 			 	 memset(temp_dgst, '\0', DIGEST_SIZE);
				 	 md_len = 3;
				 	 for(d = 0; d < md_len; d++)
				 	 	 sprintf(temp_dgst + strlen(temp_dgst), "%02x", md_value[d]);

					 if(strncmp(temp_dgst, cropped_dgst, DIGEST_SIZE) == 0) {
					 	 
						 if(strncmp(message, temp_msg, DIGEST_SIZE/2) != 0) {
						 	 printf("Collision detected: \"%s\" and \"%s\" yield the same hash value \"%s\"\n", message, temp_msg, cropped_dgst);
							 fprintf(f, "Collision detected: \"%s\" and \"%s\" yield the same hash value \"%s\"\n", message, temp_msg, cropped_dgst);
						 }

					 }
					
					 if(strncmp(new_msg, temp_msg, DIGEST_SIZE/2) == 0 && new_iter != -1) {
					 	 //generate the next string to use in brute force
						 if(j == HIGHEST_ASCII && k == HIGHEST_ASCII) {
						 	 new_msg[0] = i+1;
							 new_msg[1] = LOWEST_ASCII;
							 new_msg[2] = LOWEST_ASCII;
						 }
						 else if(k == HIGHEST_ASCII) {
						 	 new_msg[0] = i;
							 new_msg[1] = j+1;
							 new_msg[2] = LOWEST_ASCII;
						 }
						 else {
						 	 new_msg[0] = i;
							 new_msg[1] = j;
							 new_msg[2] = k+1;
						 }
						 new_iter = -1;
					 }
					 /*if(i == HIGHEST_ASCII && j == HIGHEST_ASCII && k == HIGHEST_ASCII) {
					 	 done = 1;
					 }*/
				 }
			 }

		 }

		 //message = new_msg;
		 old_iter++;
		 new_iter = 0;

	 }

	 free(cropped_dgst);
	 free(temp_dgst);
	 free(temp_msg);
	 fclose(f);

 }


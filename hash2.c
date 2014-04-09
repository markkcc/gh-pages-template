/*
 * Mark El-Khoury
 * April 2014
 * Available at http://mu.gl/key.c
 * 
 * Compile with: gcc hash.c -lcrypto 
 * You also need libssl-dev package
 *
 * Breaks One-way property of MD5
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
	 //This string is the one we will try to find by brute force
	 char message[] = "~z6";

	 char *cropped_dgst = (char *)malloc(DIGEST_SIZE*sizeof(char));
	 char *temp_dgst = (char *)malloc(DIGEST_SIZE*sizeof(char));
	 char *temp_msg = (char *)malloc(DIGEST_SIZE/2*sizeof(char));
	 int  i, j, k, d, countwords;

	 memset(cropped_dgst, '\0', DIGEST_SIZE);
	 memset(temp_dgst, '\0', DIGEST_SIZE);
	 memset(temp_msg, '\0', DIGEST_SIZE);

	 cropped_dgst = digest(message);
	 
	 printf("Target digest: %s (first 24 bits only)\n", cropped_dgst);

	 //Generate 3-character string "ijk" with random ASCII characters i, j and k
	 printf("Brute-forcing plaintext ...\n");
	 countwords = 0;
 	 for(i = LOWEST_ASCII; i <= HIGHEST_ASCII; i++) {

	 	 temp_msg[0] = i;
	 	 for(j = LOWEST_ASCII; j <= HIGHEST_ASCII; j++) {

	 		 temp_msg[1] = j;
			 for(k = LOWEST_ASCII; k <= HIGHEST_ASCII; k++, countwords++) {

				 temp_msg[2] = k;

				 temp_dgst = digest(temp_msg);

				 if(strncmp(temp_dgst, cropped_dgst, DIGEST_SIZE) == 0) {
				 	 printf("One-way property broken: Plaintext found after %d tries!\nString \"%s\" yields hash \"%s\"\n", countwords, temp_msg, temp_dgst);
				 	 
					 if(strncmp(message, temp_msg, 3) != 0)
					 	 printf("Collision detected: Collision-free property broken.\nStrings \"%s\" and \"%s\" yield the same hash value \"%s\"\n", message, temp_msg, cropped_dgst);

				 }
			 }
		 }

	 }

	 free(cropped_dgst);
	 free(temp_dgst);
	 free(temp_msg);


 }


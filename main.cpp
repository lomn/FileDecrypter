#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 32
#define BUFF_SIZE 1024

void decrypt_main (FILE*,FILE*);
void decrypt_buffer (unsigned char*, size_t, long int);
void decrypt_file(FILE* in, FILE* out, long int key);

int main(int argc,char** argv)
{
	char path[128];
	FILE *in, *out;

	//Only one argument needed
	if(argc != 2){
		printf("[-] Usage : %s <file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	//prevent overflow :D
	if(strlen(argv[1])>=128){
		printf("[-] Path is too long");
		return EXIT_FAILURE;
	}

	//copy the path with extension of the output
	snprintf(path, sizeof(path)-1, "%s.bmp", argv[1]);

	//Check for error while opening the source file
	in = fopen(argv[1], "r");
	if(in == NULL){
		printf("[-] Error while openning : %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	//Check for error while opening the output file
	out = fopen(path, "w");
	if(out == NULL){
		printf("[-] Error while openning : %s \n\n", path);
	}

	decrypt_main(in, out);

	printf("[+] File %s as been decrypted with success !\n", argv[1]);
	printf("[+] Done\n");
}

void decrypt_main (FILE *in,FILE *out){
	long int key = 0xa12c3f; //0xdeadbeef
	printf("[+] Decrypting\n");
	
	//while(key < 0xffffff){
		decrypt_file(in, out, key);
		key++;
	//}
	return;
}

void decrypt_buffer (unsigned char* buffer, size_t size, long int key)
{
	size_t i = 0;
	int j;

	//Simply XOR the buffer with the key which is 32 bytes long
	for(i=0; i < size; i++){
		buffer[i] ^= key;
	}
}

void decrypt_file(FILE* in, FILE* out, long int key){
	unsigned char buffer[BUFF_SIZE];
	unsigned char* pbuffer = buffer;
	size_t size;
	int counter=1;

	//in this order, read the file and copy it to the buffer, decrypt the buffer and then write it to the output file
	do {
    size = fread(buffer, 1, BUFF_SIZE, in);
    decrypt_buffer(pbuffer, size, key);
    fwrite(buffer, 1, size, out);
    counter++;
  }while(size == BUFF_SIZE);
  printf("[-_-] Buffer filled for : %d times \n", counter);
}
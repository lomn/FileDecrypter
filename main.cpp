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

	if(argc != 2){
		printf("[-] Usage : %s <file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	snprintf(path, sizeof(path)-1, "%s.bmp", argv[1]);

	in = fopen(argv[1], "r");
	if(in == NULL){
		printf("[-] Error while openning : %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	out = fopen(path, "w");
	if(out == NULL){
		printf("[-] Error while openning : %s \n\n", path);
	}

	decrypt_main(in, out);

	printf("[+] File %s as been decrypted with success !\n", argv[1]);
	printf("[+] Done\n");
}

void decrypt_main (FILE *in,FILE *out){
	long int key = 0x0; //0xdeadbeef
	printf("[+] Decrypting\n");
	
	while(key < 0xffffff){
		decrypt_file(in, out, key);
		key++;
	}
	return;
}

void decrypt_buffer (unsigned char* buffer, size_t size, long int key)
{
	size_t i = 0;
	int j;

	for(i=0; i < size; i++){
		buffer[i] ^= key;
	}
}

void decrypt_file(FILE* in, FILE* out, long int key){
	unsigned char buffer[BUFF_SIZE];
	unsigned char* pbuffer = buffer;
	size_t size;

	do {
    size = fread(buffer, 1, BUFF_SIZE, in);
    decrypt_buffer(pbuffer, size, key);
    fwrite(buffer, 1, size, out);
  }while(size == BUFF_SIZE);
}
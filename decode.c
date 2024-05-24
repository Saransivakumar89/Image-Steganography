#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"
#include <stdlib.h>

//Read and validate decode args

Status_d read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
	{
		decInfo->d_src_image_fname = argv[2];
	}
	else
		return d_failure;
	if (argv[3] != NULL)
		decInfo->d_secret_fname = argv[3];
	else
		decInfo->d_secret_fname = "output.txt";
	return d_success;
}


//Open files


Status_d open_files_dec(DecodeInfo *decInfo)
{
	//Stego Image file
	decInfo->fptr_d_src_image = fopen(decInfo->d_src_image_fname, "r");

	//Do Error handling
	if (decInfo->fptr_d_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->d_src_image_fname);
		return d_failure;
	}
	//Dest file
	decInfo->fptr_d_secret = fopen(decInfo->d_secret_fname, "w");

	//Do Error handling
	if (decInfo->fptr_d_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->d_secret_fname);
		return d_failure;
	}

	// If no failure then return e_success
	return d_success;
}

//Do decoding

Status_d do_decoding(DecodeInfo *decInfo)
{


	if (open_files_dec(decInfo) == d_success)
	{
		printf("Open files is a success\n");
	}
	else
	{
		printf("Open files is a failure\n");
		return d_failure;
	}



	if (decode_magic_string(decInfo) == d_success)
	{
		printf("Decoded magic string Successfully\n");
	}
	else
	{
		printf("Decoding of magic string is a failure\n");
		return d_failure;
	}


	if (decode_file_extn_size(strlen(".txt"), decInfo->fptr_d_src_image) == d_success)
	{
		printf("Decoded file extension size Successfully\n");
	}
	else
	{
		printf("Decoded of file extension size is a failure\n");
		return d_failure;
	}


	if (decode_secret_file_extn(decInfo->d_extn_secret_file, decInfo) == d_success)
	{
		printf("Decoded Secret File Extension Successfully\n");
	}
	else
	{
		printf("Decode of Secret file extension is a failure\n");
		return d_failure;
	}


	if (decode_secret_file_size(decInfo->size_secret_file, decInfo) == d_success)
	{
		printf("Decoded secret file size Successfully\n");
	}
	else
	{
		printf("Decode of secret file size is a failure\n");
		return d_failure;
	}


	if (decode_secret_file_data(decInfo) == d_success)
	{
		printf("Decoded secret file data Successfully\n");
	}
	else
	{
		printf("Decoding of secret file data is a failure\n");
		return d_failure;
	}

	return d_success;
}

//Decode Magic string

Status_d decode_magic_string(DecodeInfo *decInfo)
{
	fseek(decInfo->fptr_d_src_image,54,SEEK_SET);
	char magic_str[3];
	char buff[8];
	char usr[20];
	printf("Enter the password : ");
	scanf(" %s",usr);
	int i;
	for(i=0 ;i<2;i++)
	{
		fread(buff,sizeof(buff),1,decInfo->fptr_d_src_image);
		magic_str[i] = decode_byte_from_lsb(buff);
		
	}
	magic_str[2]='\0';

	if(strcmp(magic_str,usr)==0)
	{
	
		return d_success;
	}

	else
	{

		printf("Invalid Password\n");
		return d_failure;
	}

}

//Decode byte from lsb

char decode_byte_from_lsb( char *image_buffer)
{
	char ch = 0; 

	for (int i = 0; i < 8; i++)
	{
		if(image_buffer[i]&01)
		{
			ch=ch|(1<<i);
		}
	}	
	return ch;
}

//Decode file extn size

Status_d decode_file_extn_size(int size, FILE *fptr_d_src_image)
{
	char str[32];
	int length;

	fread(str, 32, sizeof(char), fptr_d_src_image);
	decode_size_from_lsb(str, &length);

	if (length == size)
		return d_success;
	else
		return d_failure;
}

//Decode size from lsb

Status_d decode_size_from_lsb(char *buffer, int *size)
{
	int num = 0;
	for (int i = 0; i < 32; i++)
	{
		if(buffer[i]&01)
		{
			num=num|(1<<i);
		}
	}
	*size=num;
}

//Decode secret file extn

Status_d decode_secret_file_extn(char *file_ext, DecodeInfo *decInfo)
{
	file_ext = ".txt";
	int i = strlen(file_ext);
	decInfo->d_extn_secret_file = malloc(i + 1);
	decode_extension_data_from_image(strlen(file_ext), decInfo->fptr_d_src_image, decInfo);

	decInfo->d_extn_secret_file[i] = '\0';

	if (strcmp(decInfo->d_extn_secret_file, file_ext) == 0)
	{
		return d_success;
	}

	else
	{
		return d_failure;
	}

}

//Decode Exsention data from image

Status_d decode_extension_data_from_image(int size, FILE *fptr_d_src_image, DecodeInfo *decInfo)
{
	for (int i = 0; i < size; i++)
	{
		fread(decInfo->d_src_image_fname, 8, 1, fptr_d_src_image);
		decInfo->d_extn_secret_file[i]=decode_byte_from_lsb(decInfo->d_src_image_fname);
	}

	return d_success;
}


//Decode secret file size

Status_d decode_secret_file_size(int file_size, DecodeInfo *decInfo)
{
	char str[32];
	fread(str, 32, sizeof(char), decInfo->fptr_d_src_image);
	decode_size_from_lsb(str, &file_size);
	decInfo->size_secret_file = file_size;

	return d_success;
}


//Decode Secret file

Status_d decode_secret_file_data(DecodeInfo *decInfo)
{
	char ch;
	for (int i = 0; i < decInfo->size_secret_file; i++)
	{
		fread (decInfo->d_src_image_fname, 8, sizeof(char), decInfo->fptr_d_src_image);
		ch=decode_byte_from_lsb(decInfo->d_src_image_fname);
		fputc(ch, decInfo->fptr_d_secret);
	}
	return d_success;
}



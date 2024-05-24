#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include <string.h>

int main(int argc, char *argv[])
{
	//Check Operations type
	if (check_operation_type(argv) == e_encode)
	{
		printf("Selected encoding\n");

	
		EncodeInfo encInfo;
	         //Read and validate encode args
		if (read_and_validate_encode_args(argv, &encInfo) == e_success)
		{
			printf("Read and validate encode arguments is a success\n");


		        //Do Encoding
			if (do_encoding(&encInfo) == e_success)
			{
				printf("Encoded Succesfully\n");
			}
			else
			{
				printf("Failed to encode\n");
				return e_failure;
			}
		}
		else
		{
			printf("Read and validate encode arguments is a failure\n");

			return e_failure;
		}
	}
	
	//Check operation type
	else if (check_operation_type(argv) == e_decode)
	{
		printf("Selected decoding\n");

	
		DecodeInfo decInfo;
		if (read_and_validate_decode_args(argv, &decInfo) == d_success)
		{
			printf("Read and validate decode arguments is a success\n");


		        //Do Decoding
			if (do_decoding(&decInfo) == d_success)
			{
				printf("Decoded Successfully\n");
			}
			else
			{
				printf("Failed to decode\n");

				return e_failure;
			}
		}  
		else
		{
			printf("Read and validate decode arguments is a failure\n");
			return e_failure;
		}
	} 
	else
	{
		printf("Invalid option\nKindly pass for\nEncoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\nDecoding: ./a.out -d stego.bmp decode.txt\n");
	}
	return 0;
}

OperationType check_operation_type(char *argv[])
{    		
	if(strcmp(argv[1],"-e") == 0)
	{		
		return e_encode;
	}  
	else if (strcmp(argv[1],"-d") == 0)
	{	
		return e_decode;
	} 
	else 
	{
		return e_unsupported;
	}
	return 0;
}

#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
/* Function Definitions */
Status do_decoding(DecodeInfo *decInfo)                         				//start decoding
{
    if(open_decode_files(decInfo) == e_success)                          			//check if files exist
    {
	if(decode_magic_string(decInfo) == e_success)                   			//decode magic string
	{
	    puts("INFO: Done");
	    if(decode_file_extn_size(decInfo) == e_success)
	    {
		puts("INFO: Done");
		if(decode_file_extn(decInfo) == e_success)
		{
		    puts("INFO: Done");
		    if(open_output_file(decInfo)==e_success)
		    {
			puts("INFO: Done. Opened all required files");
			if(decode_secret_file_size(decInfo) == e_success)
			{
			    puts("INFO: Done");
			    if(decode_secret_data(decInfo) == e_success)
			    {
				puts("INFO: Done");
			    }
			    else
			    {
				printf("INFO: decoding failed\n");
			    }
			}
			else
			{
			    printf("ERROR: File size not decoded\n");
			}
		    }
		    else
			printf("ERROR: Output file does not exist\n");
		}
		else
		    printf("ERROR: File extn not decoded\n");
	    }
	    else
	    {
		printf("ERROR: extension not decoded\n");
	    }
	}
	else
	{
	    printf("ERROR: Magic string not decoded\n");
	}
    }	
    else
    {
	printf("ERROR: Files dont exist\n");
    }
}

Status open_decode_files(DecodeInfo *decInfo)                 							//opening files
{
    puts("INFO: Opening required files");
    decInfo->fptr_stego_image = fopen(decInfo->stego_image,"r");
    if(decInfo->fptr_stego_image == NULL)
    {
	printf("Error : Given bmp file doesnt exist\n");
	return e_failure;
    }
    puts("INFO: Opened steged_img.bmp");
    return e_success;
}
Status open_output_file(DecodeInfo *decInfo)
{
    decInfo->fptr_output = fopen(decInfo->output_fname,"w");
    if(decInfo->fptr_output == NULL)
    {
	printf("Error : Output file doesnt exist\n");
	return e_failure;
    }
    printf("INFO: Opened %s\n",decInfo->output_fname);
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)              							//decode magic string
{
    puts("INFO: Decoding Magic String Signature");
    char buf[8];
    char magic_string[3];
    rewind(decInfo->fptr_stego_image);
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    for(int i=0;i<2;i++)
    {
	fread(buf,8,1,decInfo->fptr_stego_image);
	decode_lsb_to_byte(&magic_string[i],buf);
    }
    //Check whether the magic string decoded is same or not
    if(!strcmp(magic_string,MAGIC_STRING))
	return e_success;
    else
	return e_failure;
}


Status decode_file_extn_size(DecodeInfo *decInfo)                                           			//file extension size
{
    puts("INFO: Decoding Output File Extension Size");
    char buf[32];
    fread(buf,32,1,decInfo->fptr_stego_image);
    decInfo->file_extn_size = decode_lsb_to_num(buf);
    if(decInfo->file_extn_size)
	return e_success;
    else
	return e_failure;
}

Status decode_file_extn(DecodeInfo *decInfo)                 							//fetch file extension
{
    puts("INFO: Decoding Output File Extension");
    char data[4];
    char buf[8];
    for(int i=0;i<decInfo->file_extn_size;i++)
    {
	fread(buf,8,1,decInfo->fptr_stego_image);
	decode_lsb_to_byte(&data[i],buf);
    }
    int i=0;
    if(strcmp(decInfo->file_extn,".txt") && strcmp(decInfo->file_extn,".c") && strcmp(decInfo->file_extn,".sh"))
    {
	while(i<decInfo->file_extn_size)
	    decInfo->output_fname[decInfo->out_file_size++]=data[i++];               				//concatenate file name
	return e_success;
    }
    else
	return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)                       					//secret file size decode
{
    puts("INFO: Decoding File Size");
    char buf[32];
    fread(buf,32,1,decInfo->fptr_stego_image);
    decInfo->size_secret_file = decode_lsb_to_num(buf);
    if(decInfo->size_secret_file)
	return e_success;
    else
	return e_failure;
}

Status decode_secret_data(DecodeInfo *decInfo)                           					//decode secret data from image
{
    puts("INFO: Decoding File Data");
    char secret_data[decInfo->size_secret_file];
    char buf[8];
    for(int i=0;i<decInfo->size_secret_file;i++)
    {
	fread(buf,8,1,decInfo->fptr_stego_image);
	decode_lsb_to_byte(&secret_data[i],buf);
	fwrite(&secret_data[i],1,1,decInfo->fptr_output);
    }
    return e_success;
}

Status decode_lsb_to_byte(char *temp_arr,char *image_buffer)                                  			//fetch lsb
{
    char data = 0;
    for(int i=0;i<sizeof(image_buffer);i++)
    {
	data = data | ((image_buffer[i] & 1) << i);
    }
    *temp_arr = data;
}

Status decode_lsb_to_num(char *image_buffer)                                    				//fetching lsb of a number
{
    uint num=0;
    int i,j=0;
    for(i=0;i<(sizeof(num)*8);i++)
    {
	num = num | ((image_buffer[i] & 1) << i);
    }
    return num;
}

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
	puts("INFO: Done");
	encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
	encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
	puts("INFO: Done.Not Empty");

	if(check_capacity(encInfo)==e_success)
	{
	    puts("INFO: Done.Found OK");
	    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	    {
		puts("INFO: Done");
		if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
		{
		    puts("INFO: Done");
		    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success)
		    {
			puts("INFO: Done");
			if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
			{
			    puts("INFO: Done");
			    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
			    {
				puts("INFO: Done");
				if(encode_secret_file_data(encInfo)==e_success);
				{
				    puts("INFO: Done");
				    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
				    {
					puts("INFO: Done");
				    }
				    else
				    {
					puts("Error"); 
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
}

//get the size of secret file
uint get_file_size(FILE *fptr)
{
    puts("INFO: Checking for secret file size");
    uint sec_size;
    fseek(fptr,0,SEEK_END);
    sec_size=ftell(fptr);
    return sec_size;
}

//check the capacity of the bmp file
Status check_capacity(EncodeInfo *encInfo)
{
    printf("INFO: Checking for SkeletonCode/beautiful.bmp capacity to handle secret%s\n",encInfo->extn_secret_file);
    uint secret_capacity;
    secret_capacity=(2+sizeof(int)+strlen(encInfo->extn_secret_file)+sizeof(encInfo->size_secret_file)+encInfo->size_secret_file)*8;
    if(encInfo->image_capacity>=secret_capacity)
	return e_success;
    else
	return e_failure;
}

//copy the bmp header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    puts("INFO: Copying Image Header");
    char buf[60];
    rewind(fptr_src_image);
    fread(buf,54,1,fptr_src_image);
    fwrite(buf,54,1,fptr_dest_image);
    return e_success;
}

//enocde the magic string
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{
    puts("INFO: Encoding Magic String Signature");
    char buffer[8];
    for(int i=0; i<strlen(magic_string); i++)
    {
	fread(buffer,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(magic_string[i],buffer);
	fwrite(buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

//encode the secret file extension size
Status encode_secret_file_extn_size(uint file_extn_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret%s File Extension Size\n",encInfo->extn_secret_file);
    char buf_size[32];
    fread(buf_size,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_extn_size,buf_size);
    fwrite(buf_size,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//encode the secret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret%s File Extension\n",encInfo->extn_secret_file);
    char buffer[8];
    for(int i=0; i<strlen(file_extn); i++)
    {
	fread(buffer,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(file_extn[i],buffer);
	fwrite(buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

//encode secret file size
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret%s File Size\n",encInfo->extn_secret_file);
    char buf_size[32];
    fread(buf_size,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,buf_size);
    fwrite(buf_size,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret%s File Data\n",encInfo->extn_secret_file);
    rewind(encInfo->fptr_secret);
    fread(encInfo->secret_data,(encInfo->size_secret_file),1,encInfo->fptr_secret);
    if(encode_data_to_image(encInfo->secret_data, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
	return e_success;
}

//encode data to image
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    for(int i=0; i<size; i++)
    {
	fread(buffer,8,1,fptr_src_image);
	encode_byte_to_lsb(data[i],buffer);
	fwrite(buffer,8,1,fptr_stego_image);
    }
    return e_success;
}

//copy the remaining image data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    puts("INFO: Copying Left Over Data");
    fseek(fptr_src,0,SEEK_END);
    long last_byte=ftell(fptr_src);
    rewind(fptr_src);
    long start_byte=ftell(fptr_dest);
    long buffer_size=last_byte-start_byte;
    char buffer[buffer_size];
    fseek(fptr_src,start_byte,SEEK_SET);
    fread(buffer,buffer_size,1,fptr_src);
    fwrite(buffer,buffer_size,1,fptr_dest);
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0; i<sizeof(image_buffer); i++)
    {
	image_buffer[i] = ((data & (1<<i)) >> i ) | (image_buffer[i] & (~(1<<0)));
    }
    return e_success;
}

Status encode_size_to_lsb(uint data, char *image_buffer)
{
    for(int i=0; i<32; i++)
    {
	image_buffer[i] = ((data & (1<<i)) >> i ) | (image_buffer[i] & (~(1<<0)));
    }
    return e_success;
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 e* Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    puts("INFO: Opening the required files");

    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	puts("Hello");
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }
    puts("INFO: Opened SkeletonCode/beautiful.bmp");

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	puts("Hi");
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }
    puts("INFO: Opened secret.txt");

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }
    puts("INFO: Opened steged_img.bmp");

    // No failure return e_success
    return e_success;
}

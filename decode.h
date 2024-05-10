#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/*
   structure to store the data retrieved from stegged bmp file
 */


typedef struct _DecodeInfo
{
    char *stego_image;
    char output_fname[50];
    FILE *fptr_output;
    FILE *fptr_stego_image;
    int out_file_size;
    char file_extn[10];
    int file_extn_size;
    char extn_secret_file[50];
    long size_secret_file;

}DecodeInfo;

/* This prototype will check output arguments are properly provided */

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/* This will extract the lsb's of bmp file and store it in output file */

Status do_decoding(DecodeInfo *decInfo);

/*This will check whether the files exits */

Status open_decode_files(DecodeInfo *decInfo);

/* This will get magic string from stego.bmp */

Status open_output_file(DecodeInfo *decInfo);

/* This will get magic string from stego.bmp */

Status decode_magic_string(DecodeInfo *decInfo);

/* This fetches lsb's from bmp file */

Status decode_lsb_to_byte(char *temp_arr,char *image_buffer);

/* This fetches the file extension size */

Status decode_file_extn_size(DecodeInfo *decInfo);

/* This decodes the lsb's from image buffer and converts into number */

Status decode_lsb_to_num(char *image_buffer);

/* This decodes the file extension of secret text */

Status decode_file_extn(DecodeInfo *decInfo);

/* This obtains the file size of secret file */

Status decode_secret_file_size(DecodeInfo *decInfo);

/* This decodes the actual secret data from image file and reads to output file */

Status decode_secret_data(DecodeInfo *decInfo);

#endif

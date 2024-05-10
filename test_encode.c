#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
    if(argc >= 3)
    {
	if(check_operation_type(argv) == e_encode)
	{
	    if(read_and_validate_encode_args(argv,&encInfo) == e_success)
		do_encoding(&encInfo);
	    else
		puts("ERROR : Please pass proper \"bmp\" file\nLike : ./a.out -e \"beautiful.bmp\" .txt");
	}
	else if(check_operation_type(argv) == e_decode)
	{
	    if(read_and_validate_decode_args(argv,&decInfo)==e_success)
		do_decoding(&decInfo);
	    else
		puts("ERROR : Please pass proper \"bmp\" file\nLike : ./a.out -d \"stego.bmp\"");
	}
	else if(check_operation_type(argv) == e_unsupported)	
	{
	    printf("ERROR : Please pass the arguments\nLike : ./a.out -e beautiful\".bmp\" .txt\n");
	    exit(0);
	}
    }
    else
    {
	printf("ERROR : Please pass the arguments\nLike : ./a.out -e beautiful.bmp .txt\n");
	exit(0);
    }

    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(!strcmp(argv[1],"-e"))
	return e_encode;
    else if(!strcmp(argv[1],"-d"))
	return e_decode;
    else
	return e_unsupported;
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char extension1[10];
    char extension3[10];

    char *cptr;
    if(strrchr(argv[2],'.'))
	cptr=strrchr(argv[2],'.');
    else
	return e_failure;

    sprintf(extension1,"%s",cptr);

    if(!strcmp(extension1,".bmp"))
    {
	encInfo->src_image_fname = argv[2];
	if(argv[3])
	{
	    char *cptr1=strrchr(argv[3],'.');
	    sprintf(encInfo->extn_secret_file,"%s",cptr1);
	    if(!strcmp(encInfo->extn_secret_file,".txt") || !strcmp(encInfo->extn_secret_file,".c") || !strcmp(encInfo->extn_secret_file,".sh"))
	    {
		encInfo->secret_fname=argv[3];
		if(argv[4])
		{
		    char *cptr2=strrchr(argv[4],'.');
		    sprintf(extension3,"%s",cptr2);
		    if(!strcmp(extension3,".bmp"))
		    {
			encInfo->stego_image_fname=argv[4];
			return e_success;
		    }
		    else
		    {
			printf("ERROR1 : Please pass the arguments\nLike : ./a.out -e beautiful\".bmp\" .txt\n");
			return e_failure;
		    }
		}
		else
		{
		    encInfo->stego_image_fname="stego.bmp";
		    encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"w");
		    fclose(encInfo->fptr_stego_image);
		    return e_success;
		}

	    }
	}
	else
	    return e_failure;
    }
    else
    {
	printf("ERROR1 : Please pass the arguments\nLike : ./a.out -e beautiful\".bmp\" .txt\n");
	return e_failure;
    }
}


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char extension[50];
    if(argv[2])
    {
	char *cptr;
	if(strrchr(argv[2],'.'))
	    cptr=strrchr(argv[2],'.');
	else
	    return e_failure;

	sprintf(extension,"%s",cptr);
	if(!strcmp(extension,".bmp"))
	{
	    decInfo->stego_image=argv[2];
	    if(argv[3])
	    {
		char *cptr1;
		if(strrchr(argv[3],'.'))
		{
		    cptr1 = strrchr(argv[3],'.');
		    sprintf(decInfo->file_extn,"%s",cptr1);
		}
		decInfo->out_file_size=0;
		while(argv[3][decInfo->out_file_size])
		{
		    decInfo->output_fname[decInfo->out_file_size]=argv[3][decInfo->out_file_size];
		    decInfo->out_file_size++;
		}
		return e_success;
	    }
	    else
	    {
		decInfo->out_file_size=0;
		char out_file[50]="output";
		while(out_file[decInfo->out_file_size])
		{
		    decInfo->output_fname[decInfo->out_file_size]=out_file[decInfo->out_file_size];
		    decInfo->out_file_size++;
		}
		return e_success;
	    }
	}
	else
	    return e_failure;
    }
    else
	return e_failure;
}

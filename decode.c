#include <stdio.h>
#include <string.h>
#include "decode.h"

#define WAV_HEADER 44

Status decode_data_from_image(char *data,int size,FILE *src)
{
    char buffer[8];
    for(int i=0;i<size;i++)
    {
        fread(buffer,1,8,src);
        decode_byte_from_lsb(&data[i],buffer);
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data,char *buffer)
{
    char ch=0;
    for(int i=0;i<8;i++)
    {
        ch<<=1;
        ch|=buffer[i]&1;
    }
    *data=ch;
    return e_success;
}

Status decode_magic_string(DecodeInfo *dec)
{
    char magic[4]={0};
    decode_data_from_image(magic,3,dec->fptr_stego_image);
    return strcmp(magic,MAGIC_STRING)==0?e_success:e_failure;
}

Status decode_secret_file_extn_size(DecodeInfo *dec)
{
    return decode_data_from_image((char*)&dec->extn_size,sizeof(int),dec->fptr_stego_image);
}

Status decode_secret_file_extn(DecodeInfo *dec)
{
    decode_data_from_image(dec->extn_secret_file,dec->extn_size,dec->fptr_stego_image);
    dec->extn_secret_file[dec->extn_size]='\0';
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *dec)
{
    return decode_data_from_image((char*)&dec->size_secret_file,sizeof(long),dec->fptr_stego_image);
}

Status decode_secret_file_data(DecodeInfo *dec)
{
    char ch;
    for(long i=0;i<dec->size_secret_file;i++)
    {
        decode_data_from_image(&ch,1,dec->fptr_stego_image);
        fwrite(&ch,1,1,dec->fptr_secret);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *dec)
{
    dec->fptr_stego_image=fopen(dec->stego_image_fname,"rb");
    fseek(dec->fptr_stego_image,WAV_HEADER,SEEK_SET);

    if(decode_magic_string(dec)==e_failure) return e_failure;
    decode_secret_file_extn_size(dec);
    decode_secret_file_extn(dec);

    sprintf(dec->secret_fname,"decoded%s",dec->extn_secret_file);
    dec->fptr_secret=fopen(dec->secret_fname,"wb");

    decode_secret_file_size(dec);
    decode_secret_file_data(dec);

    fclose(dec->fptr_stego_image);
    fclose(dec->fptr_secret);
    return e_success;
}

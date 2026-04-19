#include <stdio.h>
#include <string.h>
#include "encode.h"

#define WAV_HEADER 44

Status encode_data_to_image(char *data, int size, FILE *src, FILE *dest)
{
    char buffer[8];
    for(int i=0;i<size;i++)
    {
        fread(buffer,1,8,src);
        encode_byte_to_lsb(data[i],buffer);
        fwrite(buffer,1,8,dest);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *buffer)
{
    for(int i=7;i>=0;i--)
    {
        buffer[7-i] &= 0xFE;
        buffer[7-i] |= ((data>>i)&1);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *src, FILE *dest)
{
    char ch;
    while(fread(&ch,1,1,src)==1)
        fwrite(&ch,1,1,dest);
    return e_success;
}

Status encode_magic_string(const char *magic, EncodeInfo *enc)
{
    return encode_data_to_image((char*)magic, strlen(magic),
                                enc->fptr_src_image, enc->fptr_stego_image);
}

Status encode_secret_file_extn_size(EncodeInfo *enc)
{
    enc->extn_size = strlen(enc->extn_secret_file);
    return encode_data_to_image((char*)&enc->extn_size, sizeof(int),
                                enc->fptr_src_image, enc->fptr_stego_image);
}

Status encode_secret_file_extn(EncodeInfo *enc)
{
    return encode_data_to_image(enc->extn_secret_file, enc->extn_size,
                                enc->fptr_src_image, enc->fptr_stego_image);
}

Status encode_secret_file_size(EncodeInfo *enc)
{
    fseek(enc->fptr_secret,0,SEEK_END);
    enc->size_secret_file = ftell(enc->fptr_secret);
    rewind(enc->fptr_secret);
    return encode_data_to_image((char*)&enc->size_secret_file, sizeof(long),
                                enc->fptr_src_image, enc->fptr_stego_image);
}

Status encode_secret_file_data(EncodeInfo *enc)
{
    char ch;
    while(fread(&ch,1,1,enc->fptr_secret)==1)
        encode_data_to_image(&ch,1,enc->fptr_src_image,enc->fptr_stego_image);
    return e_success;
}

Status do_encoding(EncodeInfo *enc)
{
    enc->fptr_src_image=fopen(enc->src_image_fname,"rb");
    enc->fptr_secret=fopen(enc->secret_fname,"rb");
    enc->fptr_stego_image=fopen(enc->stego_image_fname,"wb");

    char header[WAV_HEADER];
    fread(header,1,WAV_HEADER,enc->fptr_src_image);
    fwrite(header,1,WAV_HEADER,enc->fptr_stego_image);

    char *dot = strrchr(enc->secret_fname,'.');
    strcpy(enc->extn_secret_file,dot);

    encode_magic_string(MAGIC_STRING,enc);
    encode_secret_file_extn_size(enc);
    encode_secret_file_extn(enc);
    encode_secret_file_size(enc);
    encode_secret_file_data(enc);
    copy_remaining_img_data(enc->fptr_src_image,enc->fptr_stego_image);

    fclose(enc->fptr_src_image);
    fclose(enc->fptr_secret);
    fclose(enc->fptr_stego_image);
    return e_success;
}

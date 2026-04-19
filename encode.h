#ifndef ENCODE_H
#define ENCODE_H
#include <stdio.h>
#include "types.h"
#include "common.h"

#define MAX_FILE_SUFFIX 8

typedef struct _EncodeInfo
{
    char *src_image_fname;
    FILE *fptr_src_image;

    char *secret_fname;
    FILE *fptr_secret;

    char extn_secret_file[MAX_FILE_SUFFIX];
    int extn_size;
    long size_secret_file;

    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;

Status do_encoding(EncodeInfo *encInfo);
Status encode_magic_string(const char *magic, EncodeInfo *encInfo);
Status encode_secret_file_extn_size(EncodeInfo *encInfo);
Status encode_secret_file_extn(EncodeInfo *encInfo);
Status encode_secret_file_size(EncodeInfo *encInfo);
Status encode_secret_file_data(EncodeInfo *encInfo);
Status encode_data_to_image(char *data, int size, FILE *src, FILE *dest);
Status encode_byte_to_lsb(char data, char *buffer);
Status copy_remaining_img_data(FILE *src, FILE *dest);

#endif

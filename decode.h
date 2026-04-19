#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include "types.h"
#include "common.h"

typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char secret_fname[50];
    FILE *fptr_secret;

    char extn_secret_file[8];
    int extn_size;
    long size_secret_file;

} DecodeInfo;

Status do_decoding(DecodeInfo *decInfo);
Status decode_magic_string(DecodeInfo *decInfo);
Status decode_secret_file_extn_size(DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);
Status decode_data_from_image(char *data, int size, FILE *src);
Status decode_byte_from_lsb(char *data, char *buffer);

#endif

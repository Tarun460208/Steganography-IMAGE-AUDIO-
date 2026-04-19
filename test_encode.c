#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"

int main(int argc,char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    {
        EncodeInfo enc;
        enc.src_image_fname=argv[2];
        enc.secret_fname=argv[3];
        enc.stego_image_fname=argv[4];
        do_encoding(&enc);
        printf("INFO: Encoding completed successfully\n");
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        DecodeInfo dec;
        dec.stego_image_fname=argv[2];
        do_decoding(&dec);
        printf("INFO: Decoding completed successfully\n");
    }
    return 0;
}

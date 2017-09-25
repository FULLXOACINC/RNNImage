
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#pragma pack(2)




void  get_rgb_from_img(char* file){
    FILE *inFile;
    BmpHeader header;
    BmpImageInfo info;
    Rgb *palette;
    int i = 0;


    inFile = fopen( file, "rb" );
    if( !inFile ) {
    }

    if( fread(&header, 1, sizeof(BmpHeader), inFile) != sizeof(BmpHeader) ) {
        printf( "Error reading bmp header.\n" );
    }

    if( fread(&info, 1, sizeof(BmpImageInfo), inFile) != sizeof(BmpImageInfo) ) {
        printf( "Error reading image info.\n" );
    }

    if( info.numColors > 0 ) {
        printf( "Reading palette.\n" );
        palette = (Rgb*)malloc(sizeof(Rgb) * info.numColors);
        if( fread(palette, sizeof(Rgb), info.numColors, inFile) != (info.numColors * sizeof(Rgb)) ) {
            printf( "Error reading palette.\n" );
        }
    }

    FILE *outFile = fopen( "hf", "wb" );
    if( !outFile ) {
        printf( "Error opening outputfile.\n" );
    }

    Rgb *pixel = (Rgb*) malloc( sizeof(Rgb));

    matrix = (Rgb **)malloc(info.height * sizeof(Rgb *));
       for(i = 0; i < info.height; i++)
           matrix[i] = (Rgb *)malloc(info.width * sizeof(Rgb));

    int read, j;
    for(j=0; j<info.height; j++ ) {
        printf( "------ Row %d\n", j+1 );
        read = 0;
        for(i=0; i<info.width; i++ ) {
            if( fread(pixel, 1, sizeof(Rgb), inFile) != sizeof(Rgb) ) {
                printf( "Error reading pixel!\n" );
            }
            read += sizeof(Rgb);



            //printf( "Pixel %d: %3d %3d %3d\n", i+1, pixel->red, pixel->green, pixel->blue );
            matrix[j][i].blue=pixel->blue;
            matrix[j][i].red=pixel->red;
            matrix[j][i].green=pixel->green;
        }
        if( read % 4 != 0 ) {
            read = 4 - (read%4);
            printf( "Padding: %d bytes\n", read );
            fread( pixel, read, 1, inFile );
        }
    }

    printf( "Done.\n" );
    fclose(inFile);
    fclose(outFile);

    printf( "\nBMP-Info:\n" );
    printf( "Width x Height: %i x %i\n", info.width, info.height );
    printf( "Depth: %i\n", (int)info.bitDepth );
    width=info.width;
    height=info.height;
}

void print_matrix(){
    for(int i=height-1;i>=0;i--){
        for(int j=0;j<width;j++){
                float d=((float)matrix[i][j].red)*(2.0/255.0);
            printf("(%2F , %i , %i)\n",d,matrix[i][j].green,matrix[i][j].blue);

        }
        printf("\n");
    }


}

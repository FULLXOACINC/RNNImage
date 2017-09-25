#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma pack(2)



typedef struct
{
    char signature[2];
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int offset;
} BmpHeader;

typedef struct
{
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planeCount;
    unsigned short bitDepth;
    unsigned int compression;
    unsigned int compressedImageSize;
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
    unsigned int numColors;
    unsigned int importantColors;

} BmpImageInfo;

typedef struct
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Rgb;

typedef struct
{
    int blue_neuron;
    int green_neuron;
    int red_neuron;
} Rgb_neuron;


Rgb **matrix;
unsigned int width;
unsigned int height;

void get_rgb_from_img(char* file);
void print_matrix();
void from_matrix_to_X(int block_x,int block_y);

#endif // MAIN_H_INCLUDEDs

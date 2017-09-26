
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "image.h"
#pragma pack(2)


float *X;
float *_X;
float *dX;
float *Y;
float **W;
float **_W;

int N;
int P;


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
        //printf( "------ Row %d\n", j+1 );
        read = 0;
        for(i=0; i<info.width; i++ ) {
            if( fread(pixel, 1, sizeof(Rgb), inFile) != sizeof(Rgb) ) {
                printf( "Error reading pixel!\n" );
            }
            read += sizeof(Rgb);

            matrix[j][i].blue=pixel->blue;
            matrix[j][i].red=pixel->red;
            matrix[j][i].green=pixel->green;
        }
        if( read % 4 != 0 ) {
            read = 4 - (read%4);
            //printf( "Padding: %d bytes\n", read );
            fread( pixel, read, 1, inFile );
        }
    }

    //printf( "Done.\n" );
    fclose(inFile);
    fclose(outFile);


    width=info.width;
    height=info.height;
}

void print_matrix(){
    for(int i=height-1;i>=0;i--){
        for(int j=0;j<width;j++){
            printf("(%i , %i , %i)\n",matrix[i][j].red,matrix[i][j].green,matrix[i][j].blue);
        }
        printf("\n");
    }


}

void from_matrix_to_X(int block_x,int block_y){
    float convert_value;

    int size=2;
    N=size*size*3;
    P=size*size;

    X=(float *)malloc(N*sizeof(float));
    _X=(float *)malloc(N*sizeof(float));
    dX=(float *)malloc(N*sizeof(float));
    Y=(float *)malloc(P*sizeof(float));


    int k=0;
    for(int i=0+block_y*size;i<(block_y+1)*size;i++){
        for(int j=0+block_x*size;j<(block_x+1)*size;j++){

            convert_value=((matrix[i][j].red)/255.0)*2-1;
            X[k]=convert_value;
            k++;

            convert_value=((matrix[i][j].green)/255.0)*2-1;
            X[k]=convert_value;
            k++;

            convert_value=((matrix[i][j].blue)/255.0)*2-1;
            X[k]=convert_value;
            k++;
        }
    }
    /*for(int i=0;i<3*size*size;i++){
        if(i%3==0)
            printf("\n");
        printf("%f ",X[i]);

    }*/


}

void generate_W_and__W(){
    W=(float *)malloc(N*sizeof(float));
    for(int i = 0; i < N; i++)
           W[i] = (float *)malloc(P * sizeof(float));

    _W=(float *)malloc(P*sizeof(float));
    for(int i = 0; i < P; i++)
           _W[i] = (float *)malloc(N * sizeof(float));
    srand(time(NULL));
    for(int i=0;i<N;i++)
       for(int j=0;j<P;j++)
        W[i][j]=(((float)rand()/(float)(RAND_MAX)) * 2)-1;

    for(int i=0;i<P;i++)
       for(int j=0;j<N;j++)
        _W[i][j]=(((float)rand()/(float)(RAND_MAX)) * 2)-1;



}

void countment_Y(){
    float y=0.0;
    for(int i=0;i<P;i++){
        for(int j=0;j<N;j++){
            y+=(X[j]*W[j][i]);
        }
        Y[i]=y;
        y=0.0;
    }
    /*
    printf("            W           \n");

    for(int i=0;i<N;i++){
        for(int j=0;j<P;j++){
            printf("%f  ",W[i][j]);
        }
        printf("\n");

    }

    printf("            X          \n");
    for(int i=0;i<N;i++){
       printf(" %f    \n",X[i]);

    }
        printf("            Y          \n");
    for(int i=0;i<P;i++){
       printf(" %f    \n",Y[i]);

    }*/

}

void countment__Y(){
        printf("            \n");

    float _x=0.0;
    for(int i=0;i<N;i++){
        for(int j=0;j<P;j++){
            _x+=(Y[j]*_W[j][i]);
        }
        _X[i]=_x;
        _x=0.0;
    }
    /*
    printf("            _W           \n");

    for(int i=0;i<P;i++){
        for(int j=0;j<N;j++){
            printf("%f  ",_W[i][j]);
        }
        printf("\n");

    }

    printf("            Y          \n");
    for(int i=0;i<P;i++){
       printf(" %f    \n",Y[i]);

    }
        printf("            _X          \n");
    for(int i=0;i<N;i++){
       printf(" %f    \n",_X[i]);

    }*/

}

void countment_dX(){
    for(int i=0;i<N;i++){
        dX[i]=_X[i]-X[i];
    }
    /*printf(" dX    \n");

    for(int i=0;i<N;i++){
        printf(" %f\n",dX[i]);
    }*/

}
void countment_increment_W(){

    float **Xt_dX;
    float **Xt_dX__Wt;

    for(int i=0;i<N;i++)
        for(int j=0;j<1;j++)
            X[i]=rand()%5+1;

    for(int i=0;i<N;i++)
        for(int j=0;j<1;j++)
            dX[i]=rand()%5+1;


    printf(" dX    \n");

    for(int i=0;i<N;i++){
        printf(" %f\n",dX[i]);
    }

    printf(" X    \n");

    for(int i=0;i<N;i++){
        printf(" %f\n",X[i]);
    }




    Xt_dX=(float *)malloc(N*sizeof(float));
    for(int i = 0; i < N; i++)
           Xt_dX[i] = (float *)malloc(N * sizeof(float));

    Xt_dX__Wt=(float *)malloc(N*sizeof(float));
    for(int i = 0; i < N; i++)
           Xt_dX__Wt = (float *)malloc(P * sizeof(float));

    for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                Xt_dX[i][j]=X[i]*dX[j];

    printf(" Xt_dX    \n");
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)
            printf(" %f  ",Xt_dX[i][j]);
        printf("\n");

    }



}

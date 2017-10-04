#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "image.h"
#pragma pack(2)


float **X;
float **_X;
float **dX;
float **Y;
float **W;
float **_W;
float **Xt_dX;
float **Xt_dX__Wt;
float **Yt_dX;

int N;
int P;
int size;
int block_count;

void  get_rgb_from_img(char* file)
{
    FILE *inFile;
    BmpHeader header;
    BmpImageInfo info;
    Rgb *palette;
    int i = 0;


    inFile = fopen( file, "rb" );
    if( !inFile )
    {
    }

    if( fread(&header, 1, sizeof(BmpHeader), inFile) != sizeof(BmpHeader) )
    {
        printf( "Error reading bmp header.\n" );
    }

    if( fread(&info, 1, sizeof(BmpImageInfo), inFile) != sizeof(BmpImageInfo) )
    {
        printf( "Error reading image info.\n" );
    }

    if( info.numColors > 0 )
    {
        printf( "Reading palette.\n" );
        palette = (Rgb*)malloc(sizeof(Rgb) * info.numColors);
        if( fread(palette, sizeof(Rgb), info.numColors, inFile) != (info.numColors * sizeof(Rgb)) )
        {
            printf( "Error reading palette.\n" );
        }
    }

    FILE *outFile = fopen( "hf", "wb" );
    if( !outFile )
    {
        printf( "Error opening outputfile.\n" );
    }

    Rgb *pixel = (Rgb*) malloc( sizeof(Rgb));

    matrix = (Rgb **)malloc(info.height * sizeof(Rgb *));
    for(i = 0; i < info.height; i++)
        matrix[i] = (Rgb *)malloc(info.width * sizeof(Rgb));

    int read, j;
    for(j=0; j<info.height; j++ )
    {
        read = 0;
        for(i=0; i<info.width; i++ )
        {
            if( fread(pixel, 1, sizeof(Rgb), inFile) != sizeof(Rgb) )
            {
                printf( "Error reading pixel!\n" );
            }
            read += sizeof(Rgb);

            matrix[j][i].blue=pixel->blue;
            matrix[j][i].red=pixel->red;
            matrix[j][i].green=pixel->green;
        }
        if( read % 4 != 0 )
        {
            read = 4 - (read%4);
            fread( pixel, read, 1, inFile );
        }
    }

    fclose(inFile);
    fclose(outFile);

    //size=width;
    size=2;
    width=info.width;
    height=info.height;
    block_count=(int)(height/size)*(height/size);
    //block_count=height;
    printf("%i",block_count);
}

void print_matrix()
{
    for(int i=height-1; i>=0; i--)
    {
        for(int j=0; j<width; j++)
        {
            printf("(%i , %i , %i)\n",matrix[i][j].red,matrix[i][j].green,matrix[i][j].blue);
        }
        printf("\n");
    }


}

void from_matrix_to_X()
{
    float convert_value;

    N=size*size*3;
    P=size*size*2;

    //N=width*3;
    //P=width*6;

    X=(float **)malloc(block_count*sizeof(float*));
    for(int i = 0; i < block_count; i++)
        X[i] = (float *)malloc(N * sizeof(float));
    _X=(float **)malloc(block_count*sizeof(float*));
    for(int i = 0; i < block_count; i++)
        _X[i] = (float *)malloc(N * sizeof(float));
    dX=(float **)malloc(block_count*sizeof(float*));
    for(int i = 0; i < block_count; i++)
        dX[i] = (float *)malloc(N * sizeof(float));
    Y=(float **)malloc(block_count*sizeof(float*));
    for(int i = 0; i < block_count; i++)
        Y[i] = (float *)malloc(P * sizeof(float));

    Xt_dX=(float **)malloc(N*sizeof(float*));
    for(int i = 0; i < N; i++)
        Xt_dX[i] = (float *)malloc(N * sizeof(float));

    Xt_dX__Wt=(float **)malloc(N*sizeof(float*));
    for(int i = 0; i < N; i++)
        Xt_dX__Wt[i] = (float *)malloc(P * sizeof(float));

    Yt_dX=(float **)malloc(P*sizeof(float*));
    for(int i = 0; i < P; i++)
        Yt_dX[i] = (float *)malloc(N * sizeof(float));

    for(int i=0;i<block_count;i++)
        memset(_X[i],0,N * sizeof(float));
    for(int i=0;i<block_count;i++)
        memset(Y[i],0,P * sizeof(float));
    for(int i=0;i<block_count;i++)
        memset(dX[i],0,N * sizeof(float));

    int block_y=0;
    int block_x=0;
    int k;

    for(int z=0; z<block_count; z++)
    {
        k=0;
        for(int i=0+block_y*size; i<(block_y+1)*size; i++)
        {
            for(int j=0+block_x*size; j<(block_x+1)*size; j++)
            {

                convert_value=((matrix[i][j].red)/255.0)*2-1;
                X[z][k]=convert_value;
                k++;

                convert_value=((matrix[i][j].green)/255.0)*2-1;
                X[z][k]=convert_value;
                k++;

                convert_value=((matrix[i][j].blue)/255.0)*2-1;
                X[z][k]=convert_value;
                k++;
            }
        }
        //printf("\n%i %i %i",block_y,block_x,(int)(height/size));

        block_x++;

        if(block_x>=(int)(height/size))
        {
            block_x=0;
            //block_y++;
        }

    }
/*
    for(int z=0; z<block_count; z++)
    {
        k=0;
            for(int j=0; j<width; j++)
            {

                convert_value=((matrix[z][j].red)/255.0)*2-1;
                X[z][k]=convert_value;
                k++;

                convert_value=((matrix[z][j].green)/255.0)*2-1;
                X[z][k]=convert_value;
                k++;

                convert_value=((matrix[z][j].blue)/255.0)*2-1;
                X[z][k]=convert_value;
                k++;
            }

        //printf("\n%i %i %i",block_y,block_x,(int)(height/size));



    }*/






}

void generate_W_and__W()
{
    W=(float **)malloc(N*sizeof(float*));
    for(int i = 0; i < N; i++)
        W[i] = (float *)malloc(P * sizeof(float));

    _W=(float **)malloc(P*sizeof(float*));
    for(int i = 0; i < P; i++)
        _W[i] = (float *)malloc(N * sizeof(float));
    srand(time(NULL));
    for(int i=0; i<N; i++)
        for(int j=0; j<P; j++)
            W[i][j]=((((float)rand()/(float)(RAND_MAX)) * 2)-1);

    for(int i=0; i<P; i++)
        for(int j=0; j<N; j++)
            _W[i][j]=W[j][i];


}

void countment_Y(int index)
{
    for(int i=0; i<P; i++)
    {
        Y[index][i]=0;
        for(int j=0; j<N; j++)
        {

            //printf("\n%f %f %f",X[index][j]*W[j][i],X[index][j],W[j][i]);

            Y[index][i]+=(float)(X[index][j]*W[j][i]);

        }



    }



}


void countment__X(int index)
{
    //  printf("            \n");

    for(int i=0; i<N; i++)
    {
        _X[index][i]=0;
        for(int j=0; j<P; j++)
        {
            //printf("\n%f %f %f",Y[index][j]*_W[j][i],Y[index][j],_W[j][i]);
            _X[index][i]+=(Y[index][j]*_W[j][i]);
        }
        //printf("\n %f",_X[index][i]);
        //getchar();
    }

}

void countment_dX(int index)
{
    for(int i=0; i<N; i++)
    {
        dX[index][i]=0;
        dX[index][i]=_X[index][i]-X[index][i];
    }
    //printf(" dX   _X     X \n");

    //printf(" %f %f %f %f\n",dX[index][0],_X[index][0],X[index][0],Y[index][0]);


}
void countment_increment_W(int index)
{


    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++){
            Xt_dX[i][j]=0;
            Xt_dX[i][j]=X[index][i]*dX[index][j];
        }



    for(int i=0; i<N; i++)
    {
        for(int j=0; j<P; j++)
        {
            for(int k=0; k<N; k++)
            {
                Xt_dX__Wt[i][j]=0;
                Xt_dX__Wt[i][j]+=Xt_dX[i][k]*_W[j][k];
            }

        }
    }

    float A=0.0;
    for(int i=0;i<N;i++)
        A+=(X[index][i]*X[index][i]);

    A=(1.0/(5.0*A));
    //A=0.01;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<P; j++)
            W[i][j]=W[i][j]-(A*Xt_dX__Wt[i][j]);

    }

}

void countment_increment__W(int index)
{
    for(int i=0; i<P; i++)
        for(int j=0; j<N; j++)
            Yt_dX[i][j]=Y[index][i]*dX[index][j];

    float A=0.0;
    for(int i=0;i<P;i++)
        A+=(Y[index][i]*Y[index][i]);

    A=(1.0/(5.0*A));
    //A=0.01;
    for(int i=0; i<P; i++)
    {
        for(int j=0; j<N; j++)
            _W[i][j]=_W[i][j]-(A*Yt_dX[i][j]);

    }




}

int function_E(const float e)
{

    float E=0.0;
    for(int index=0; index<block_count; index++)
    {
        for(int i=0; i<N; i++)
            E+=(float)(dX[index][i]*dX[index][i]);
    }


    printf("Error =%f\n",E);
    if(E<e)
        return 0;

    return 1;
}


void start_lern()
{
    float e=20;
    generate_W_and__W();

    int k=0;
    do
    {
        k++;

        for(int i=0; i<block_count; i++)
        {

            countment_Y(i);
            countment__X(i);
            countment_dX(i);
            countment_increment_W(i);
            countment_increment__W(i);
        }

    }while(function_E(e));

    getchar();
    for(int index=0; index<block_count; index++)
        for(int i=0; i<N; i++)
            printf("%f    \n",(X[index][i]-_X[index][i]));

            //printf("%f   %f \n",((X[index][i]+1)/2.0)*255.0,((_X[index][i]+1)/2.0)*255.0);
//    convert_matrix_rgb(x,y);
}
/*
void convert_matrix_rgb(int x,int y){
    int k=0;
    float convert_value;
    for(int i=0+y*size;i<(x+1)*size;i++){
        for(int j=0+x*size;j<(x+1)*size;j++){

            convert_value=((_X[k]+1)/2.0)*255.0;
            matrix[i][j].red=convert_value;
            k++;

            convert_value=((_X[k]+1)/2.0)*255.0;
            matrix[i][j].green=convert_value;
            k++;

            convert_value=((_X[k]+1)/2.0)*255.0;
            matrix[i][j].blue=convert_value;
            k++;
        }
    }

}

*/







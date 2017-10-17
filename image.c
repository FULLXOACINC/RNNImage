#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"
#pragma pack(2)


float **X;
float **_X;
float **dX;
float **Y;
float **W;
float **_W;
float **Xt_dX;
float *dX__Wt;
float **Xt_dX__Wt;
float **Yt_dX;

int N;
int P;
int size;
int block_count;
int x_size,y_size;

void  get_rgb_from_img(char* file,int size_x,int size_y)
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

    x_size=size_x;
    y_size=size_y;
    width=info.width;
    height=info.height;
    block_count=(int)(height/y_size)*(width/x_size);
    printf("L: %i\n",block_count);
}


void from_matrix_to_X(int P_size)
{
    float convert_value;

    N=y_size*x_size*3;
    P=P_size;

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

    dX__Wt=(float *)malloc(P*sizeof(float));
    memset(dX__Wt,0,P * sizeof(float));

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
        for(int i=0+block_y*y_size; i<(block_y+1)*y_size; i++)
        {
            for(int j=0+block_x*x_size; j<(block_x+1)*x_size; j++)
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

        block_x++;

        if(block_x>=(int)(width/x_size))
        {
            block_x=0;
            block_y++;
        }

    }



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

void countment_Y(const int  index)
{
    for(int i=0; i<P; i++)
    {
        Y[index][i]=0;
        for(int j=0; j<N; j++)
        {
            Y[index][i]+=(X[index][j]*W[j][i]);

        }



    }



}


void countment__X(const int  index)
{
    for(int i=0; i<N; i++)
    {
        _X[index][i]=0;
        for(int j=0; j<P; j++)
        {
            _X[index][i]+=(Y[index][j]*_W[j][i]);
        }

    }

}

void countment_dX(const int index)
{
    for(int i=0; i<N; i++)
    {
        dX[index][i]=_X[index][i]-X[index][i];
    }



}
void countment_increment_W(const int  index)
{
    float A=N*N;
    for(int i=0;i<N;i++)
        A+=(X[index][i]*X[index][i]);

    A=(1.0/(1.0*A));


    for(int i=0; i<P; i++){
        dX__Wt[i]=0.0;
        for(int j=0; j<N; j++){
            dX__Wt[i]+=dX[index][j]*_W[i][j];
        }
    }

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<P; j++)
            W[i][j]-=(A*X[index][i]*dX__Wt[j]);

    }

}

void countment_increment__W(const int  index)
{
    float A=N*N;
    for(int i=0;i<P;i++)
        A+=(Y[index][i]*Y[index][i]);

    A=(1.0/(1.0*A));

    for(int i=0; i<P; i++)
    {
        for(int j=0; j<N; j++)
            _W[i][j]-=(A*Y[index][i]*dX[index][j]);

    }


}

int function_E(double e)
{

    float E=0.0;
    for(int index=0; index<block_count; index++)
    {
        for(int i=0; i<N; i++)
            E+=(dX[index][i]*dX[index][i]);
    }


    printf("Error = %f\n",E);
    if(E<e)
        return 0;

    return 1;
}


void start_lern(double e)
{
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
            countment_increment__W(i);
            countment_increment_W(i);

        }

    }while(function_E(e));
    printf("count iteration :%i\n",k);
    from__X_to_matrix();
}

void from__X_to_matrix(){
    int block_y=0;
    int block_x=0;
    int k;
    float convert_value;


    for(int z=0; z<block_count; z++)
    {
        k=0;
        for(int i=0+block_y*y_size; i<(block_y+1)*y_size; i++)
        {
            for(int j=0+block_x*x_size; j<(block_x+1)*x_size; j++)
            {
                convert_value=(((_X[z][k]+1)/2.0)*255.0);
                matrix[i][j].red=(int)convert_value;
                k++;

                convert_value=(((_X[z][k]+1)/2.0)*255.0);
                matrix[i][j].green=(int)convert_value;
                k++;

                convert_value=(((_X[z][k]+1)/2.0)*255.0);
                matrix[i][j].blue=(int)convert_value;
                k++;
            }
        }

        block_x++;

        if(block_x>=(int)(width/x_size))
        {
            block_x=0;
            block_y++;
        }

    }



}

void create_bmp()
{


BITMAPFILEHEADER bfh;
BITMAPINFOHEADER bih;

unsigned short bfType=0x4d42;
bfh.bfReserved1 = 0;
bfh.bfReserved2 = 0;
bfh.bfSize = 2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+640*480*3;
bfh.bfOffBits = 0x36;

bih.biSize = sizeof(BITMAPINFOHEADER);
bih.biWidth = width;
bih.biHeight = height;
bih.biPlanes = 1;
bih.biBitCount = 24;
bih.biCompression = 0;
bih.biSizeImage = 0;
bih.biXPelsPerMeter = 5000;
bih.biYPelsPerMeter = 5000;
bih.biClrUsed = 0;
bih.biClrImportant = 0;

FILE *file = fopen("res.bmp", "wb");
if (!file)
    {
    printf("Could not write file\n");
    return;
    }


fwrite(&bfType,1,sizeof(bfType),file);
fwrite(&bfh, 1, sizeof(bfh), file);
fwrite(&bih, 1, sizeof(bih), file);


for (int y = 0; y<bih.biHeight; y++)
    {
    for (int x = 0; x < bih.biWidth; x++)
        {

        unsigned char r = matrix[y][x].red;
        unsigned char g = matrix[y][x].green;
        unsigned char b = matrix[y][x].blue;
        fwrite(&b, 1, 1, file);
        fwrite(&g, 1, 1, file);
        fwrite(&r, 1, 1, file);
        }
    }
fclose(file);
}
void print_res(){
    printf("W:\n");
    for(int i=0;i<N;i++){
        for(int j=0;j<P;j++)
            printf("%f ",W[i][j]);
        printf("\n");

    }
    printf("W':\n");
    for(int i=0;i<P;i++){
        for(int j=0;j<N;j++)
            printf("%f ",_W[i][j]);
        printf("\n");

    }

    float f1=(N*block_count);
    float f2=(N+block_count)*P+2.0;
    printf("Z = %lf",(f1/f2));

}









#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#pragma pack(2)



int main( )
{

    int size_x,size_y,P_size;
    double e;
    printf("enter block size:\n");
    scanf("%i",&size_x);
    scanf("%i",&size_y);
    printf("enter P :\n");
    scanf("%i",&P_size);
    printf("enter max error:\n");
    scanf("%lf",&e);


    get_rgb_from_img("test4.bmp",size_x,size_y);
    from_matrix_to_X(P_size);
    start_lern(e);
    create_bmp();
    print_res();


    return 0;

}

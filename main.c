#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#pragma pack(2)



int main( )
{

    int size_x,size_y;
    scanf("%i",&size_x);
    scanf("%i",&size_y);
    get_rgb_from_img("test4.bmp",size_x,size_y);
    from_matrix_to_X();
    start_lern();
    create_bmp();


    return 0;

}

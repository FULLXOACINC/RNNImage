#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#pragma pack(2)



int main( ) {


    get_rgb_from_img("test.bmp");


    from_matrix_to_X(0,0);
    generate_W_and__W();
    countment_Y();
    countment__Y();
    countment_dX();

countment_increment_W();

    //print_matrix();
    return 0;

}

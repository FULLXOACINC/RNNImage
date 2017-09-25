#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#pragma pack(2)



int main( ) {


    get_rgb_from_img("test.bmp");


    from_matrix_to_X(0,0);

    print_matrix();
    return 0;

}

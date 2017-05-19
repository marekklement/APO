#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "julia_set.h"
uint16_t transfer (unsigned char red, unsigned char green, unsigned char blue) {
    return (uint16_t)(((uint16_t)(red / 255 * 31)) << 11 | ((uint16_t)(green / 255 * 63) << 5) | ((uint16_t)(blue * 31 / 255)));
}

unsigned char * julia_make (double movex, double movey, double zoom){

	const double c_real = -0.8;
	const double c_imag = 0.156;
	const int num_of_iter = 600;
    int img_wid = 320;
    int img_len = 480;
    uint16_t col;
    int x, y;

	unsigned char *colors= (unsigned char*)malloc(3*sizeof(unsigned char));
    uint16_t *image = (uint16_t*)malloc(img_len*img_wid* sizeof(uint16_t));

    for (y = 0; y < img_wid; ++y) {
        for (x = 0; x < img_len; ++x) {
            double z_real = 0, z_imag = 0, z_real_s = 0, z_imag_s = 0;
            //double zoom = 1;
            z_real = 1.5 * (x - img_len / 2) / (0.5 * img_len * zoom) + movex;
            z_imag = (y - img_wid / 2) / (0.5 * img_wid * zoom) + movey;
            double i = 0;
            for (i = 0; i < num_of_iter && (z_real * z_real) + (z_imag * z_imag) < 4; ++i) {
                z_real_s = z_real;
                z_imag_s = z_imag;
                z_real = (z_real_s * z_real_s) - (z_imag_s * z_imag_s) + (c_real);
                z_imag = (2 * z_real_s * z_imag_s) + c_imag;
            }

            i = i / num_of_iter;

            colors[0] = 9 * (1 - i) * i * i * i * 255;
            colors[1] = 15 * (1 - i) * (1 - i) * i * i * 255;
            colors[2] = 8.5 * (1 - i) * (1 - i) * (1 - i) * i * 255;

            col = transfer(colors[1],colors[2],colors[3]);
            image[x*y+x]=col;
        }
    }

	return image;
}

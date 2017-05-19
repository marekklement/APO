#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "julia_set.h"

unsigned char * julia_make (int img_height, int img_width,int x,int y,double movex, double movey,double zoom){

	const double c_real = -0.8;
	const double c_imag = 0.156;
	const int num_of_iter = 600;

	unsigned char *colors= (unsigned char*)malloc(3*sizeof(unsigned char));

	double z_real=0,z_imag=0,z_real_s=0,z_imag_s=0;
	//double zoom = 1;
	z_real=1.5*(x-img_width/2)/(0.5*img_width*zoom)+movex;
	z_imag=(y-img_height/2)/(0.5*img_height*zoom)+movey;
	double i = 0;
	for(i=0;i<num_of_iter && (z_real*z_real)+(z_imag*z_imag)<4;++i){
		z_real_s= z_real;
		z_imag_s = z_imag;
		z_real =(z_real_s*z_real_s)-(z_imag_s*z_imag_s)+(c_real);
		z_imag = (2*z_real_s*z_imag_s)+c_imag;
	}

	i = i / num_of_iter;

	colors[0]=9*(1-i)*i*i*i*255;
	colors[1]=15*(1-i)*(1-i)*i*i*255;
	colors[2]=8.5*(1-i)*(1-i)*(1-i)*i*255;

	return colors;
}

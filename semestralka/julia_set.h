#ifndef JULIA_SET_H
#define JULIA_SET_H

#include <stdio.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned char *julia_make (int img_height, int img_width,int x,int y, double movex, double movey,double zoom);

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif  /*JULIA_SET_H*/
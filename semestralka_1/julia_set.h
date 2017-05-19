#ifndef JULIA_SET_H
#define JULIA_SET_H

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t transfer (unsigned char red, unsigned char green, unsigned char blue);

unsigned char * julia_make (double movex, double movey, double zoom);

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif  /*JULIA_SET_H*/
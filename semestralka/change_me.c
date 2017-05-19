/*******************************************************************
  Simple program to check LCD functionality on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_lcdtest.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include "julia_set.h"

uint16_t transfer (unsigned char red, unsigned char green, unsigned char blue) {
    return (uint16_t)(((uint16_t)(red / 255 * 31)) << 11 | ((uint16_t)(green / 255 * 63) << 5) | ((uint16_t)(blue * 31 / 255)));
}

int main(int argc, char *argv[])
{
	printf("Hello world\n");
	int x,y;
	uint16_t col;

	unsigned char *mem_base;
  	mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  	if (mem_base == NULL) exit(1);

	 //Initialize display
	 unsigned char *parlcd_mem_base;
	 parlcd_mem_base = (unsigned char *) map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	 if (parlcd_mem_base == NULL) exit(1);

	 parlcd_hx8357_init(parlcd_mem_base);

	 parlcd_write_cmd(parlcd_mem_base, 0x2c);

	unsigned char *colors;
	int img_wid = 320;
	int img_len = 480;

    double wid_jump = 0.00625;
    double len_jump = 0.004167;

	for (y = 0; y < img_wid; ++y)
	{
		for (x = 0; x < img_len; ++x)
		{
			colors = julia_make(img_wid,img_len,x,y,0,0,1);
			col = transfer(colors[0],colors[1],colors[2]);
            parlcd_write_data(parlcd_mem_base, col);
		}
	}
	double movex;
	double movey;
	double zoom=1;
	while(1){
		movex = 0;
		movey = 0;
		//zoom = 1;
		uint32_t rgb_knobs_value;
		rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		//printf("%d\n",rgb_knobs_value);
		uint8_t red_knob = (rgb_knobs_value & 0x00FF0000) >> 16;
		uint8_t green_knob = (rgb_knobs_value & 0x0000FF00) >> 8;
		uint8_t blue_knob = (rgb_knobs_value & 0x000000FF);
		printf("red: %d green: %d blue: %d\n",red_knob,green_knob,blue_knob);
		zoom+=blue_knob/50;
		movex = wid_jump*red_knob;
		movey = len_jump*green_knob;
		zoom = 1+(blue_knob/4);
        for (y = 0; y < img_wid; ++y)
		{
			for (x = 0; x < img_len; ++x)
			{
				colors = julia_make(img_wid,img_len,x,y,movex,movey,zoom);
				col = transfer(colors[0],colors[1],colors[2]);
				parlcd_write_data(parlcd_mem_base, col);
			}
	}
	}

	sleep(4);

	printf("Goodbye world\n");

	return 0;
}

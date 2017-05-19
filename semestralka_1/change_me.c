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
#include <pthread.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include "julia_set.h"

//set img height and width
int img_wid = 320;
int img_len = 480;

void *print_array_to_desk (unsigned char *parlcd_mem_base, double movex,double movey,double zoom){
    //pointer to array for colors
    uint16_t *colors;

    //len of array
    int total = img_len*img_wid;

    //compute julia set and show values on lcd
    colors = julia_make(movex,movey,zoom);
    for(int i=0;i<total;++i){
        parlcd_write_data(parlcd_mem_base, colors[i]);
    }
}

int main(int argc, char *argv[])
{
    pthread_t julia_compute_thr;

    printf("App just started!\n");

    //Initialize RGB knobs
    unsigned char *mem_base;
    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (mem_base == NULL) exit(1);

     //Initialize display
    unsigned char *parlcd_mem_base;
    parlcd_mem_base = (unsigned char *) map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (parlcd_mem_base == NULL) exit(1);

    //lcd-init
    parlcd_hx8357_init(parlcd_mem_base);

    //lcd-command
    parlcd_write_cmd(parlcd_mem_base, 0x2c);

    //pointer to array for colors
    uint16_t *colors;

    //set coeficient of movement x/y
    double wid_jump = 0.00625;
    double len_jump = 0.004167;

    //len of array
    int total = img_len*img_wid;

    //compute julia set and show values on lcd
//    pthread_create(&julia_compute_thr,NULL,print_array_to_desk(parlcd_mem_base,0,0,1),NULL);
    print_array_to_desk(parlcd_mem_base,0,0,1);
//    colors = julia_make(0,0,1);
//    for(int i=0;i<total;++i){
//        parlcd_write_data(parlcd_mem_base, colors[i]);
//    }

    //set the current movement of the picture
    double current_movex = 0;
    double current_movey = 0;

    //set movement of last checking of the knobs, to move over 256
    double last_movex = 0;
    double last_movey = 0;

    //set the current zoom of the picture
    double zoom=1;

    //here begins the infinite loop for changing the picture
    while(1){
        //set and get knobs value
        uint32_t rgb_knobs_value;
        rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);

        //get value of each knob alone R-G-B
        uint8_t red_knob = (rgb_knobs_value & 0x00FF0000) >> 16;
        uint8_t green_knob = (rgb_knobs_value & 0x0000FF00) >> 8;
        uint8_t blue_knob = (rgb_knobs_value & 0x000000FF);

        //to make sure, if it is right, print values of each knob
        printf("%d %d %d",red_knob,green_knob,blue_knob);

        //get movement from RGB knobs
        current_movex+=(last_movex+red_knob)*wid_jump;
        current_movey+=(last_movey+green_knob)*len_jump;

        //set last movement
        last_movex=current_movex;
        last_movey=current_movey;

        //get zoom from blue knob
        zoom = 1+(blue_knob/4);

        //here we are making julia set with current movements and zoom
//        pthread_create(&julia_compute_thr,NULL,print_array_to_desk(parlcd_mem_base,current_movex,current_movey,zoom),NULL);
        print_array_to_desk(parlcd_mem_base,current_movex,current_movey,zoom);
//        colors = julia_make(current_movex,current_movey,zoom);
//        //to be smooth, sending it alone
//        for(int i=0;i<total;++i){
//            parlcd_write_data(parlcd_mem_base, colors[i]);
//        }
    }

    sleep(4);

    printf("We shold not be at the end of this app!\n");

    return 0;
}

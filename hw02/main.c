#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//colors values
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} picture_color_number;

//defining number of rows, columns and highest possible color
int rows;
int cols;
int number_defining_color;

//this function take path given and turn it into array of colors, also write number of colums and rows
picture_color_number *ImageRead(char *filename){
	FILE *file;
	file = fopen(filename,"rb");
	char buf[10];
	//scaning for 9 characters long string--- to remove header comments and P6 comment
	fscanf(file,"%9s",buf);
	//reading number of columns and rows
	fscanf(file,"%d %d\n",&cols,&rows);
	//reading maximum possible color number
	fscanf(file,"%d\n",&number_defining_color);
	//alocate for new image ---for three colors and rows*cols pixels
	picture_color_number *image = (picture_color_number*)malloc(rows*cols*sizeof(picture_color_number));
	//reading array to alocated image
	fread(image,cols*3,rows,file);
	fclose(file);
	return image;
}
//this function makes new histogram with interval values
void HistogramWrite (const char *filename,picture_color_number *img){
	int number,first=0,second=0,third=0,fourt=0,fifth=0;
	FILE *file;
	file = fopen(filename, "wb");
    // counting histogram
    for(int i = 0;i<cols*rows;++i){
    	number = round((0.2126*img[i].red)+(0.7152*img[i].green)+(0.0722*img[i].blue));
    	//making five diferent intervals
    	if(number<=50){
    		++first;
    	}else if(number<=101){
    		++second;
    	}else if(number<=152){
    		++third;
    	}else if(number<=203){
    		++fourt;
    	}else if(number<=255){
    		++fifth;
    	}
    	
    }
	fprintf(file, "%d %d %d %d %d", first,second,third,fourt,fifth);
	fclose(file);
}
//this function write new, already Zoomed image
void ImageWrite(const char *filename, picture_color_number *img)
{
    FILE *file;
    file = fopen(filename, "wb");
    //printing the header
    fprintf(file, "P6\n");
    fprintf(file, "%d %d\n",cols,rows);
    fprintf(file, "%d\n",number_defining_color);
    //writing values of new picture
    fwrite(img,cols*3,rows,file);
    fclose(file);
}
//this function is the main function of the program, like brain...it make the picture sharper
void GetZoomedImage(picture_color_number *img,picture_color_number *img2)
{
    int smaller,bigger,waysmaller,waybigger,size,i,redc,greenc,bluec;
    if(img){
    	size = rows*cols;
        for(i=0;i<size;++i){
        	//this just copy borders
        	waysmaller = i - cols;
        	smaller = i-1;
        	bigger = i+1;
        	waybigger = i + cols;
            if((i<cols) || ((i)%cols ==0) || i>(size)-1-cols || (bigger)%cols==0){
            	//red
            	img2[i].red=img[i].red;
            	//green
            	img2[i].green=img[i].green;
            	//blue
            	img2[i].blue=img[i].blue;
            }else{
	            redc = (5*img[i].red)-(img[bigger].red)-(img[smaller].red)-(img[waybigger].red)-(img[waysmaller].red);
	            greenc = (5*img[i].green)-(img[bigger].green)-(img[smaller].green)-(img[waybigger].green)-(img[waysmaller].green);
	            bluec = (5*img[i].blue)-(img[bigger].blue)-(img[smaller].blue)-(img[waybigger].blue)-(img[waysmaller].blue);
	            //red
	            //count color number and if it is more than 255, it just write 255
	            if(redc>255){
	            	img2[i].red=255;
            	//if it is less than 0, it writes just 0
	            }else if (redc<=0){
	            	img2[i].red=0;
	            }else{
	            	img2[i].red=redc;
	            }
	            //green
	            if(greenc>255){
	            	img2[i].green=255;
	            }else if (greenc<=0){
	            	img2[i].green=0;
	            }else{
	            	img2[i].green=greenc;
	            }
	            //blue
	            if(bluec>255){
	            	img2[i].blue=255;
	            }else if (bluec<=0){
	            	img2[i].blue=0;
	            }else{
	            	img2[i].blue=bluec;
	            }
        	}
        }
    }
}

//main caller of program
int main(int argc, char **argv)
{
	//clock for timing
	/*struct timespec start, stop;
	clock_gettime( CLOCK_REALTIME, &start);*/

	picture_color_number *image;
	picture_color_number *image2;
	
	//calling function for reading image
	image = ImageRead(argv[1]);
	//allocate memory for another color
	image2 = (picture_color_number*)malloc(rows*cols*sizeof(picture_color_number));
	//calling function for sherpening picture
	GetZoomedImage(image,image2);
	//calling function for writing new image into ppm file (/output.ppm)
    ImageWrite("output.ppm",image2);
    //calling function for writing histogram into output.txt
    HistogramWrite("output.txt",image2);
    //free for valgrind
    free(image);
    free(image2);

    //calling and printing clock
    /*clock_gettime( CLOCK_REALTIME, &stop);
	double accum = ( stop.tv_sec - start.tv_sec ) * 1000.0 + ( stop.tv_nsec - start.tv_nsec ) / 1000000.0;
	printf( "Time: %.6lf ms\n", accum );*/

	return 0;
}
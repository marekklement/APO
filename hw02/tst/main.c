#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
    unsigned char red,green,blue;
} PPMPixel;

typedef struct {
    int x, y;
    PPMPixel *data;
} PPMImage;

#define RGB_COMPONENT_COLOR 255

static PPMImage *readPPM(const char *filename){
	//setting the image
	char buff[16];
	int rgb_comp_col,c;
	PPMImage *img;
	FILE *fp;
	//openning file
	fp = fopen(filename,"rb");
	if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }
    /*if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }*/
	//checking for error while openning
	/*if(!fp){
		fprintf(stderr,"Unable to print %s\n",filename);
		exit(1);
	}*/
	//allocate the memory for image
	img = (PPMImage*)malloc(sizeof(PPMImage));
	//check for error while alocating
	/*if(!img){
		fprintf(stderr,"Unable alocate\n");
		exit(1);
	}*/
	//check for comments
    c = getc(fp);
    while (c == '#') {
    	while (getc(fp) != '\n') ;
     	c = getc(fp);
    }

    ungetc(c, fp);
	//scan for size of the image and checking for error while scaning
	if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
        fprintf(stderr, "unable to get image size of %s\n", filename);
        exit(1);
    }
    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_col) != 1) {
        fprintf(stderr, "wrong component color %s\n", filename);
        exit(1);
    }
    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));

    /*if (!img) {
        fprintf(stderr,"Unable alocate\n");
		exit(1);
	}*/
    //read pixel data from file
    if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
        fprintf(stderr, "Error loading image %s\n", filename);
        exit(1);
    }

    fclose(fp);
    return img;

}
void writeHistogram (const char *filename,PPMImage *img){
	int number,first=0,second=0,third=0,fourt=0,fifth=0;
	FILE *fp;
	fp = fopen(filename, "wb");
	if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }
    for(int i = 0;i<img->x*img->y;++i){
    	number = round((0.2126*img->data[i].red)+(0.7152*img->data[i].green)+(0.0722*img->data[i].blue));
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
	fprintf(fp, "%d %d %d %d %d", first,second,third,fourt,fifth);
	fclose(fp);
}
void writePPM(const char *filename, PPMImage *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
}
void zoom(PPMImage *img,PPMImage *img2)
{
    if(img){

        for(int i=0;i<img->x*img->y;++i){
            if((i<img->x) || ((i)%img->x ==0) || i>((img->x)*(img->y))-1-img->x || (i+1)%img->x==0){
            	img2->data[i].red=img->data[i].red;
            	img2->data[i].green=img->data[i].green;
            	img2->data[i].blue=img->data[i].blue;
            }else{
	            //red
	            if(((5*img->data[i].red)-(img->data[i+1].red)-(img->data[i-1].red)-(img->data[i+img->x].red)-(img->data[i-img->x].red))>255){
	            	img2->data[i].red=255;
	            }else if ((5*img->data[i].red)-(img->data[i+1].red)-(img->data[i-1].red)-(img->data[i+img->x].red)-(img->data[i-img->x].red)<=0){
	            	img2->data[i].red=0;
	            }else{
	            	img2->data[i].red=((5*img->data[i].red)-(img->data[i+1].red)-(img->data[i-1].red)-(img->data[i+img->x].red)-(img->data[i-img->x].red));
	            }
	            //green
	            if(((5*img->data[i].green)-(img->data[i+1].green)-(img->data[i-1].green)-(img->data[i+img->x].green)-(img->data[i-img->x].green))>255){
	            	img2->data[i].green=255;
	            }else if ((5*img->data[i].green)-((img->data[i+1].green)+(img->data[i-1].green)+(img->data[i+img->x].green)+(img->data[i-img->x].green))<=0){
	            	img2->data[i].green=0;
	            }else{
	            	img2->data[i].green=((5*img->data[i].green)-(img->data[i+1].green)-(img->data[i-1].green)-(img->data[i+img->x].green)-(img->data[i-img->x].green));
	            }
	            //blue
	            if(((5*img->data[i].blue)-(img->data[i+1].blue)-(img->data[i-1].blue)-(img->data[i+img->x].blue)-(img->data[i-img->x].blue))>255){
	            	img2->data[i].blue=255;
	            }else if ((5*img->data[i].blue)-((img->data[i+1].blue)+(img->data[i-1].blue)+(img->data[i+img->x].blue)+(img->data[i-img->x].blue))<=0){
	            	img2->data[i].blue=0;
	            }else{
	            	img2->data[i].blue=((5*img->data[i].blue)-(img->data[i+1].blue)-(img->data[i-1].blue)-(img->data[i+img->x].blue)-(img->data[i-img->x].blue));
	            }
	            //img2->data[i].red=(5*img->data[i].red)-(img->data[i+1].red)-(img->data[i-1].red)-(img->data[i+img->x].red)-(img->data[i-img->x].red);
	            //img2->data[i].green=(5*img->data[i].green)-((img->data[i+1].green)+(img->data[i-1].green)+(img->data[i+img->x].green)+(img->data[i-img->x].green));
	            //img2->data[i].blue=(5*img->data[i].blue)-(img->data[i+1].blue)-(img->data[i-1].blue)-(img->data[i+img->x].blue)-(img->data[i-img->x].blue);
        	}
        }
    }
}


int main(int argc, char **argv)
{
	PPMImage *image;
	PPMImage *image2;
	

	image = readPPM(argv[1]);
	image2 = (PPMImage*)malloc(sizeof(PPMImage));
	image2->x = image->x;
	image2->y = image->y;
	image2->data = (PPMPixel*)malloc(image2->x * image2->y * sizeof(PPMPixel));
	

	zoom(image,image2);
    writePPM("output.ppm",image2);
    writeHistogram("output.txt",image2);

	free(image->data);
	free(image2->data);

    free(image);
    free(image2);

	return 0;
}
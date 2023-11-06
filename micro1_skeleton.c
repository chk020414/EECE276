#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"


#include <math.h>
#include <stdio.h>
#include <float.h>


void mirror_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void grayScale_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void sobelFiltering_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);

int main()
{
 
	int height;
	int width;
	int channel;

	char command;
	/*
 	printf("Take a picture? (y/n)\n");
 	scanf("%c", &command);

 	if(command == 'n')
 		exit(1);

	
 	printf("Cheeze !\r\n");
	system("raspistill -w 640 -h 480 -t 10 -o image.bmp");
 	*/
 	
 	unsigned char* imgIn = stbi_load("test_image.bmp", &width, &height, &channel, 3);


	unsigned char* imgOut_mirror = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
	unsigned char* imgOut_grayScale = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
	unsigned char* imgOut_sobelFiltering = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
 	
	mirror_transform(imgIn, height, width, channel, imgOut_mirror);
	grayScale_transform(imgIn, height, width, channel, imgOut_grayScale);
	sobelFiltering_transform(imgOut_grayScale, height, width, channel, imgOut_sobelFiltering);


	stbi_write_bmp("image_mirror.bmp", width, height, channel, imgOut_mirror);
	stbi_write_bmp("image_grayScale.bmp", width, height, channel, imgOut_grayScale);
	stbi_write_bmp("image_sobelFiltering.bmp", width, height, channel, imgOut_sobelFiltering);
	
	stbi_image_free(imgIn);
	free(imgOut_mirror);
	free(imgOut_grayScale);
	free(imgOut_sobelFiltering);
 

	return 0;
}

void mirror_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	
	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			for (int z = 0; z < channel; z++)
			{
				out[x * width * channel + y * channel + z] = in[x * width * channel + (width - y - 1) * channel + z];
			}
		}
	}
}

void grayScale_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	
	int sum = 0;

	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			for (int z = 0; z < channel; z++)
			{
				sum += in[x * width * channel + y * channel + z]/3;
			}

			for (int w = 0; w < channel; w++)
			{
				out[x * width * channel + y * channel + w] = sum;
			}

			sum = 0;
		}
	}
}


void sobelFiltering_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	int xfilter = 0;
	int yfilter = 0;
	for (int x = 1; x<height-1; x++){
		for (int y = 1; y < width-1; y++) {
			for (int z = 0; z < channel; z++) {
				xfilter = (-1) * in[(x - 1) * width * channel + (y + 1) * channel + z] + (-2) * in[(x)*width * channel + (y + 1) * channel + z] + (-1) * in[(x + 1) * width * channel + (y + 1) * channel + z] + (1) * in[(x - 1) * width * channel + (y - 1) * channel + z] + (2) * in[(x - 1) * width * channel + (y - 1) * channel + z] + (1) * in[(x - 1) * width * channel + (y - 1) * channel + z];
				yfilter = (1) * in[(x - 1) * width * channel + (y + 1) * channel + z] + (2) * in[(x - 1) * width * channel + (y)*channel + z] + (1) * in[(x - 1) * width * channel + (y - 1) * channel + z] + (-1) * in[(x + 1) * width * channel + (y + 1) * channel + z] + (-2) * in[(x + 1) * width * channel + (y)*channel + z] + (-1) * in[(x + 1) * width * channel + (y - 1) * channel + z];
				out[x * width * channel + y * channel + z] = out[x * width * channel + y * channel + z] = abs(xfilter) + abs(yfilter);
			}
			xfilter = yfilter = 0;
		}
	}

}

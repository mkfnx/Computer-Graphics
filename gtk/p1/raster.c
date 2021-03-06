/**
 \filename raster.c 

  author: Rafael Norman Saucedo Delgado
*/

#include "raster.h"

int dda_drawline(unsigned char *raster, int r, int c, int xi, int yi, int xf, int yf)
{
    double dx = xf-xi;
    double dy = yf-yi;

    double m = dy/dx;
    double b = yi-m*xi;
    
    int xn, yn;
    double x1 = xi;
    double y1 = yi;
    
    unsigned char black_pixel[4] = {255, 255, 255, 0};
    
    // iterate through the longest axis
    
    // x is the longest axis
    int i;
    if (abs(dx) > abs(dy)) {
      int index;
      for (i=1; i<=abs(dx); i++) {
	xn = xi+i;
	yn = nearbyint(y1 + i*m);
	index = c*(yn-1)+(xn-1);
	
	if (index >=0 && index <= (r-1)*(c-1)) {
	  memcpy(raster+index*4, black_pixel, 4);
	}
// 	else {
// 	  break;
// 	}
      }
    }
    // y is the longest axis
    else {
      int topToBottom = 0;
      // drawing from top to bottom
      if (yf-yi<0) {
	m *= -1;
	topToBottom = 1;
      }
      int index;
      for (i=1; i<=abs(dy); i++) {
	xn = nearbyint(xi + i/m);
	yn = topToBottom==0 ? yi+i : yi-i;
	index = c*(yn-1)+(xn-1);

	if (index >= 0 && index <= (r-1)*(c-1)) {
	    memcpy(raster+index*4, black_pixel, 4);
	}
// 	else  {
// 	  break;
// 	}
      }
    }
}

/**
  randomize raster
*/
void randomize_raster( unsigned char *raster, unsigned int width, unsigned int height)
{
//   unsigned char random_value = 0;
  GRand *random_generator = NULL;
  unsigned int size = 0;

  random_generator = g_rand_new_with_seed( time(NULL) );

  size = height * width * 4; /* 32 bits per pixel */
  
  int i;
  for (i=0; i<1000; i++) {
    dda_drawline(raster, height, width,
		g_rand_int_range(random_generator, 0, width-1),
		g_rand_int_range(random_generator, 0, height-1),
		g_rand_int_range(random_generator, 0, width-1),
		g_rand_int_range(random_generator, 0, height-1)
		);
  }
  
}


/**
 create the raster data 
*/
unsigned char *create_raster ( unsigned int width, unsigned int height )
{
  unsigned char *raster = NULL;
  unsigned int size = 0;

  /* CAIRO_FORMAT_RGB24 is used. Each pixel is a 32 bits quantity, with the upper 8 bits unused.
     Red, Green, Blue are stored in the remaining 24 bits in that order.
   */
  size = width * height * 4 ; /* 4 bytes for each pixel */

  raster = (unsigned char *) malloc( size );

  return raster;
}

/**
 create the image_surface
*/

cairo_surface_t *create_image_surface( unsigned char * raster, unsigned int width, unsigned int height)
{
  int stride = 0;

  stride = cairo_format_stride_for_width( CAIRO_FORMAT_RGB24, width );

  return cairo_image_surface_create_for_data( raster, CAIRO_FORMAT_RGB24, width, height, stride );
}


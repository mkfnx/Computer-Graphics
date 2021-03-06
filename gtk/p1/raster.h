/**
 \filename raster.h 

  author: Rafael Norman Saucedo Delgado
*/
#ifndef __RASTER_H__
#define __RASTER_H__

#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include <time.h>

/**
  randomize raster
*/
void randomize_raster( unsigned char *raster, unsigned int width, unsigned int height);

/**
 create the raster data 
*/
unsigned char *create_raster ( unsigned int width, unsigned int height );

/**
 create the image_surface
*/
cairo_surface_t *create_image_surface( unsigned char * raster, unsigned int width, unsigned int height);

#endif

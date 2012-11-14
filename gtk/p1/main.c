/** 
  Test program for gtk+ 

  author: Rafael Norman Saucedo Delgado
*/

#include <gtk/gtk.h>
#include "raster.h"

/**
  callback function for drawing


  cr is of type cairo_t, a cairo context.
  
  cairo_t is the main object used when drawing with cairo.
  In this case we don't need to create a cairo context, 
  the cairo context is form the drawing_area widget.
 
*/
gboolean
draw (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  guint width = 0;
  guint height = 0;
  //GdkRGBA color;
  cairo_surface_t * image_surface = NULL;
  unsigned char *raster = NULL;

  /* get the dimensions */
  width = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);

  raster = create_raster( 640, 480 );

  /* draw something random on the raster */
  randomize_raster( raster, 640, 480 );

  /* the raster is used to form a 'image_surface_t' for cairo functions */
  image_surface = create_image_surface( raster, 640, 480 );
  
  /* drawing is done with cairo library */
  cairo_set_source_surface( cr, image_surface, 0, 0 );

  cairo_paint( cr );

  /* free memory */
  free ( raster );
  cairo_surface_destroy( image_surface ); 

 return FALSE;
}


int main(int argc, char **argv)
{
  GtkWidget *window = NULL;
  GtkWidget *drawing_area = NULL; //<< widget for drawing


  /* before any gtk function */
  gtk_init( &argc, &argv );

  
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL );

  gtk_window_set_title( GTK_WINDOW(window), "Line Drawing");

  
  drawing_area = gtk_drawing_area_new();

  gtk_widget_set_size_request (drawing_area, 640, 480);

  g_signal_connect( window, "destroy", G_CALLBACK(gtk_main_quit), NULL );

  /* basic signal for drawing area */
  g_signal_connect( drawing_area, "draw", G_CALLBACK( draw ), NULL);

  /* gtk_window only can have one child widget */
  gtk_container_add( GTK_CONTAINER(window), drawing_area ); 
 
  gtk_widget_show_all( window ); 

  /* gives control to gtk */
  gtk_main();

  return 0;
}

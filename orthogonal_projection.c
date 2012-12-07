#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define COLUMNS (640)
#define ROWS (480)

int dda_drawline(int *raster, int r, int c, int xi, int yi, int xf, int yf);
void createFile(int *raster, int rows, int columns);

int main(int argc, char *argv[])
{
  int *raster;
  int r, c;
  
  double x[3], y[3], z[3];
  
  c = COLUMNS;
  r = ROWS;
  raster = (int *) malloc(sizeof(int)*r*c);
  
  if (raster == NULL) {
    perror("Reserving memory for raster\n");
    return;
  }
  memset(raster, 0, sizeof(int)*r*c);
  
  FILE *rawInput = fopen(argv[1], "r");
  
  int res;
  do {
    int i;
    
    for (i=0; i<3; i++) {
      res = fscanf(rawInput, "%lf %lf %lf", &x[i], &y[i], &z[i]);
//       printf("%f %f %f\n", x[i], y[i], z[i]);
    }
    
    dda_drawline(raster, r, c, x[0], y[0], x[1], y[1]);
    dda_drawline(raster, r, c, x[1], y[1], x[2], y[2]);
    dda_drawline(raster, r, c, x[0], y[0], x[2], y[2]);
    
  } while (res != EOF);
  fclose(rawInput);
  
  createFile(raster, r, c);
  
  free(raster);
  
  return 0;
}

void createFile(int *raster, int rows, int columns)
{
  FILE *f;
  
  f = fopen("OrthogonalProjection.ppm", "w");
  //srandom(time());

  fprintf(f, "P6\n");
  fprintf(f, "%d %d\n", columns, rows);
  fprintf(f, "255\n");
  
  unsigned char whitePixel[3] = {255, 255, 255};
  unsigned char blackPixel[3] = {0, 0, 0};

  int i;
  for (i=rows-1; i>=0; i--) {
    int j;
    for (j=0; j<columns; j++) {
      if (raster[columns*i+j]==1) {
	fwrite(blackPixel, 1, 3, f);
      }
      else {
	fwrite(whitePixel, 1, 3, f);
      }
    }
  }

  fclose(f);
}

int dda_drawline(int *raster, int r, int c, int xi, int yi, int xf, int yf)
{
  printf("Drawing line from (%d,%d) to (%d,%d)\n", xi,yi,xf,yf);
//   int m=(int)nearbyint((yf-yi)/(xf-xi));
    double dx = xf-xi;
    double dy = yf-yi;

    double m = dy/dx;
    printf("m: %f \n", m);
    double b = yi-m*xi;
    printf("b: %f \n", b);
    
    int xn, yn;
    double x1 = xi;
    double y1 = yi;
    
    // iterate through the longest axis
    int i;
    if (abs(dx) > abs(dy)) {
      printf("x\n");
      
      int index;
      for (i=1; i<=abs(dx); i++) {
	xn = xi+i;
	yn = nearbyint(y1 + i*m);
	printf("c*(yn-1)+(xn-1): %d*(%d-1)+(%d-1)\n", c, yn, xi);
	index = c*(yn-1)+(xn-1);
	printf("index: %d\n", index);
	
	if (index >=0 && index <= (r-1)*(c-1)) {
	  raster[index] = 1;
	  printf("(%d,%d)\n\n", xn, yn);
	}
// 	else {
// 	  break;
// 	}
      }
      
    }
    else {
      printf("y\n");
      
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
	printf("c*(yn-1)+(xn-1): %d*(%d-1)+(%d-1)\n", c, yn, xn);
	index = c*(yn-1)+(xn-1);
	printf("index: %d\n", index);
	printf("(%d,%d)\n", xn, yn);
	if (index >= 0 && index <= (r-1)*(c-1)) {
	    raster[index] = 1;
	    printf("(%d,%d)\n", xn, yn);
	}
// 	else  {
// 	  break;
// 	}
      }
      
    }
}
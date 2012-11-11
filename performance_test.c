#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int dda_drawline(int *raster, int r, int c, int xi, int yi, int xf, int yf);

int naive_drawline(int *raster, int r, int c, int xi, int yi, int xf, int yf);

int bresenham_drawline(int *raster, int r, int c, int xi, int yi, int xf, int yf);

void createFile(int *raster, int rows, int columns, char algorithm);

#define COLUMNS (640)
#define ROWS (480)

int main(int argc, char *argv[])
{
  int *raster;
  int r, c;
  int xi, yi, xf, yf;

  c = COLUMNS;
  r = ROWS;
  raster = (int *) malloc(sizeof(int)*r*c);
  
  if (raster == NULL) {
    perror("Reserving memory for raster\n");
    return;
  }
  
  srandom(time(NULL));
  
  int i;
  char algorithm;
  for(algorithm='A'; algorithm<='B'; algorithm++) {
    memset(raster, 0, sizeof(int)*r*c);
    for (i=750; i<=750; i*=10) {
      int j;
      for (j=1; j<=i; j++) {
	xi = random()%COLUMNS+1;
	yi = random()%ROWS+1;
	xf = random()%COLUMNS+1;
	yf = random()%ROWS+1;

	//swap points to draw from left to right
	if (xf-xi<0) {
	  int xt = xi;
	  int yt = yi;
	  xi = xf;
	  yi = yf;
	  xf = xt;
	  yf = yt;
	  printf("points swapped:\n");
	  printf("i: (%d, %d)\n", xi, yi);
	  printf("f: (%d, %d)\n", xf, yf);
	}

	double dx = xf-xi;
	double dy = yf-yi;
	int a;
	int index;

	//vertical line
	if (dx == 0) {
	  if (yf > yi) {
	    for (a=yi; a<=yf; a++) {
	      index = c*a+xi;
	      raster[index] = 1;
	    }
	  }
	  else {
	    for (a=yi; a>=yf; a--) {
	      index = c*a+xi;
	      raster[index] = 1;
	    }
	  }
	}
	//horizontal line
	else if (dy == 0){
	  for (a=xi; a<=xf; a++) {
	    index = c*yi+a;
	    raster[index] = 1;
	  }
	}
	//45 degrees line
	else if (dy == dx) {
	  int b;
	  for (a=xi, b=yi; a<=xf; a++, b++) {
	    index = c*b+a;
	    raster[index] = 1;
	  }
	}
	else {
	  if (algorithm=='B') {
	    naive_drawline(raster, r, c, xi, yi, xf, yf);
	  }
	  else if (algorithm=='A') {
	    dda_drawline(raster, r, c, xi, yi, xf, yf);
	  }
    // 	else {
    // 	  bresenham_drawline(raster, r, c, xi, yi, xf, yf);
    // 	}
	}
      }
    }
    createFile(raster, r, c, algorithm);
  }
  
  free(raster);
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

int naive_drawline(int *raster, int r, int c, int xi, int yi, int xf, int yf)
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
    double x1 = m*yi+b;
    double y1 = m*xi+b;
    
    // iterate through the longest axis
    int i;
    int index = 0;
    if (abs(dx) > abs(dy)) {
      printf("x\n");
      for (i=1; i<=dx; i++) {
	xn = x1+i;
	yn = nearbyint(m*xn + b);
// 	printf("c*(yn-1)+(xn-1): %d*(%f-1)+(%d-1)\n", c, x1, yi);
	index = c*(yn-1)+(xn-1);
// 	printf("index: %d\n", index);
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
// 	m *= -1;
	topToBottom = 1;
      }
      
//       printf("(%d,%d)\n\n", xn, yn);
      for (i=1; i<=abs(dy); i++) {
	yn = topToBottom==0 ? yi+i : yi-i;
	xn = nearbyint((yn-b)/m);
// 	printf("c*(xi-1)+(yn-1): %d*(%d-1)+(%d-1)\n", c, xi, yn);
	index = c*(yn-1)+(xn-1);
	printf("index: %d\n", index);
	printf("(%d,%d)\n\n", xn, yn);
	if (index >=0 && index <= (r-1)*(c-1)) {
	  raster[index] = 1;
// 	  printf("(%d,%d)\n\n", xn, yn);
	}
// 	else {
// 	  break;
// 	}
      }
    }
}

int bresenham_drawline(int *raster, int r, int c, int xi, int yi, int xf, int yf)
{
  
}

void createFile(int *raster, int rows, int columns, char algorithm)
{
  FILE *f;
  
  if (algorithm == 'B') {
    f = fopen("naive.ppm", "w");
  }
  else if (algorithm == 'A'){
    f = fopen("dda.ppm", "w");
  }
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
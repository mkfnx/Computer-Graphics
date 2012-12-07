#include <stdio.h>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 4) {
    printf("Use: translate <input> <output> <scale multiplier>\n");
  }
  
  FILE *fin = fopen(argv[1], "r");
  FILE *fout = fopen(argv[2], "w");
  
  int scale_mult = atoi(argv[3]);
  double x1, x2, x3, y1, y2, y3, z1, z2, z3;
  double minx, miny, minz;
  int p = 1;
  
  while (fscanf(fin, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3) != EOF) {
    if (p) {
      minx = x1;
      miny = y1;
      minz = z1;
      p = 0;
    }
    
    minx = min( minx, min(x1, min(x2, x3)) );
    miny = min( miny, min(y1, min(y2, y3)) );
    minz = min( minz, min(z1, min(z2, z3)) );
  }
  
  printf("mins: %lf, %lf, %lf\n", minx, miny, minz);
  
  rewind(fin);
  while (fscanf(fin, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3) != EOF) {
    fprintf(fout, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", (x1-minx)*scale_mult, (y1-miny)*scale_mult, (z1-minz)*scale_mult, (x2-minx)*scale_mult, (y2-miny)*scale_mult, (z2-minz)*scale_mult, (x3-minx)*scale_mult, (y3-miny)*scale_mult, (z3-minz)*scale_mult);
  }
  
  return 0;
}
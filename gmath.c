#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  normalize(normal);
  normalize(view);
  normalize(light[LOCATION]);
	
  color amb = calculate_ambient(alight, areflect);
  color fuse = calculate_diffuse(light, dreflect, normal);
  color lar = calculate_specular(light, sreflect, view, normal);

  i.red = amb.red + fuse.red + lar.red;
  i.green = amb.green + fuse.green + lar.green;
  i.blue = amb.blue + fuse.blue + lar.blue;
  limit_color(&i);
  return i;

 
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = areflect[RED]*alight.red;
  a.green = areflect[GREEN]*alight.green;
  a.blue = areflect[BLUE]*alight.blue;
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double fuser = dot_product(light[LOCATION], normal);
  
  d.red = light[COLOR][RED]*dreflect[RED]*fuser;
  d.green = light[COLOR][GREEN]*dreflect[GREEN]*fuser;
  d.blue = light[COLOR][BLUE]*dreflect[BLUE]*fuser;
  
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
 color s;
 double temp[3], cons;
 cons = 2 * dot_product(normal, light[LOCATION]);
 if (cons < 0) {
   cons = 0;
 }
 temp[0] = cons*normal[0]- light[LOCATION][0];
 temp[1] = cons*normal[1]- light[LOCATION][1];
 temp[2] = cons*normal[2]- light[LOCATION][2];
 cons = pow(dot_product(temp, view), 16);
 s.red = light[COLOR][RED] * sreflect[RED] * cons;
 s.green = light[COLOR][GREEN] * sreflect[GREEN] * cons;
 s.blue = light[COLOR][BLUE] * sreflect[BLUE] * cons;
 return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if(c->red > 255)
    c->red = 255;
  if(c->green > 255)
    c->green = 255;
  if(c->blue > 255)
    c->blue = 255;  
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double tude = sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));//mangniTUDE
  vector[0] = vector[0] / tude;
  vector[1] = vector[1] / tude;
  vector[2] = vector[2] / tude;

}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];;
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}

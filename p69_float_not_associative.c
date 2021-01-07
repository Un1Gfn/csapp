// Floating-point arithmetic is NOT associative

#include <stdio.h>

double a0=0,a1=0,a2=0,v=0,w=0,x=0,y=0;

int main(){

  a0=3.14;
  a1=1e20;
  a2=1e20;

  v=a0+a1;
  w=v-a2;
  printf("(%e) %f\n",v,w);

  y=a1-a2;
  x=a0+y;
  printf("(%f) %f\n",y,x);

  return 0;

}


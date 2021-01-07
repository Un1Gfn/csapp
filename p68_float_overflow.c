#include <assert.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>

const int a0=200;
const int a1=300;
const int a2=400;
const int a3=500;

int main(){

  int p=0;
  p=a0*a1*a2*a3;

  const char *const locale="en_US";
  const char *const s=setlocale(LC_NUMERIC,locale);
  assert(s&&0==strcmp(s,locale));
  printf("%'d\n",p);

  return 0;

}
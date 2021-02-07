// http://csapp.cs.cmu.edu/3e/ics3/code/data/show-bytes.c

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_bytes(const void *const start, const size_t len) {
  size_t i;
  for (i = 0; i < len; i++)
  	printf(" %.2X", ((const unsigned char*)start)[i]);
  printf("\n");
}

void show_int(const int x) {
    show_bytes(&x, sizeof(int));
}

void show_float(const float x) {
    show_bytes(&x, sizeof(float));
}

void show_pointer(const void *const x) {
  show_bytes(&x, sizeof(void*));
}

void test_show_bytes(const int val) {
  show_int(val);
  show_float((float)val);
  show_pointer(&val);
}

void simple_show_a() {
  const int val = 0x87654321;
  show_bytes(&val, 1); /* A. */
  show_bytes(&val, 2); /* B. */
  show_bytes(&val, 3); /* C. */
}

void simple_show_b() {
  const int val = 0x12345678;
  show_bytes(&val, 1); /* A. */
  show_bytes(&val, 2); /* B. */
  show_bytes(&val, 3); /* C. */
}

void float_eg() {
  int x = 3490593;
  printf("For x = %d\n", x);
  show_int(x);
  show_float((float)x);
  x = 3510593;
  printf("For x = %d\n", x);
  show_int(x);
  show_float((float)x);
}

void string_ueg() {
  const char *s = "ABCDEF";
  show_bytes(s, strlen(s)); 
}

void string_leg() {
  const char *s = "abcdef";
  show_bytes(s, strlen(s)); 
}

void show_twocomp() {
    short x = 12345; 
    short mx = -x;
    show_bytes(&x, sizeof(short));
    show_bytes(&mx, sizeof(short)); 
}

int main(int argc, char *argv[]){

  if (argc==1+1) {
    printf("calling test_show_bytes\n");
    test_show_bytes(strtol(argv[1], NULL, 0));
  }else if(argc==0+1){
    printf("calling show_twocomp\n");
    show_twocomp();
    printf("Calling simple_show_a\n");
    simple_show_a();
    printf("Calling simple_show_b\n");
    simple_show_b();
    printf("Calling float_eg\n");
    float_eg();
    printf("Calling string_ueg\n");
    string_ueg();
    printf("Calling string_leg\n");
    string_leg();
  }else{
    assert(false);
  }

  return 0;

}


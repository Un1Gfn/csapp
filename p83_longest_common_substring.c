// If there are more than one LCS's, only the first one is kept

// void swap(size_t *x,size_t *y){
//   const size_t z=*x;
//   *x=*y;
//   *y=*z;
// }

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define SWAP(X,Y) {\
  typeof(X) Z=X;\
  X=Y;\
  Y=Z;\
}

// 0000000000000000001 1000000111001
//           010001100 1000000111001 0000000000

const char *const a="00000000000000000011000000111001";
const char *const b="01000110010000001110010000000000";

const char *L=NULL; // The longer string
const char *S=NULL; // The shorter string
size_t Ll=0;
size_t Sl=0;

struct {
  size_t shift;
  size_t common;
  size_t i_pass;
} rec = {};
void show(){
  printf(" shift %zu\n",rec.shift);
  printf("     i %zu\n",rec.i_pass-rec.common);
  printf("common %zu\n",rec.common);
  printf("\n");
}

void settle(){
  assert(a);Sl=strlen(a);
  assert(b);Ll=strlen(b);
  if(Sl<Ll){
    S=a;
    L=b;
  }else{
    SWAP(Sl,Ll);
    S=b;
    L=a;
  }
}

void scan(const size_t shift){
  assert(shift<Sl);
  const char *const S_=S+shift;
  const size_t Sl_=Sl-shift;
  size_t i=0;
  size_t common=0;
  for(;;){
    if(i>Sl_-1)
      break;
    assert(i<=Ll-1);
    if(S_[i]==L[i]){
      ++common;
    }else{
      if(common>rec.common){
        show();
        rec.shift=shift;
        rec.common=common;
        rec.i_pass=i;
      }
      common=0;
    }
    ++i;
  }
}

void lgc(){
  settle();
  // scan(2);
  for(size_t shift=0;shift<Sl;++shift)
    scan(shift);
  show();
}

int main(){
  printf("\n");
  lgc();
  return 0;
}

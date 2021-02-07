#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <gmodule.h>

#define SWAP(X,Y) {\
  typeof(X) Z=X;\
  X=Y;\
  Y=Z;\
}

size_t u0len=0;const char *u0=NULL,*u=NULL;
size_t l0len=0;const char *l0=NULL,*l=NULL;
size_t clen=0;

typedef struct {
  size_t u0i;
  size_t l0i;
  size_t common;
} Rec;

size_t max_common=1;
GSList *slist=NULL;
#define FREE() {g_slist_free_full(slist,g_free);slist=NULL;}

void show(const gpointer r0,const gpointer garbage){
  assert(!garbage);
  const Rec *const r=(const Rec*)r0;
  #define u0i    r->u0i
  #define l0i    r->l0i
  #define common r->common
  const size_t w=(u0i>=l0i)?u0i:l0i;
  assert(u0i+common<=u0len);
  assert(l0i+common<=l0len);
  printf("u0 %*.*s-%.*s-%.*s\n",
    (int)w, (int)(u0i),                u0,
            (int)(common),             u0+u0i,
            (int)((u0len-u0i)-common), u0+u0i+common
  );
  printf("l0 %*.*s-%.*s-%.*s\n",
    (int)w, (int)(l0i),                l0,
            (int)(common),             l0+l0i,
            (int)((l0len-l0i)-common), l0+l0i+common
  );
  printf("\n");
  #undef u0i
  #undef l0i
  #undef common
  // printf("   u0i %zu\n",rec.u0i);
  // printf("   l0i %zu\n",rec.l0i);
  // printf("common %zu\n",rec.common);
  // printf("%.*s\n",);
}

void in(const char *const a,const char *const b){
  // u <- shorter string
  // l <- longer string
  assert(a);u0len=strlen(a);
  assert(b);l0len=strlen(b);
  assert(u0len>=1&&l0len>=1);
  if(u0len<=l0len){
    u0=a;l0=b;
  }else{
    SWAP(u0len,l0len);
    u0=b;l0=a;
  }
  //          u
  // u0:      ooo
  // l0: ooooo
  //          l
  u=u0;
  l=l0+l0len;
  clen=0;
}

/*---------------------------------------
         shift1   shift2   shift3
       u   ->   u   -> u     ->   u
       ooo ->   ooo -> ooo   -> ooo
  ooooo    -> ooooo -> ooooo ->   ooooo
       l   ->   l   -> l     ->   l
-----------------------------------------*/
void shift1(){ assert(u==u0); --l;      ++clen;}
void shift2(){ assert(u==u0); --l; assert(clen==u0len);}
void shift3(){      ++u; assert(l==l0); --clen;}

void scan(){
  size_t common=0;
  size_t i=0;
  for(;;){
    if(i<clen&&u[i]==l[i]){
      ++common;
    }else{
      if(common>=max_common){
        if(common>max_common)
          FREE();
        Rec *const t=g_malloc0(sizeof(Rec));
        *t=(Rec){
          .u0i=(i-common)+(u-u0),
          .l0i=(i-common)+(l-l0),
          .common=common
        };
        slist=g_slist_prepend(slist,t);
        max_common=common;
      }
      common=0;
      if(i>=clen)
        break;
    }
    ++i;
  }
}


int main(){

  printf("\n");

  // in(NULL,NULL);
  // in(NULL,"");
  // in("",NULL);
  // in("","");
  // in("0","0");
  // in("0","1");
  // in("1","0");
  // in("1","1");
  // in("0","00");
  // in("0","10");
  // in("10","01");
  // in("100001000","010010");

  // 0000000000000000001 1000000111001
  //           010001100 1000000111001 0000000000
  // in("00000000000000000011000000111001","01000110010000001110010000000000");

  // in("10000001110011011000000111001","01000110010000001110010000000000");

  in(
    "00000000001001111100100011111000",
    "01001010000111110010001111100000"
  );

  for(size_t i=0;i<u0len;++i){
    shift1();
    scan();
  }

  for(size_t i=0;i<(l0len-u0len);++i){
    shift2();
    scan();
  }

  for(size_t i=0;i<(u0len-1);++i){
    shift3();
    scan();
  }

  slist?g_slist_foreach(slist,show,NULL):printf("%p\n\n",NULL);
  FREE();

  return 0;

}

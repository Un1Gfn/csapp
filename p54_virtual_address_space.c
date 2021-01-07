#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <asm/vsyscall.h>

#define SZ 256

char self[SZ]={};
char libc[SZ]={};

const char *const string="string";
char globalvar='\0';

void *function(){
  char localvar='\0';
  return &localvar;
}

void readlink2(const char *const link,char *const target,const size_t sz){
  const ssize_t r=readlink(link,target,sz);
  assert(1<=r&&r<=SZ-1);
  assert(strlen(target)); 
}

int main(){

  readlink2("/proc/self/exe",self,SZ);
  readlink2("/usr/lib/libc.so.6",libc,SZ);

  void *heap=calloc(1,1);
  assert(heap);

  void *localvar=function();

  printf("\n");
  printf("%p in %s\n",(void*)(VSYSCALL_ADDR),"[vsyscall]");
  printf("\n");
  printf("%p in [stack]\n",&localvar);
  printf("\n");
  printf("%p in [heap]\n",heap);
  printf("\n");
  printf("%p in %s\n",&gettimeofday,libc);
  printf("%p in %s\n",&syscall,libc);
  printf("%p in %s\n",&close,libc);
  printf("%p in %s\n",&write,libc);
  printf("%p in %s\n",&read,libc);
  printf("%p in %s\n",&open,libc);
  printf("%p in %s\n",&fork,libc);
  printf("%p in %s\n",&getchar,libc);
  printf("%p in %s\n",&system,libc);
  printf("%p in %s\n",&srand,libc);
  printf("%p in %s\n",&exit,libc);
  printf("\n");
  printf("%p in %s\n",&globalvar,self);
  printf("%p in %s\n",&string,self);
  printf("%p in %s\n",string,self);
  printf("%p in %s\n",&function,self);
  printf("\n");
  printf("clear; sudo tac /proc/%d/maps ",getpid());fflush(stdout);getchar();
  printf("\n");

  free(heap);
  heap=NULL;

  return 0;

}

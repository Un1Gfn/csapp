// https://blog.holbertonschool.com/hack-the-virtual-memory-c-strings-proc/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SZ 256

char self[SZ]={};

char buf[SZ]={};

void get_self(){
  const ssize_t r=readlink("/proc/self/exe",self,SZ);
  assert(1<=r&&r<=SZ-1);
  assert(strlen(self)); 
}

unsigned long str2ul(const char *const s){
  _Static_assert(sizeof(void*)==8); // 64-bit pointer
  _Static_assert(sizeof(char*)==8); // 64-bit pointer
  _Static_assert(sizeof(unsigned long)==8); // 64-bit pointer
  assert(s);
  assert(strlen(s));
  return strtoul(s,NULL,16);
}

int main(int argc, char **argv){
  if(argc==1+2){
    assert(getuid()==0);
    char mempath[SZ]={};
    sprintf(mempath,"/proc/%s/mem",argv[1]);
    int memfd=open(mempath,O_RDWR);
    assert(memfd>=3);
    const unsigned long offset=str2ul(argv[2]);
    assert((__off_t)offset==lseek(memfd,offset,SEEK_SET));
    const char *new="xxx";
    size_t l=strlen(new)+1;
    assert((ssize_t)l==write(memfd,new,l));
    assert(0==close(memfd));
    memfd=-1;
  }else if(argc==1+0){
    printf("\n");
    get_self();
    printf("1. run 'sudo %s %d %p'\n",self,getpid(),&buf);
    printf("4. press enter here\n");
    printf("\n");
    sprintf(buf,"original");
    printf("%s",buf);fflush(stdout);
    getchar();
    printf("%s\n",buf);
    printf("\n");
  }else{
    assert(0);
  }
  return 0;

}

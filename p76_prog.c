// https://stackoverflow.com/questions/34960383
// https://github.com/sergev/elftoolchain/tree/master/documentation/libelf-by-example
// https://atakua.org/old-wp/wp-content/uploads/2015/03/libelf-by-example-20100112.pdf

#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
// #include <linux/elf.h>
// #include <elf.h>
#include <gelf.h>
#include <libelf.h>

#define LIBBSD_NETBSD_VIS
#include <bsd/vis.h>

#define SZ 1024
int fd=-1;
char path[SZ]={};
#define path_load() assert(1<=readlink("/proc/self/exe",path,SZ));printf("%s\n",path)
#define fd_open() assert(1<=(fd=open(path,O_RDONLY)))
#define fd_close() assert(0==close(fd));fd=-1

void plain(){

  #if defined (__LP64__)
  typedef Elf64_Ehdr ElfN_Ehdr;
  #define FMT "0x%lX"
  #else
  typedef Elf32_Ehdr ElfN_Ehdr;
  #define FMT "0x%X"
  #endif

  fd_open();

  ElfN_Ehdr header={};
  read(fd,&header,sizeof(ElfN_Ehdr));

  printf("0x%X ",header.e_ident[EI_MAG0]);
  printf("'%c' ",header.e_ident[EI_MAG1]);
  printf("'%c' ",header.e_ident[EI_MAG2]);
  printf("'%c' ",header.e_ident[EI_MAG3]);
  switch(header.e_ident[EI_CLASS]){
    case ELFCLASS32:printf("ELFCLASS32 ");break;
    case ELFCLASS64:printf("ELFCLASS64 ");break;
    default:assert(false);
  }
  switch(header.e_ident[EI_DATA]){
    case ELFDATA2LSB:printf("LE ");break;
    case ELFDATA2MSB:printf("BE ");break;
    default:assert(false);
  }
  assert(header.e_ident[EI_VERSION]==EV_CURRENT);
  // printf("%u ",header.e_ident[EI_OSABI]);
  assert(header.e_ident[EI_OSABI]==ELFOSABI_NONE);
  // printf("%u ",header.e_ident[EI_ABIVERSION]);
  assert(header.e_ident[EI_ABIVERSION]==0);

  // assert(header.e_type==ET_DYN);
  switch(header.e_type){
    // case ET_REL:  printf("relocatable ");break;
    case ET_EXEC: printf("executable ");break;
    case ET_DYN:  printf("shared_object ");break;
    default:assert(false);
  }

  switch(header.e_machine){
    case EM_386:printf("EM_386 ");break;
    case EM_X86_64:printf("EM_X86_64 ");break;
    default: assert(0);
  }

  assert(header.e_version==EV_CURRENT);

  printf("\n");

  printf("entry point "FMT"\n",header.e_entry);
  printf("program header table offset "FMT"\n",header.e_phoff);
  printf("section header table offset "FMT"\n",header.e_shoff);
  #undef FMT

  // printf("0x%X\n",header.e_flags);
  assert(header.e_flags==0);

  printf("ELF header %u bytes\n",header.e_ehsize);

  assert(header.e_phnum!=PN_XNUM);
  printf("program header table %u bytes/entry * %u entries = %u bytes\n",
    header.e_phentsize,
    header.e_phnum,
    header.e_phentsize*header.e_phnum
  );

  assert(header.e_shnum!=0);
  assert(header.e_shnum<SHN_LORESERVE);
  printf("section header table %u bytes/entry * %u entries = %u bytes\n",
    header.e_shentsize,
    header.e_shnum,
    header.e_shentsize*header.e_phnum
  );

  // printf("\n");

  fd_close();

}

void libelf(){

  fd_open();
  // printf("%s\n",elf_errmsg(-1));
  assert(EV_CURRENT==elf_version(EV_CURRENT));
  Elf *e=elf_begin(fd,ELF_C_READ,NULL);
  // assert(e);
  if(!e){
    printf("%s\n",elf_errmsg(-1));
    assert(false);
  }

  assert(ELF_K_ELF==elf_kind(e));
  #if defined (__LP64__)
  #define FMT "0x%lX"
  #else
  #define FMT "0x%llX"
  #endif
  printf("base offset "FMT"\n",elf_getbase(e));
  #undef FMT

  char *id=elf_getident(e,NULL);
  assert(id);
  static_assert(EI_ABIVERSION==(9-1)&&EI_PAD==(10-1));
  char dst[SZ]={};
  /*for(int i=0;i<EI_PAD;++i){
    vis(dst,id[i],0,'\0');
    printf("%s ",dst);
  }
  printf("\n");*/
  assert(1<=strvisx(dst,id,EI_PAD,VIS_CSTYLE|VIS_OCTAL));
  printf("%s\n",dst);
  id=NULL;

  const int class=gelf_getclass(e);
  if(class==ELFCLASS32){
    Elf32_Ehdr *ee=elf32_getehdr(e);
    assert(ee);
    ee=NULL;
  }else if(class==ELFCLASS64){
    Elf64_Ehdr *ee=elf64_getehdr(e);
    assert(ee);
    ee=NULL;
  }else{
    assert(false);
  }

  size_t t=0;
  assert(0==elf_getphdrnum(e,&t));
  printf("%zu program headers\n",t);
  assert(0==elf_getshdrnum(e,&t));
  printf("%zu sections\n",t);

  assert(0==elf_end(e));
  e=NULL;
  fd_close();

}

int main(){

  path_load();
  printf(".\n");
  plain();
  printf(".\n");
  libelf();

  return 0;
}

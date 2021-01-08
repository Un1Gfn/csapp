// https://stackoverflow.com/questions/34960383

#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
// #include <linux/elf.h>
#include <elf.h>

#if defined (__LP64__)
typedef Elf64_Ehdr ElfN_Ehdr;
#define FMT "0x%lX"
#else
typedef Elf32_Ehdr ElfN_Ehdr;
#define FMT "0x%X"
#endif

#define SZ 1024

int main(){

  char path[SZ]={};
  assert(1<=readlink("/proc/self/exe",path,SZ));
  printf("%s\n",path);
  int fd=-1;
  assert(1<=(fd=open(path,O_RDONLY)));

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

  printf("\n");

  assert(0==close(fd));
  fd=-1;

  return 0;
}

#include <assert.h>
#include <elf.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <vector>

#include <sys/stat.h>

#include "linux_syscall_support.h"

#if defined(__i386) || defined(__ARM_EABI__) || \
 (defined(__mips__) && _MIPS_SIM == _ABIO32)
typedef Elf32_auxv_t elf_aux_entry;
#elif defined(__x86_64) || defined(__aarch64__) || \
     (defined(__mips__) && _MIPS_SIM != _ABIO32)
typedef Elf64_auxv_t elf_aux_entry;
#endif

typedef __typeof__(((elf_aux_entry*) 0)->a_un.a_val) elf_aux_val_t;

std::vector<elf_aux_val_t> auxv_;

#if !defined(AT_SYSINFO_EHDR)
#define AT_SYSINFO_EHDR 33
#endif

#define AT_MAX AT_SYSINFO_EHDR

bool ReadAuxv(pid_t pid){
      //char auxv_path[NAME_MAX];
      //if (!BuildProcPath(auxv_path, pid_, "auxv")) {
      //  return false;
      //}
      
      printf("ReadAuxv...\n");

      char path[512] = {0};
      snprintf(path , sizeof(path)-1, "/proc/%d/auxv", pid);
      int fd = sys_open(path, O_RDONLY, 0);
      if (fd < 0) {
        return false;
      }

      elf_aux_entry one_aux_entry;
      bool res = false;
      while (sys_read(fd,
                      &one_aux_entry,
                      sizeof(elf_aux_entry)) == sizeof(elf_aux_entry) &&
             one_aux_entry.a_type != AT_NULL) {
        if (one_aux_entry.a_type <= AT_MAX) {
            //printf("type:%lld, value:%lld\n",  one_aux_entry.a_type, one_aux_entry.a_un.a_val);
          //auxv_[one_aux_entry.a_type] = one_aux_entry.a_un.a_val;
          res = true;
        }
      }
      sys_close(fd);
      
      printf("ReadAuxv end.\n");
      return res;
}

bool ReadAuxv2(pid_t pid){
      //char auxv_path[NAME_MAX];
      //if (!BuildProcPath(auxv_path, pid_, "auxv")) {
      //  return false;
      //}
      
      printf("ReadAuxv...\n");

      char path[512] = {0};
      snprintf(path , sizeof(path)-1, "/proc/%d/auxv", pid);
      
      struct stat st = {0};
      stat(path, &st);
      //printf("%u:%lu:%lu\n", st.st_mode, st.st_uid, st.st_gid);
      
      int permission = access(path, R_OK);
      printf("permission:%d\n", permission);
      
      FILE* fp = fopen(path, "rb");
      if (fp == NULL){
          return false;
      }
      
      printf("error:%d\n", ferror(fp));

      
      bool res = false;
      
      
      while(1){
          elf_aux_entry one_aux_entry;
          if (fread( &one_aux_entry,  sizeof(elf_aux_entry), 1, fp) != 1){
                int error = 0;
             if (error = ferror(fp)){
                  printf("error:%d\n", error);
              }
              if (feof(fp)){
                  printf("eof\n");
              }
              break;
          }
          
          if (one_aux_entry.a_type == AT_NULL){
              break;
          }
          
           if (one_aux_entry.a_type <= AT_MAX) {
            //printf("type:%zx, value:%zx\n",  one_aux_entry.a_type, one_aux_entry.a_un.a_val);
          //auxv_[one_aux_entry.a_type] = one_aux_entry.a_un.a_val;
          res = true;
        }
      }
      fclose(fp);
      
      printf("ReadAuxv end.\n");
      return res;
}

int main(int argc, char* argv[]){
    printf("Test elf...\n");
    
    printf("pid:%d,uid:%d,gid:%d\n", getpid(), getuid(), getgid());
    ReadAuxv2(getpid());
    pid_t child = fork();
    if (child == 0){
        pid_t p = getppid();
        printf("pid:%d,uid:%d,gid:%d\n", getpid(), getuid(), getgid());
        ReadAuxv2(p);
        sleep(5);
    }
    else{
        ReadAuxv2(child);
        int i = 2;
        do{
            sleep(1);
        }while(i--);
        printf("END\n");
    }
    
   printf("^^^^\n");
    return 0;
}
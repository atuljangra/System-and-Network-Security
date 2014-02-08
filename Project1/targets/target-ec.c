/*
 * EXTRA CREDIT
 * Thanks to: SkyLined.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char *env[])
{
  // 16 bytes each
  unsigned long p, a;

  // args is in %ecx, which is just below frame pointer. 
  if (argc != 3)
    {
      fprintf(stderr, "target-ec: argc != 3\n");
      exit(EXIT_FAILURE);
    }
  /*ebp and esp at this point differ by 36, 32 + 32 + 4(ecx)*/
  p = strtoul(argv[1], NULL, 0);
  a = strtoul(argv[2], NULL, 0);

  for (; *argv != NULL; argv++)
    memset(*argv, 0, strlen(*argv));
  for (; *env != NULL; env++)
    memset(*env, 0, strlen(*env));

  *(unsigned long *)p = a;

  return 0;
}

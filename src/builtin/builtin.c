#include "builtin.h"
#include <stdlib.h>

int shr_exit(char *name, char **args) {
  int status;
  if (args[1]) {
    status = atoi(args[1]);
  } else {
    status = 0;
  }
  exit(status);
  return status;
}

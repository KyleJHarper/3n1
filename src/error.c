#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>


/* show_error
 * Displays an error to stderr and aborts if exit_code is non-zero.  This function should basically only be used when we want to
 * exit after error message.
 */
void show_error(int exit_code, char *format, ...) {
  va_list list;
  fprintf(stderr, "ERROR: ");
  va_start(list, format);
  vfprintf(stderr, format, list);
  va_end(list);
  fprintf(stderr, "\nABORTING\n");
  if (exit_code != 0)
    exit(exit_code);
  printf("The show_error function was given exit code 0, this shouldn't ever happen.  Bailing.\n");
  exit(1);
}

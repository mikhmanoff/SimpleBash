#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  bool number_non_blank;
  bool mark_endline;
  bool number_all_lines;
  bool squeeze;
  bool tab;
  bool print_non_printable;
} Flags;

void cat_set_symbols(const char *table[static 256]);
void flag_set_endl(const char *table[static 256]);
void flag_set_tab(const char *table[static 256]);
void flag_set_nonprintable(const char *table[static 256]);
void cat(int argc, char *argv[], Flags flags, const char *table[static 256]);
void cat_output(FILE *file, Flags flags, const char *table[static 256]);

#endif  // S21_CAT_H_
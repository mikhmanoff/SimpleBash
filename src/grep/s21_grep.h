#ifndef S21_GREP_H_
#define S21_GREP_H_

#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

typedef struct Flags {
  // char *patterns;
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
} Flags;

void parse_flags(int argc, char *argv[], Flags *flags, char *patterns);
void parse_patterns_from_file(char *patterns);
void parse_args(int argc, char *argv[], Flags *flags, char *pattern);
void o_flag_implementation(char *line, regmatch_t *match, regex_t preg);
void output(int argc, Flags *flags, char *reg, char **filenames);

#endif  // S21_GREP_H_
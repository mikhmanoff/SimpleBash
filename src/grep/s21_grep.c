#define _GNU_SOURCE  // getline
#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flags flags = {'\0'};
  char patterns[BUFF_SIZE] = {'\0'};

  if (argc > 2) {
    parse_flags(argc, argv, &flags, patterns);
    parse_args(argc, argv, &flags, patterns);
  }

  return 0;
}

void parse_flags(int argc, char *argv[], Flags *flags, char *patterns) {
  char tmp[BUFF_SIZE] = {'\0'};
  int c = 0;

  for (; c != -1; c = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) {
    switch (c) {
      case 'e':
        flags->e = true;
        strcpy(tmp, patterns);
        sprintf(patterns, "%s%s%s", tmp, *patterns == '\0' ? "\0" : "|",
                optarg);
        break;
      case 'i':
        flags->i = true;
        break;
      case 'v':
        flags->v = true;
        break;
      case 'c':
        flags->c = true;
        break;
      case 'l':
        flags->l = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 'h':
        flags->h = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 'f':
        flags->f = true;
        parse_patterns_from_file(patterns);
        break;
      case 'o':
        flags->o = true;
        break;
      default:
        break;
    }
  }
}  // e : ivclnhsf : o

void parse_args(int argc, char *argv[], Flags *flags, char *patterns) {
  char reg[BUFF_SIZE] = {'\0'};
  char **filenames = NULL;

  filenames = (char **)malloc(BUFF_SIZE * sizeof(char *));

  for (int i = 0; i < BUFF_SIZE; i++) {
    filenames[i] = (char *)malloc(BUFF_SIZE * sizeof(char));
  }

  if (!flags->e && flags != NULL && !flags->f) {
    sprintf(reg, "%s", argv[optind++]);
  } else {
    sprintf(reg, "%s", patterns);
  }

  if (flags->e && flags != NULL) {
    sprintf(reg, "%s", patterns);
  }

  for (int i = optind, files_count = 0; i < argc; i++, files_count++) {
    strcpy(filenames[files_count], argv[i]);
  }

  output(argc, flags, reg, filenames);

  for (int i = 0; i < BUFF_SIZE; i++) {
    free(filenames[i]);
  }
  free(filenames);
}

void parse_patterns_from_file(char *patterns) {
  char buffer[BUFF_SIZE] = {'\0'};
  FILE *file;

  file = fopen(optarg, "r");
  if (file != NULL) {
    while (!feof(file)) {
      if (fgets(buffer, sizeof(buffer), file)) {
        strcat(patterns, buffer);
        if (patterns[strlen(patterns) - 1] == '\n')
          patterns[strlen(patterns) - 1] = '\0';
        strcat(patterns, "|");
      }
    }
    patterns[strlen(patterns) - 1] = '\0';

    fclose(file);
  } else {
    perror("s21_grep");
  }
}

void output(int argc, Flags *flags, char *reg, char **filenames) {
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  char *line = 0;
  regmatch_t match;
  size_t length = 0;
  int flag = REG_EXTENDED;
  int l_flag_count;
  int n_flag_count;
  int c_lines_count;

  if (flags->i) {
    flag |= REG_ICASE;
  }
  for (int i = 0; i < argc - optind; i++) {
    l_flag_count = 0;
    n_flag_count = 0;
    c_lines_count = 0;
    FILE *file = fopen(filenames[i], "r");
    if (file == NULL) {
      if (!flags->s) {
        perror("s21_grep");
      }
    } else {
      int reg_comp;
      if ((reg_comp = regcomp(preg, reg, flag)) == 0) {
        while (getline(&line, &length, file) > 0) {
          n_flag_count++;
          if (((reg_comp = regexec(preg, line, 1, &match, 0)) == 0) &&
              !flags->v) {
            if (flags->c || flags->l) {
              l_flag_count = 1;
              if (flags->c) {
                c_lines_count++;
                continue;
              }
              continue;
            }

            if (!flags->h && argc - optind > 1) {
              printf("%s:", filenames[i]);
            }

            if (flags->n) {
              printf("%d:", n_flag_count);
            }

            if (flags->o) {
              o_flag_implementation(line, &match, *preg);
              continue;
            }

            printf("%s", line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }

          } else if (flags->v && reg_comp == 1) {
            if (flags->c || flags->l) {
              l_flag_count = 1;

              if (flags->c) {
                c_lines_count++;
                continue;
              }
              continue;
            }

            if (!flags->h && argc - optind > 1) {
              printf("%s:", filenames[i]);
            }

            if (flags->n) {
              printf("%d:", n_flag_count);
            }

            printf("%s", line);

            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          }
        }

        if (flags->c) {
          if (!flags->h && argc - optind > 1) {
            printf("%s:", filenames[i]);
          }
          if (flags->l && c_lines_count > 1) {
            c_lines_count = 1;
          }
          printf("%d\n", c_lines_count);
        }
        if (flags->l && l_flag_count) {
          printf("%s\n", filenames[i]);
        }
      }
      if (file != NULL) fclose(file);
    }
  }
  if (line != NULL) free(line);
  if (line != NULL) regfree(preg);
}

void o_flag_implementation(char *line, regmatch_t *match, regex_t preg) {
  while (1) {
    if (regexec(&preg, line, 1, match, 0) != 0) {
      break;
    }
    for (int i = match[0].rm_so; i < match[0].rm_eo; ++i) {
      printf("%c", line[i]);
    }
    printf("\n");
    line += match[0].rm_eo;
  }
}
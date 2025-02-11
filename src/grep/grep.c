#include "grep.h"

int main(int argc, char* argv[]) {
  opt options = {0};
  char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH] = {0};
  options_switch(argc, argv, &options, patterns);

  if (optind >= argc) {
    fprintf(stderr, "No pattern specified\n");
    exit(1);
  }

  if (!options.e && !options.f && optind < argc) {
    strncpy(patterns[0], argv[optind++], MAX_PATTERN_LENGTH);
    patterns[1][0] = '\0';
  }

  for (int i = optind; i < argc; i++) {
    grep(argc, patterns, argv[i], options);
  }

  return 0;
}

void options_switch(int argc, char* argv[], opt* options,
                    char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH]) {
  int opti;
  int pattern_index = 0;
  struct option long_options[] = {{NULL, 0, NULL, 0}};

  while ((opti = getopt_long(argc, argv, "e:ivclnhsof:", long_options, NULL)) !=
         -1) {
    switch (opti) {
      case 'e':
        if (!options->f) {
          if (pattern_index >= MAX_PATTERNS) {
            fprintf(stderr, "Too many -e options. Maximum is %d\n",
                    MAX_PATTERNS);
            exit(1);
          }
          options->e = 1;
          strncpy(patterns[pattern_index], optarg, MAX_PATTERN_LENGTH);
          pattern_index++;
          if (pattern_index < MAX_PATTERNS) {
            patterns[pattern_index][0] = '\0';
          }
        }
        options->f = 0;
        break;

      case 'f':
        if (options->e) {
          pattern_index = 0;
        }
        options->e = 1;
        options->f = 1;
        f_opt(optarg, patterns, MAX_PATTERNS);
        break;

      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->o = 0;
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'o':
        options->o = 1;
        if (options->v) options->o = 0;
        break;

      default:
        exit(1);
    }
  }
}

void grep(int argc, char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH],
          const char* file_name, opt options) {
  FILE* f = fopen(file_name, "r");

  if (f != NULL) {
    regex_t re;

    init_regex(&re, patterns, options);

    char line[MAX_LINE_LENGTH];
    int line_num = 1;
    int match_count = 0;
    int file_count = argc - optind;
    int print_file_name = file_count > 1;

    while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
      int match = check_match(&re, line);

      if (options.v) {
        match = !match;
      }

      if_files(match, options, file_name, print_file_name);
      n_opt(match, options, line_num);
      o_opt(match, options, line, patterns);

      if (match && !options.l && !options.c && !options.o) {
        printf("%s", line);
        if (feof(f)) {
          printf("\n");
        }
      }

      match_count += match;
      line_num++;
    }
    regfree(&re);
    fclose(f);
    c_l_opt(match_count, options, file_name, print_file_name);
  } else {
    s_opt(options, file_name);
  }
}

void init_regex(regex_t* re, char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH],
                opt options) {
  char combined_pattern[MAX_LINE_LENGTH] = "";

  for (int i = 0; patterns[i][0] != '\0'; i++) {
    strncat(combined_pattern, patterns[i],
            MAX_PATTERN_LENGTH - strlen(combined_pattern) - 1);
    strncat(combined_pattern, "|",
            MAX_LINE_LENGTH - strlen(combined_pattern) - 1);
  }

  size_t len = strlen(combined_pattern);
  if (len > 0 && combined_pattern[len - 1] == '|') {
    combined_pattern[len - 1] = '\0';
  }

  regcomp(re, combined_pattern, REG_EXTENDED | (options.i ? REG_ICASE : 0));
}

int check_match(regex_t* re, const char* line) {
  int match = 0;
  int result = regexec(re, line, 0, NULL, 0);

  if (result == 0) {
    match = 1;
  } else if (result != REG_NOMATCH) {
    fprintf(stderr, "Error in regexec\n");
  }

  return match;
}

void if_files(int match, opt options, const char* file_name,
              int print_file_name) {
  if (!options.h && match && print_file_name && !options.l && !options.c) {
    printf("%s:", file_name);
  }
}

void n_opt(int match, opt options, int line_num) {
  if (match && options.n && !options.s && !options.c && !options.l) {
    printf("%d:", line_num);
  }
}

void o_opt(int match, opt options, const char* line,
           char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH]) {
  if (options.o && match && !options.l && !options.c) {
    regex_t re;
    init_regex(&re, patterns, options);

    regmatch_t pmatch[1];
    const char* pos = line;

    while (regexec(&re, pos, 1, pmatch, 0) == 0) {
      int start = pmatch[0].rm_so;
      int end = pmatch[0].rm_eo;

      for (int j = start; j < end; j++) {
        printf("%c", line[pos - line + j]);
      }
      printf("\n");

      pos += end;
    }

    regfree(&re);
  }
}

void c_l_opt(int match_count, opt options, const char* file_name,
             int print_file_name) {
  if (options.c) {
    if (print_file_name && !options.h) {
      printf("%s:", file_name);
    }
    if ((match_count > 1 && options.l)) {
      match_count = 1;
      printf("%d\n", match_count);
    } else {
      printf("%d\n", match_count);
    }
  }

  if (options.l && match_count > 0) {
    printf("%s\n", file_name);
  }
}

void s_opt(opt options, const char* file_name) {
  if (options.s) {
    exit(1);
  } else {
    printf("grep: %s: No such file or directory\n", file_name);
  }
}

void f_opt(const char* file_name,
           char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH], int max_patterns) {
  FILE* file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "Error opening file: %s\n", file_name);
    exit(1);
  }

  char line[MAX_PATTERN_LENGTH];
  int pattern_count = 0;

  while (fgets(line, MAX_PATTERN_LENGTH, file) != NULL &&
         pattern_count < max_patterns) {
    line[strcspn(line, "\n")] = 0;
    strncpy(patterns[pattern_count++], line, MAX_PATTERN_LENGTH);
    patterns[pattern_count][0] = '\0';
  }

  fclose(file);
}

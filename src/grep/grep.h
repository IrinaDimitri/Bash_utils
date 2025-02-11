#ifndef SRC_GREP_GREP_H_
#define SRC_GREP_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATTERNS 10
#define MAX_PATTERN_LENGTH 100
#define MAX_LINE_LENGTH 1024

typedef struct options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} opt;

void grep(int argc, char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH],
          const char* file_name, opt options);
void options_switch(int argc, char* argv[], opt* options,
                    char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH]);
void init_regex(regex_t* re, char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH],
                opt options);
void f_opt(const char* file_name,
           char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH], int max_patterns);
int check_match(regex_t* re, const char* line);
void if_files(int match, opt options, const char* file_name,
              int print_file_name);
void n_opt(int match, opt options, int line_num);
void o_opt(int match, opt options, const char* line,
           char patterns[MAX_PATTERNS][MAX_PATTERN_LENGTH]);
void c_l_opt(int match_count, opt options, const char* file_name,
             int print_file_name);
void s_opt(opt options, const char* file_name);

#endif  //  SRC_GREP_GREP_H_

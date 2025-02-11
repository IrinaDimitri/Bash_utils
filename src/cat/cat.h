#ifndef SRC_CAT_H_
#define SRC_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} opt;

opt options_switch(int argc, char* argv[]);
void cat(char* argv[], int count, opt options);
void optis(opt options, FILE* f, int before, int counter, int c, int j);

#endif

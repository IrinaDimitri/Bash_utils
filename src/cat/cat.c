#include "cat.h"

int main(int argc, char* argv[]) {
  opt options = options_switch(argc, argv);
  cat(argv, argc, options);
  return 0;
}

opt options_switch(int argc, char* argv[]) {
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {0, 0, 0, 0}};
  int opti = getopt_long(argc, argv, "+bevEnstT", long_options, 0);
  opt options = {0, 0, 0, 0, 0, 0};
  for (; opti != -1;
       opti = getopt_long(argc, argv, "+bevEnstT", long_options, 0)) {
    switch (opti) {
      case 'b':
        options.b = 1;
        break;
      case 'e':
        options.e = 1;
        options.v = 1;
        break;
      case 'n':
        options.n = 1;
        break;
      case 's':
        options.s = 1;
        break;
      case 't':
        options.t = 1;
        options.v = 1;
        break;
      case 'v':
        options.v = 1;
        break;
      case 'T':
        options.t = 1;
        break;
      case 'E':
        options.e = 1;
        break;
      default:
        fprintf(stderr, "error");
        break;
    }
  }
  return options;
}

void cat(char* argv[], int count, opt options) {
  for (int i = 1; i < count; i++) {
    FILE* f = fopen(argv[i], "r");
    if (f != NULL) {
      int before = '\n';
      int counter = 0;
      int j = 1;
      int c = 0;
      optis(options, f, before, counter, c, j);
      fclose(f);
    }
  }
}

void optis(opt options, FILE* f, int before, int counter, int c, int j) {
  while ((c = fgetc(f)) != EOF) {
    if (options.s) {
      if (c == '\n' && (before == '\n' || before == '\0')) {
        counter++;
      } else {
        counter = 0;
      }
    }
    if (counter <= 1) {
      if (options.n) {
        if ((before == '\n') && !options.b) {
          printf("%6d\t", j);
          j++;
        }
      }
      if (options.b) {
        if (before == '\n' && c != '\n') {
          printf("%6d\t", j);
          j++;
        }
      }

      if (options.e) {
        if (c == '\n') {
          printf("$");
        }
      }

      if (options.t) {
        if (c == '\t') {
          printf("^");
          c = 'I';
        }
      }
      if (options.v) {
        if (c > 127 && c < 160) {
          printf("M-^");
        }
        if ((c < 32 && c != '\n' && c != '\t') || c == 127) {
          printf("^");
        }
        if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t') {
          c = c > 126 ? c - 128 + 64 : c + 64;
        }
      }
      printf("%c", c);
      before = c;
    }
  }
}

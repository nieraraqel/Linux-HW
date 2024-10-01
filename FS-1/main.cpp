#include <iostream>

void cat(char *name) {
  FILE *f = fopen(name, "rt");

  if (f != NULL) {
    int c = fgetc(f);
    while (c != EOF) {
      putc(c, stdout);
      c = fgetc(f);
    }
    fclose(f);
  }
}
int main(int argc, char **argv) {
  if (argc < 2){
    std::cout << "error" << std::endl;
  }
  for (int i = 1; i < argc; ++i){
    cat(argv[i]);
  }
  return 0;
}

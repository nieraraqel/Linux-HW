#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

void cat(char *name) {
  int file = open(name, O_RDONLY);

  if(file == -1){
    printf("Error\n");
    exit(errno);
  }
  char* buffer = (char*)malloc(BUFFER_SIZE + 1);
  long readBytes = 0;

  while(true){
    readBytes = read(file, buffer, BUFFER_SIZE);

    if(readBytes == -1){
      printf("Error\n");
      exit(errno);
    }
    if(readBytes == 0){
      break;
    }
    buffer[readBytes] = '\0';
    printf("%s", buffer);
  }

  free(buffer);
  close(file);
}

int main(int argc, char **argv) {
  if (argc < 2){
    printf("Error\n");
    exit(1);
  }
  for (int i = 1; i < argc; ++i){
    cat(argv[i]);
  }

  return 0;
}

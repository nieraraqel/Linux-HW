#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE 4096

void deletefunc(char *source) {
  int sourceFd = open(source, O_RDWR);
  if(sourceFd == -1){
    printf("Error2\n");
    exit(errno);
  }

  char *buffer = (char*)malloc(BUFFER_SIZE);

  long readBytes = 0;

  while(true){
    readBytes = read(sourceFd, buffer, BUFFER_SIZE);
    if(readBytes == -1){
      printf("Error3\n");
      exit(errno);
    }

    for(int i = 0; i < readBytes; ++i) {
      buffer[i] = '\0';
    }

    if(readBytes == 0){
      break;
    }

    lseek(sourceFd, -readBytes, SEEK_CUR);

    long writeBytes = write(sourceFd, buffer, (size_t)readBytes);
    if(writeBytes == -1){
      printf("Error4\n");
      exit(errno);
    }

  }
  close(sourceFd);
  free(buffer);

  unlink(source);
}

int main(int argc, char **argv) {
  if (argc != 2){
    printf("Error\n");
  }
  deletefunc(argv[1]);

  return 0;
}

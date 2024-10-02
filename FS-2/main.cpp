#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4097

void copy(char *source, char *destination) {
  int sourceFd = open(source, O_RDONLY);
  if(sourceFd == -1){
    printf("Error\n");
    exit(errno);
  }

  int destinationFd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
  if(destinationFd == -1){
    printf("Error\n");
    exit(errno);
  }

  char *buffer = (char*)malloc(BUFFER_SIZE);

  long readBytes = 0;

  while(1 == 1){
    readBytes = read(sourceFd, buffer, BUFFER_SIZE);
    if(readBytes == -1){
      printf("Error\n");
      exit(errno);
    }
    if(readBytes == 0){
      break;
    }

    long writeBytes = write(destinationFd, buffer, (size_t)readBytes);
    if(writeBytes == -1){
      printf("Error\n");
      exit(errno);
    }
  }

  close(sourceFd);
  close(destinationFd);

  free(buffer);
}
int main(int argc, char **argv) {
  if (argc != 3){
    printf("Error\n");
  }
  copy(argv[1], argv[2]);

  return 0;
}
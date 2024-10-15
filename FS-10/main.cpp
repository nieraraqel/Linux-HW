#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <cmath>
#include <limits>

#define BUFFER_SIZE 4096

int main(int argc, char** argv){

  if(argc == 2){
    std::cerr << "Missing filename argument" << std::endl;
    exit(1);
  }

  const char* source_filename = argv[1];
  const char* destination_filename = argv[2];

  int sourceFd = open(source_filename, O_RDONLY);

  if(sourceFd == -1){
    std::cerr << "Error while opening the file " << source_filename << ". " << strerror(errno) << std::endl;
    exit(errno);
  }

  int destinationFd = open(destination_filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);

  if(destinationFd == -1){
    std::cerr << "Error while opening the file " << destination_filename << ". " << strerror(errno) << std::endl;
    exit(errno);
  }

  char *buffer = (char*)malloc(BUFFER_SIZE + 1);
  long readBytes = 0;

  off_t totalBytes = 0;
  off_t holeBytes = 0;

  while(true){
    readBytes = read(sourceFd, buffer, BUFFER_SIZE);

    if(readBytes == 0){
      std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
      exit(errno);
    }

    for (int i = 0; i < readBytes; ++i) {
      if (buffer[i] == '\0') {
        holeBytes++;
      }
    }

    long writeBytes = write(destinationFd, buffer, (size_t)readBytes);

    if(writeBytes == -1){
      std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
      exit(errno);
    }

  }

  off_t fileSize = lseek(sourceFd, 0, SEEK_END);
  if (fileSize > totalBytes) {
    holeBytes += fileSize - totalBytes;
  }

  std::cout << "Successfully copied " << totalBytes << " bytes (data: " << totalBytes - holeBytes << ", hole: " << holeBytes << ")." << std::endl;

  close(sourceFd);
  close(destinationFd);

  free(buffer);

  return 0;
}
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

  if(argc != 3){
    std::cerr << "Missing filename arguments or there are more than 2 arguments " << std::endl;
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

  long readBytes = 0;
  int offset = 0;

  off_t totalBytes = 0;
  off_t holeBytes = 0;

  while(true){
    off_t holeOffset = lseek(sourceFd, offset, SEEK_HOLE);

    if (holeOffset < 0) {
      if(errno == ENXIO){
        break;
      }
      std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
      exit(errno);
    }

    if (holeOffset > offset) {
      off_t holeSize = holeOffset - offset;

      if (lseek(destinationFd, offset, SEEK_SET) == -1) {
        std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
        exit(errno);
      }
      char* buffer = (char*)malloc(holeSize);

      if (write(destinationFd, buffer, holeSize) != holeSize) {
        std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
        free(buffer);
        exit(errno);
      }

      holeBytes += offset;

      free(buffer);
    }


    lseek(sourceFd, holeOffset, SEEK_DATA);

    char* buffer = (char*)malloc(BUFFER_SIZE);

    readBytes = read(sourceFd, buffer, BUFFER_SIZE);

    if(readBytes < 0) {
      std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
      exit(errno);
    }

    if(readBytes == 0) {
      break;
    }

    if (write(destinationFd, buffer, readBytes) != readBytes) {
      std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
      exit(errno);
    }

    offset = holeOffset + readBytes;

    free(buffer);
  }

  off_t fileSize = lseek(sourceFd, 0, SEEK_END);

  std::cout << "Successfully copied " << fileSize << " bytes (data: " << fileSize - holeBytes << ", hole: " << holeBytes << ")." << std::endl;

  close(sourceFd);
  close(destinationFd);

  return 0;
}
